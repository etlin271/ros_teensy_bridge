/* *************************************************************
   Motor driver definitions - adapted for DRV8871 on Teensy 4.0
   ************************************************************ */

#ifdef USE_BASE

#ifdef DRV8871_MOTOR_DRIVER
  void initMotorController() {
    pinMode(LEFT_MOTOR_IN1, OUTPUT);
    pinMode(LEFT_MOTOR_IN2, OUTPUT);
    pinMode(RIGHT_MOTOR_IN1, OUTPUT);
    pinMode(RIGHT_MOTOR_IN2, OUTPUT);
    setMotorSpeeds(0, 0); // Ensure motors start stopped
  }

  void setMotorSpeed(int i, int spd) {
    if (spd > MAX_PWM) spd = MAX_PWM;
    else if (spd < -MAX_PWM) spd = -MAX_PWM;

    if (i == LEFT) {
      if (spd > 0) {
        digitalWrite(LEFT_MOTOR_IN2, LOW);
        analogWrite(LEFT_MOTOR_IN1, spd);
      } else if (spd < 0) {
        digitalWrite(LEFT_MOTOR_IN1, LOW);
        analogWrite(LEFT_MOTOR_IN2, abs(spd));
      } else {
        analogWrite(LEFT_MOTOR_IN1, 0);
        analogWrite(LEFT_MOTOR_IN2, 0);
      }
    } else { // RIGHT
      if (spd > 0) {
        digitalWrite(RIGHT_MOTOR_IN2, LOW);
        analogWrite(RIGHT_MOTOR_IN1, spd);
      } else if (spd < 0) {
        digitalWrite(RIGHT_MOTOR_IN1, LOW);
        analogWrite(RIGHT_MOTOR_IN2, abs(spd));
      } else {
        analogWrite(RIGHT_MOTOR_IN1, 0);
        analogWrite(RIGHT_MOTOR_IN2, 0);
      }
    }
  }

  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }
#else
  #error A motor driver must be selected!
#endif

#endif