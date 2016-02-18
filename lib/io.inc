
IO_FXNS		CODE

SetupConfig
	; CONFIG1
	; __config 0x37F4
	 __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_NSLEEP & _PWRTE_OFF & _MCLRE_ON & _CP_OFF & _CPD_OFF & _BOREN_ON & _CLKOUTEN_ON & _IESO_ON & _FCMEN_ON
	; CONFIG2
	; __config 0xFFFF
	__CONFIG _CONFIG2, _WRT_OFF & _PLLEN_ON & _STVREN_ON & _BORV_LO & _LVP_ON

    return

SetupIntOsc
  BANKSEL			OSCCON
  movlw				b'00000000'
  movwf				OSCCON				; Set Internal Oscillator frequency to 31khz

  return

SetupIO
  BANKSEL			TRISE
  movlw				b'00000000'
  movwf				TRISE					; Set pins RE0-RE3 as digital outputs

  BANKSEL			TRISA
  movlw				b'00000011'
  movwf				TRISA					; Set pin RA1 as digital input

  BANKSEL			ANSELA
  movlw				b'00000001'		; Configure RA0 as analog input, the rest are
  movwf				ANSELA				; digital I/O.

  return

SetupFOUTCtr
  BANKSEL			FOUT_CTR
  movlw				H'64'
  movwf				FOUT_CTR
  return

SetupFVRC

  BANKSEL			CM1CON0
  movlw				b'10010110'
  movwf				CM1CON0
				; Enable comparator, inverting, fast speed,
				; hysteresis enabled (45mV), asynchronous mode,
				; comparison level is 1.024V

  BANKSEL			CM1CON1
  movlw				b'00100000'
  movwf				CM1CON1
				; No interrupt generated by comparator, pos input
				; set to FVR, neg input set to C12IN0- (RA0)

  BANKSEL			FVRCON
  movlw				b'11000100'
  movwf				FVRCON

  btfss				FVRCON,FVRRDY		; Wait until the FVR can be used
  goto				$-1

  return

PullPSWLow
  BANKSEL			PORTA
  bsf					PORTA,2
  nop
  return

RELEASE_PSW
  BANKSEL			PORTA
  bcf					PORTA,2
  nop
  return