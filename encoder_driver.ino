/* *************************************************************
   Encoder definitions - adapted for Teensy 4.0 with Encoder library
   ************************************************************ */

#ifdef USE_BASE

#ifdef ENCODER_LIBRARY
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return leftEnc.read(); // Invert left encoder reading
    else return -rightEnc.read();
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT) leftEnc.write(0);
    else rightEnc.write(0);
  }
#else
  #error An encoder driver must be selected!
#endif

/* Reset both encoders */
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

#endif