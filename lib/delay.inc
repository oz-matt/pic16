
DELAY_FXNS    CODE

DELAYMS
  BANKSEL	    MSTMR
NUMMS
  movlw	      H'02'
  movwf       MSTMR
NEXTMS
  decfsz      MSTMR,1
  goto        NEXTMS
  decfsz      MSLEFT,1
  goto        NUMMS
  return

DELAY100MS
NEXTHUN
  BANKSEL	    MSLEFT
  movlw	      H'51'
  movwf	      MSLEFT
  call        DELAYMS
  decfsz      MS100LEFT,1
  goto        NEXTHUN
  return

DOUBLEBLINKLED
  BANKSEL	    PORTE
  movlw	      b'00000010'
  movwf	      PORTE			      ; Set pin RE1 low

  BANKSEL	    MS100LEFT
  movlw	      H'01'
  movwf	      MS100LEFT			  ; Put 255 (FF) into MSLEFT and then
  call	      DELAY100MS			; call DELAYMS. This will block for 255ms.

  BANKSEL	    PORTE
  movlw	      b'00000000'
  movwf	      PORTE			      ; Set pin RE1 high

  BANKSEL	    MS100LEFT
  movlw	      H'01'
  movwf	      MS100LEFT			  ; Put 255 (FF) into MSLEFT and then
  call        DELAY100MS			; call DELAYMS. This will block for 255ms.
SINGLEBLINKLED
  BANKSEL	    PORTE
  movlw	      b'00000010'
  movwf	      PORTE			      ; Set pin RE1 low

  BANKSEL	    MS100LEFT
  movlw	      H'01'
  movwf	      MS100LEFT			  ; Put 255 (FF) into MSLEFT and then
  call        DELAY100MS			; call DELAYMS. This will block for 255ms.

  BANKSEL	    PORTE
  movlw	      b'00000000'
  movwf	      PORTE			      ; Set pin RE1 high

  return
