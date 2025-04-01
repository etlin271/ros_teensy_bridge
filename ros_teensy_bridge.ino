/*********************************************************************
 *  Adapted ROSArduinoBridge for Teensy 4.0 with Dual DRV8871 Motors
 *********************************************************************/

#define USE_BASE      // Enable the base controller code

/* Define the motor controller and encoder library */
#ifdef USE_BASE
   #define DRV8871_MOTOR_DRIVER  // Custom driver for DRV8871
   #define ENCODER_LIBRARY       // Use Encoder.h library
#endif

/* Serial port baud rate */
#define BAUDRATE     115200

/* Maximum PWM signal */
#define MAX_PWM      255

#include "Arduino.h"

/* Include definition of serial commands */
#include "commands.h"

/* Motor driver and encoder function definitions */
#include "motor_driver.h"
#include "encoder_driver.h"

#ifdef USE_BASE
  /* PID parameters and functions */
  #include "diff_controller.h"

  /* Run the PID loop at 30 times per second */
  #define PID_RATE           30     // Hz
  const int PID_INTERVAL = 1000 / PID_RATE;
  unsigned long nextPID = PID_INTERVAL;

  /* Plotting interval (e.g., every 200 ms, ~5 Hz) */
  #define PLOT_INTERVAL      200
  unsigned long nextPlot = PLOT_INTERVAL;

  /* Auto-stop interval */
  #define AUTO_STOP_INTERVAL 2000
  long lastMotorCommand = AUTO_STOP_INTERVAL;
#endif

/* Variable initialization */
int arg = 0;
int idx = 0;
char chr;
char cmd;
char argv1[16];
char argv2[16];
long arg1;
long arg2;

void resetCommand() {
  cmd = 0;
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
  arg = 0;
  idx = 0;
}

int runCommand() {
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);
  
  switch(cmd) {
    case GET_BAUDRATE:
      Serial.println(BAUDRATE);
      break;
    case ANALOG_READ:
      Serial.println(analogRead(arg1));
      break;
    case DIGITAL_READ:
      Serial.println(digitalRead(arg1));
      break;
    case ANALOG_WRITE:
      analogWrite(arg1, arg2);
      Serial.println("OK"); 
      break;
    case DIGITAL_WRITE:
      digitalWrite(arg1, arg2 == 0 ? LOW : HIGH);
      Serial.println("OK"); 
      break;
    case PIN_MODE:
      pinMode(arg1, arg2 == 0 ? INPUT : OUTPUT);
      Serial.println("OK");
      break;
#ifdef USE_BASE
    case READ_ENCODERS:
      Serial.print(readEncoder(LEFT));
      Serial.print(" ");
      Serial.println(readEncoder(RIGHT));
      break;
    case RESET_ENCODERS:
      resetEncoders();
      resetPID();
      Serial.println("OK");
      break;
    case MOTOR_SPEEDS:
      lastMotorCommand = millis();
      if (arg1 == 0 && arg2 == 0) {
        setMotorSpeeds(0, 0);
        resetPID();
        moving = 0;
      } else {
        moving = 1;
      }
      leftPID.TargetTicksPerFrame = arg1;
      rightPID.TargetTicksPerFrame = arg2;
      Serial.println("OK"); 
      break;
    case MOTOR_RAW_PWM:
      lastMotorCommand = millis();
      resetPID();
      moving = 0;
      setMotorSpeeds(arg1, arg2);
      Serial.println("OK"); 
      break;
    case UPDATE_PID: {
      char *p = argv1;
      char *str;
      int pid_args[4];
      int i = 0;
      while ((str = strtok_r(p, ":", &p)) != NULL) {
        pid_args[i] = atoi(str);
        i++;
      }
      Kp = pid_args[0];
      Kd = pid_args[1];
      Ki = pid_args[2];
      Ko = pid_args[3];
      Serial.println("OK");
      break;
    }
#endif
    default:
      Serial.println("Invalid Command");
      break;
  }
  return 0;
}

void setup() {
  Serial.begin(BAUDRATE);
  while (!Serial) {
    ; // Wait for serial port to connect (Teensy-specific)
  }

#ifdef USE_BASE
  initMotorController();
  resetPID();
#endif
}

void loop() {
  while (Serial.available() > 0) {
    chr = Serial.read();
    if (chr == 13) { // Carriage return
      if (arg == 1) argv1[idx] = '\0';
      else if (arg == 2) argv2[idx] = '\0';
      runCommand();
      resetCommand();
    } else if (chr == ' ') {
      if (arg == 0) arg = 1;
      else if (arg == 1) {
        argv1[idx] = '\0';
        arg = 2;
        idx = 0;
      }
    } else {
      if (arg == 0) {
        cmd = chr;
      } else if (arg == 1) {
        argv1[idx] = chr;
        idx++;
      } else if (arg == 2) {
        argv2[idx] = chr;
        idx++;
      }
    }
  }

#ifdef USE_BASE
  if (millis() > nextPID) {
    updatePID();
    nextPID += PID_INTERVAL;

    // Plot desired and current speeds when in PID mode, synchronized with PID update
    if (moving && millis() > nextPlot) {
      long leftCurrentSpeed = leftPID.Encoder - leftPID.PrevEnc;  // Ticks per frame
      long rightCurrentSpeed = rightPID.Encoder - rightPID.PrevEnc;
      Serial.print(leftPID.TargetTicksPerFrame);  // Left target
      Serial.print("\t");
      Serial.print(leftCurrentSpeed);             // Left current
      Serial.print("\t");
      Serial.print(rightPID.TargetTicksPerFrame); // Right target
      Serial.print("\t");
      Serial.print(rightCurrentSpeed);            // Right current
      Serial.println();
      Serial.flush(); // Ensure data is sent immediately
      nextPlot += PLOT_INTERVAL;
    }
  }

  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {
    setMotorSpeeds(0, 0);
    moving = 0;
  }
#endif
}