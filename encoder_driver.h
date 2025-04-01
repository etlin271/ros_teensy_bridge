/* *************************************************************
   Encoder driver function definitions - adapted for Encoder library
   ************************************************************ */

#ifdef ENCODER_LIBRARY
  #include <Encoder.h>

  #define LEFT_ENC_PIN_A 3   // Left encoder A
  #define LEFT_ENC_PIN_B 4   // Left encoder B
  #define RIGHT_ENC_PIN_A 22 // Right encoder A
  #define RIGHT_ENC_PIN_B 21 // Right encoder B

  Encoder leftEnc(LEFT_ENC_PIN_A, LEFT_ENC_PIN_B);
  Encoder rightEnc(RIGHT_ENC_PIN_A, RIGHT_ENC_PIN_B);
#endif

long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();