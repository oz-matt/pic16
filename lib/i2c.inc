
I2C_FXNS    CODE

I2CINIT
	movlw				H'00'
	BANKSEL				SSPADD
	movwf				SSPADD	    ; configure I2C SCL for 31kHz/4 = 7.75kHz

	movlw				b'00101000'
	BANKSEL				SSPCON1
	movwf				SSPCON1	    ; configure for Master I2C

	movlw				H'00'
	BANKSEL				SSPSTAT
	movwf				SSPSTAT	    ; 'enable slew rate'

	movlw				b'00011000'
	BANKSEL				TRISC
	iorwf				TRISC,f	    ; configure SDA and SCL as outputs (RC3 and RC4)
	return

I2C_SEND_START
	BANKSEL				SSPCON2
	bsf					SSPCON2,SEN ; Initiate I2C Start

	btfsc				SSPCON2,SEN
	goto				$-1
	return

I2C_SEND_STOP
	BANKSEL				SSPCON2
	bsf					SSPCON2,PEN
	btfsc				SSPCON2,PEN
	goto				$-1
	return

I2C_TEST_WRITE
	BANKSEL				SSPBUF
	movlw				H'7A'
	movwf				SSPBUF
	BANKSEL				SSPSTAT
	btfsc				SSPSTAT,R_NOT_W		; Wait until transmit is not in progress
	goto				$-1
	return

I2C_WAIT_FOR_ACK
	btfsc				SSPCON2,ACKSTAT
	goto				$-1
	return

I2C_GEN_READ
	BANKSEL				SSPCON2
	bsf					SSPCON2,RCEN
	btfsc				SSPCON2,RCEN
	goto				$-1
	return

I2C_GET_READ_DATA
	BANKSEL	    		SSPBUF
	movf	    		SSPBUF,w
	BANKSEL	    		LATEST_I2C_BYTE
	movwf	    		LATEST_I2C_BYTE
	return


I2C_TEST_GET_READ_DATA
  BANKSEL	    		SSPBUF
  movf	    			SSPBUF,w
  BANKSEL	    		TXREG
  movwf	    			TXREG	    ; Transmit the read byte over EUSART
  BANKSEL	    		PIR1
  btfsc	    			PIR1,TXIF
  goto	    			$-1
  return

I2C_WRITE_NEXT_DATA
	BANKSEL				NEXT_I2C_DATA
	movf				NEXT_I2C_DATA,w
	BANKSEL				SSPBUF
	movwf				SSPBUF
	BANKSEL				SSPSTAT
	btfsc				SSPSTAT,R_NOT_W		; Wait until transmit is not in progress
	goto				$-1
	return

I2C_WRITE_NEXT_ADDRESS
  BANKSEL				NEXT_I2C_ADDRESS
  movf					NEXT_I2C_ADDRESS,w
  BANKSEL				SSPBUF
  movwf					SSPBUF
  BANKSEL				SSPSTAT
  btfsc					SSPSTAT,R_NOT_W		; Wait until transmit is not in progress
  goto					$-1
  return

I2C_SEND_ACK
  BANKSEL				SSPCON2
  bcf					SSPCON2, ACKDT
  nop
  nop
  bsf					SSPCON2, ACKEN
  return

I2C_SEND_NACK
  BANKSEL				SSPCON2
  bsf					SSPCON2, ACKDT
  nop
  nop
  bsf					SSPCON2, ACKEN
  return
