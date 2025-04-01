/* *************************************************************
   Motor driver function definitions - adapted for DRV8871
   ************************************************************ */

#ifdef DRV8871_MOTOR_DRIVER
  #define LEFT_MOTOR_IN1 11
  #define LEFT_MOTOR_IN2 10
  #define RIGHT_MOTOR_IN1 14
  #define RIGHT_MOTOR_IN2 15
#endif

void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);