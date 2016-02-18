PIC_INIT     macro

  call      SetupConfig
  call      SetupIntOsc
  call      SetupIO
  call      SetupFVRC

  call      WatchdogPeriodInit
            ; Watchdog Timer will now reset the PIC
            ; when it idles for 16s without a CLRWDT
            ; instruction

  call      I2CINIT
            ; Setup I2C SDA and SCL on RC4 and RC3 at 7.75kHz
            ; Setup EUSART at 150 baud.
            ; Datasheet says to run this before enabling timer

  endm
