AM1805_FXNS         CODE

AM1805_WRITE_ADDRESS    equ	H'D2'
AM1805_READ_ADDRESS     equ	H'D3'

Delay50ms	       MACRO
  BANKSEL	       MSLEFT
  movlw	         .50
  movwf	         MSLEFT
  call           DELAYMS
  endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; TIME TO PROGRAM INTO AM1805 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#define	    PgmSec	.0		; xTTTOOOO  T = Tens place of
#define	    PgmMin	b'00100110'	; xTTTOOOO
#define	    PgmHr	b'00010001'	; xxTTOOOO
#define	    PgmDay	b'00100010'	; xxTTOOOO
#define	    PgmMo	b'00010010'	; xxxTOOOO
#define	    PgmYr	b'00010101'	; TTTTOOOO
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; ALARM TIME TO PROGRAM INTO AM1805 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#define	    PgmAlmSec	.0		; xTTTOOOO  T = Tens place of
#define	    PgmAlmMin	b'00100111'	; xTTTOOOO
#define	    PgmAlmHr	b'00010001'	; xxTTOOOO
#define	    PgmAlmDay	b'00100010'	; xxTTOOOO
#define	    PgmAlmMo	b'00010010'	; xxxTOOOO
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

I2C_SEND_AM1805_READ
  BANKSEL	    SSPBUF
  movlw	      AM1805_READ_ADDRESS	; Read '0xD3' into W
  movwf	      SSPBUF
  BANKSEL	    SSPSTAT
  btfsc	      SSPSTAT,R_NOT_W		; Wait until transmit is not in progress
  goto        $-1
  return			; If WDT doesn't timeout, transmission was successful

I2C_SEND_AM1805_WRITE
  BANKSEL	    SSPBUF
  movlw	      AM1805_WRITE_ADDRESS	; Read '0xD2' into W
  movwf	      SSPBUF
  BANKSEL	    SSPSTAT
  btfsc	      SSPSTAT,R_NOT_W		; Wait until transmit is not in progress
  goto        $-1
  return		  ; If WDT doesn't timeout, transmission was successful

I2C_AM1805_WRITE_DATA_TO_ADDRESS
  call        I2C_SEND_START
  call        I2C_SEND_AM1805_WRITE
  call        I2C_WAIT_FOR_ACK
  call        I2C_WRITE_NEXT_ADDRESS
  call        I2C_WAIT_FOR_ACK
  call        I2C_WRITE_NEXT_DATA
  call        I2C_WAIT_FOR_ACK
  call        I2C_SEND_STOP
  return

I2C_AM1805_WRITE_DEFAULTS
  BANKSEL	    NEXT_I2C_ADDRESS
  movlw	      H'11'			; Control2 register in AM1805
  movwf	      NEXT_I2C_ADDRESS
  movlw	      b'00011001'		; Disable nEXTR pin to generate nRST,
				; Set out2s to SLEEP, set FOUT to be driven
				; by the OUT bit.
  BANKSEL	    NEXT_I2C_DATA
  movwf	      NEXT_I2C_DATA
  call        I2C_AM1805_WRITE_DATA_TO_ADDRESS  ; Writes 0x18 to 0x11 @ AM1805

  BANKSEL	    NEXT_I2C_ADDRESS
  movlw	      H'30'			; Output Control register in AM1805
  movwf	      NEXT_I2C_ADDRESS
  movlw	      b'01100000'		; EXTI enabled when in VBAT mode, EXTI enabled
				; when in Sleep mode.
  BANKSEL	    NEXT_I2C_DATA
  movwf	      NEXT_I2C_DATA
  call        I2C_AM1805_WRITE_DATA_TO_ADDRESS  ; Writes 0x2D to 0x30 @ AM1805

  BANKSEL	NEXT_I2C_ADDRESS
  movlw	H'10'			; Oscillator Control register in AM1805
  movwf	NEXT_I2C_ADDRESS
  movlw	b'00000101'		; PSW pin is true open-drain output, and OUT bit
				; which is driving FOUT directly, is set to zero

  BANKSEL	NEXT_I2C_DATA
  movwf	NEXT_I2C_DATA

  call	I2C_AM1805_WRITE_DATA_TO_ADDRESS    ; Writes 0x04 to 0x10 @ AM1805

  BANKSEL	NEXT_I2C_ADDRESS
  movlw	H'1C'			; Oscillator Control register in AM1805
  movwf	NEXT_I2C_ADDRESS
  movlw	b'00000000'
  BANKSEL	NEXT_I2C_DATA
  movwf	NEXT_I2C_DATA

  call	I2C_AM1805_WRITE_DATA_TO_ADDRESS    ; Writes 0x00 to 0x1C @ AM1805

  nop

  return

AM1805_START_CTDN_TMR
  BANKSEL	NEXT_I2C_ADDRESS
  movlw	H'19'		 ; Countdown timer duration
  movwf	NEXT_I2C_ADDRESS
  BANKSEL	NEXT_CTDWN_VAL
  movf	NEXT_CTDWN_VAL,w
  BANKSEL	NEXT_I2C_DATA
  movwf	NEXT_I2C_DATA
  call	I2C_AM1805_WRITE_DATA_TO_ADDRESS  ; Writes NEXT_CTDWN_VAL to 0x19 @ AM1805

  BANKSEL	NEXT_I2C_ADDRESS
  movlw	H'18'			; Oscillator Control register in AM1805
  movwf	NEXT_I2C_ADDRESS
  movlw	b'11010110'		; Countdown Timer has Maintained interrupt, 1/60hz counter
				;
  BANKSEL	NEXT_I2C_DATA
  movwf	NEXT_I2C_DATA
  call	I2C_AM1805_WRITE_DATA_TO_ADDRESS    ; Writes 0xC3 to 0x18 @ AM1805

  BANKSEL	MS100LEFT
  movlw	H'14'
  movwf	MS100LEFT
  call	DELAY100MS

  BANKSEL	NEXT_I2C_ADDRESS
  movlw	H'18'			; Oscillator Control register in AM1805
  movwf	NEXT_I2C_ADDRESS
  movlw	b'11010111'		; Countdown Timer has Maintained interrupt, 1/60hz counter
				;
  BANKSEL	NEXT_I2C_DATA
  movwf	NEXT_I2C_DATA
  call	I2C_AM1805_WRITE_DATA_TO_ADDRESS    ; Writes 0xC3 to 0x18 @ AM1805

  return

AM1805_INITIATE_SLEEP
  BANKSEL	NEXT_I2C_ADDRESS
  movlw	H'12'			; Interrupt Mask register in AM1805
  movwf	NEXT_I2C_ADDRESS
  movlw	b'01101100'		; Enable timer, alarm and exti interrupts
  BANKSEL	NEXT_I2C_DATA
  movwf	NEXT_I2C_DATA
  call	I2C_AM1805_WRITE_DATA_TO_ADDRESS    ; Writes 0x2D to 0x18 @ AM1805

  BANKSEL	NEXT_I2C_ADDRESS
  movlw	H'0F'			;
  movwf	NEXT_I2C_ADDRESS
  movlw	b'00000000'		; Clear all interrupts
  BANKSEL	NEXT_I2C_DATA
  movwf	NEXT_I2C_DATA
  call	I2C_AM1805_WRITE_DATA_TO_ADDRESS    ; Writes 0x00 to 0x0F @ AM1805

  ;BANKSEL	NEXT_I2C_ADDRESS
  ;movlw	H'18'			; Oscillator Control register in AM1805
  ;movwf	NEXT_I2C_ADDRESS
  ;movlw	b'11000010'		; Countdown Timer has Maintained interrupt, 1/60hz counter
  ;BANKSEL	NEXT_I2C_DATA
  ;movwf	NEXT_I2C_DATA

  ;call	I2C_AM1805_WRITE_DATA_TO_ADDRESS    ; Writes 0xC3 to 0x18 @ AM1805

  ;BANKSEL	MS100LEFT			;
  ;movlw	H'13'				;
  ;movwf	MS100LEFT			; Put 255 (FF) into MSLEFT and then
  ;call	DELAY100MS

  BANKSEL	NEXT_I2C_ADDRESS
  movlw	H'17'			; Sleep Control register in AM1805
  movwf	NEXT_I2C_ADDRESS
  movlw	b'10000000'		; Enter Sleep mode, rising edge of EXTI triggers interrupt
  BANKSEL	NEXT_I2C_DATA
  movwf	NEXT_I2C_DATA
  call	I2C_AM1805_WRITE_DATA_TO_ADDRESS    ; Writes 0x80 to 0x17 @ AM1805

  nop

;AM1805_POLL_SLEEP:
;	    call	I2C_SEND_START
;	    call	I2C_SEND_AM1805_WRITE
;	    call	I2C_WAIT_FOR_ACK
;
;	    movlw	H'17'
;	    BANKSEL	NEXT_I2C_ADDRESS
;	    movwf	NEXT_I2C_ADDRESS
;
;	    call	I2C_WRITE_NEXT_ADDRESS
;	    call	I2C_WAIT_FOR_ACK
;	    call	I2C_SEND_STOP
;
;	    nop
;
;	    call	I2C_SEND_START
;	    call	I2C_SEND_AM1805_READ
;	    call	I2C_WAIT_FOR_ACK
;	    call	I2C_GEN_READ
;	    call	I2C_GET_READ_DATA
;
;	    call	I2C_SEND_STOP
;
;	    BANKSEL	LATEST_I2C_BYTE
;	    movf	LATEST_I2C_BYTE,w
;
;	    BTFSS	LATEST_I2C_BYTE,7
;	    goto	AM1805_INITIATE_SLEEP
;
  return

AM1805_PRINT_REGISTER
  call	I2C_SEND_START
  call	I2C_SEND_AM1805_WRITE
  call	I2C_WAIT_FOR_ACK

  BANKSEL	PRINT_REGISTER
  movf	PRINT_REGISTER,w
  BANKSEL	NEXT_I2C_ADDRESS
  movwf	NEXT_I2C_ADDRESS

  call	I2C_WRITE_NEXT_ADDRESS
  call	I2C_WAIT_FOR_ACK
  call	I2C_SEND_STOP

  nop

  call	I2C_SEND_START
  call	I2C_SEND_AM1805_READ
  call	I2C_WAIT_FOR_ACK
  call	I2C_GEN_READ
  call	I2C_GET_READ_DATA

  call	I2C_SEND_STOP

  BANKSEL	LATEST_I2C_BYTE
  movf	LATEST_I2C_BYTE,w

  BANKSEL	NEXT_UART_CHAR_BEFORE_ADJ
  movwf	NEXT_UART_CHAR_BEFORE_ADJ

  call	EUSART_TRANSMIT_CHAR_ADJ

  return

AM1805_UPDATE_CT			    ; must setup NEXT_AM1805_ADDRESS with address to read from

  BANKSEL	NEXT_I2C_ADDRESS
  movlw	H'01'
  movwf	NEXT_I2C_ADDRESS

  call	I2C_SEND_START
  call	I2C_SEND_AM1805_WRITE
  call	I2C_WAIT_FOR_ACK
  call	I2C_WRITE_NEXT_ADDRESS
  call	I2C_WAIT_FOR_ACK
  call	I2C_SEND_STOP	    ; send a read request for the addres 0x01 on the am1805

  call	I2C_SEND_START
  call	I2C_SEND_AM1805_READ
  call	I2C_WAIT_FOR_ACK

  call	I2C_GEN_READ
  BANKSEL	SSPBUF
  movf	SSPBUF,w
  BANKSEL	CT_SEC
  movwf	CT_SEC   ; receive the read data (from 0x01@am1805) and place it in CT_SEC
  call	I2C_SEND_ACK

  call	I2C_GEN_READ
  BANKSEL	SSPBUF
  movf	SSPBUF,w
  BANKSEL	CT_MIN
  movwf	CT_MIN
  call	I2C_SEND_ACK

  call	I2C_GEN_READ
  BANKSEL	SSPBUF
  movf	SSPBUF,w
  BANKSEL	CT_HR
  movwf	CT_HR
  call	I2C_SEND_ACK

  call	I2C_GEN_READ
  BANKSEL	SSPBUF
  movf	SSPBUF,w
  BANKSEL	CT_DAY
  movwf	CT_DAY
  call	I2C_SEND_ACK

  call	I2C_GEN_READ
  BANKSEL	SSPBUF
  movf	SSPBUF,w
  BANKSEL	CT_MO
  movwf	CT_MO
  call	I2C_SEND_ACK

  call	I2C_GEN_READ
  BANKSEL	SSPBUF
  movf	SSPBUF,w
  BANKSEL	CT_YR
  movwf	CT_YR
  call	I2C_SEND_NACK

  call	I2C_SEND_STOP

;;;;;;;;;;;;;;;;;;;;;;^ Populate CT_xyz with actual AM1805 vals, below: format them;;;;;;;;;;;;;;;;;

  BANKSEL	NEXT_MASK
  movlw	b'01110000'
  movwf	NEXT_MASK
  BANKSEL	CT_SEC
  movf	CT_SEC,w
  BANKSEL	CT_WORK
  movwf	CT_WORK
  call	AM1805_GET_TIMEBW_FROM_WORK   ; Format LATEST_I2C_BYTE into 8 bits, and place it in W
  BANKSEL	CT_WORK
  movf	CT_WORK,w
  BANKSEL	CT_SEC
  movwf	CT_SEC		    ; Read W into CT_SEC

  BANKSEL	NEXT_MASK
  movlw	b'01110000'
  movwf	NEXT_MASK
  BANKSEL	CT_MIN
  movf	CT_MIN,w
  BANKSEL	CT_WORK
  movwf	CT_WORK
  call	AM1805_GET_TIMEBW_FROM_WORK   ; Format LATEST_I2C_BYTE into 8 bits, and place it in CT_SEC
  BANKSEL	CT_WORK
  movf	CT_WORK,w
  BANKSEL	CT_MIN
  movwf	CT_MIN		    ; Read W into CT_MIN

  BANKSEL	NEXT_MASK
  movlw	b'00110000'
  movwf	NEXT_MASK
  BANKSEL	CT_HR
  movf	CT_HR,w
  BANKSEL	CT_WORK
  movwf	CT_WORK
  call	AM1805_GET_TIMEBW_FROM_WORK   ; Format LATEST_I2C_BYTE into 8 bits, and place it in CT_SEC
  BANKSEL	CT_WORK
  movf	CT_WORK,w
  BANKSEL	CT_HR
  movwf	CT_HR		    ; Read W into CT_MIN

  BANKSEL	NEXT_MASK
  movlw	b'00110000'
  movwf	NEXT_MASK
  BANKSEL	CT_DAY
  movf	CT_DAY,w
  BANKSEL	CT_WORK
  movwf	CT_WORK
  call	AM1805_GET_TIMEBW_FROM_WORK   ; Format LATEST_I2C_BYTE into 8 bits, and place it in CT_SEC
  BANKSEL	CT_WORK
  movf	CT_WORK,w
  BANKSEL	CT_DAY
  movwf	CT_DAY		    ; Read W into CT_MIN

  BANKSEL	NEXT_MASK
  movlw	b'00010000'
  movwf	NEXT_MASK
  BANKSEL	CT_MO
  movf	CT_MO,w
  BANKSEL	CT_WORK
  movwf	CT_WORK
  call	AM1805_GET_TIMEBW_FROM_WORK   ; Format LATEST_I2C_BYTE into 8 bits, and place it in CT_SEC
  BANKSEL	CT_WORK
  movf	CT_WORK,w
  BANKSEL	CT_MO
  movwf	CT_MO		    ; Read W into CT_MIN

  BANKSEL	NEXT_MASK
  movlw	b'11110000'
  movwf	NEXT_MASK
  BANKSEL	CT_YR
  movf	CT_YR,w
  BANKSEL	CT_WORK
  movwf	CT_WORK
  call	AM1805_GET_TIMEBW_FROM_WORK   ; Format LATEST_I2C_BYTE into 8 bits, and place it in CT_SEC
  BANKSEL	CT_WORK
  movf	CT_WORK,w
  BANKSEL	CT_YR
  movwf	CT_YR		    ; Read W into CT_MIN

  call	I2C_SEND_STOP

  return

AM1805_PRINT_CT
  BANKSEL	CT_HR
  movf	CT_HR,w
  BANKSEL	NEXT_UART_CHAR_BEFORE_ADJ
  movwf	NEXT_UART_CHAR_BEFORE_ADJ
  call	EUSART_TRANSMIT_CHAR_ADJ	; Print the hour

  Delay50ms

  movlw	H'3A'
  BANKSEL	NEXT_UART_CHAR
  movwf	NEXT_UART_CHAR
  call	EUSART_TRANSMIT			; Print a ':'

  Delay50ms

  BANKSEL	CT_MIN
  movf	CT_MIN,w
  BANKSEL	NEXT_UART_CHAR_BEFORE_ADJ
  movwf	NEXT_UART_CHAR_BEFORE_ADJ
  call	EUSART_TRANSMIT_CHAR_ADJ	; Print the minute

  Delay50ms

  movlw	H'3A'
  BANKSEL	NEXT_UART_CHAR
  movwf	NEXT_UART_CHAR
  call	EUSART_TRANSMIT			; Print a ':'

  Delay50ms

  BANKSEL	CT_SEC
  movf	CT_SEC,w
  BANKSEL	NEXT_UART_CHAR_BEFORE_ADJ
  movwf	NEXT_UART_CHAR_BEFORE_ADJ
  call	EUSART_TRANSMIT_CHAR_ADJ	; Print the second

  Delay50ms

  movlw	H'20'
  BANKSEL	NEXT_UART_CHAR
  movwf	NEXT_UART_CHAR
  call	EUSART_TRANSMIT			; Print a ' '

  Delay50ms

  BANKSEL	CT_MO
  movf	CT_MO,w
  BANKSEL	NEXT_UART_CHAR_BEFORE_ADJ
  movwf	NEXT_UART_CHAR_BEFORE_ADJ
  call	EUSART_TRANSMIT_CHAR_ADJ	; Print the month

  Delay50ms

  movlw	H'2D'
  BANKSEL	NEXT_UART_CHAR
  movwf	NEXT_UART_CHAR
  call	EUSART_TRANSMIT			; Print a '-'

  Delay50ms

  BANKSEL	CT_DAY
  movf	CT_DAY,w
  BANKSEL	NEXT_UART_CHAR_BEFORE_ADJ
  movwf	NEXT_UART_CHAR_BEFORE_ADJ
  call	EUSART_TRANSMIT_CHAR_ADJ	; Print the day

  Delay50ms

  movlw	H'2D'
  BANKSEL	NEXT_UART_CHAR
  movwf	NEXT_UART_CHAR
  call	EUSART_TRANSMIT			; Print a '-'

  Delay50ms

  BANKSEL	CT_YR
  movf	CT_YR,w
  BANKSEL	NEXT_UART_CHAR_BEFORE_ADJ
  movwf	NEXT_UART_CHAR_BEFORE_ADJ
  call	EUSART_TRANSMIT_CHAR_ADJ	; Print the year

  return

AM1805_GET_SECONDS
  BANKSEL	    LATEST_I2C_BYTE
  movf	    LATEST_I2C_BYTE,w		; am1805 seconds are now in W in the form xTTTOOOO (x = na,T = tens places, O=ones place)
  BANKSEL	    CT_WORK
  movwf	    CT_WORK
  BANKSEL	    CT_WORK_TENS
  movwf	    CT_WORK_TENS	; the seconds value is now in CT_WORK and CT_WORK_TENS
  movlw	    b'00001111'
  BANKSEL	    CT_WORK
  andwf	    CT_WORK,f
  BANKSEL	    CT_WORK_TENS
  movlw	    b'01110000'
  andwf	    CT_WORK_TENS,f

  lsrf	    CT_WORK_TENS,f
  lsrf	    CT_WORK_TENS,f
  lsrf	    CT_WORK_TENS,f
  lsrf	    CT_WORK_TENS,f	; CT_WORK now contains the ones value, CT_WORK_TENS now contains the tens value of the seconds

  movlw	    H'0A'
  BANKSEL	    MATH_X
  movwf	    MATH_X
  BANKSEL	    CT_WORK_TENS
  movf	    CT_WORK_TENS,w
  BANKSEL	    MATH_Y
  movwf	    MATH_Y
  call	    MULV8
  BANKSEL	    MATH_RESULT
  movf	    MATH_RESULT,w
  BANKSEL	    CT_WORK
  addwf	    CT_WORK,w		; multiply CT_WORK_TENS by 10 and add CT_WORK to it
  BANKSEL	    CT_SEC
  movwf	    CT_SEC		; Current seconds of the AM1805 are now in CT_SEC

  return

AM1805_GET_TIMEBW_FROM_WORK
  BANKSEL	    CT_WORK
  movf	    CT_WORK,w		; am1805 seconds are now in W in the form xTTTOOOO (x = na,T = tens places, O=ones place)
  BANKSEL	    CT_WORK
  movwf	    CT_WORK
  BANKSEL	    CT_WORK_TENS
  movwf	    CT_WORK_TENS	; the seconds value is now in CT_WORK and CT_WORK_TENS
  movlw	    b'00001111'
  BANKSEL	    CT_WORK
  andwf	    CT_WORK,f
  BANKSEL	    NEXT_MASK
  movf	    NEXT_MASK,w		; Since the time/date bytes in the am1805 have similar
  BANKSEL	    CT_WORK_TENS	; masks for their values, we can reuse this function and
  ;movlw	    b'01110000'		; only change the NEXT_MASK value depending on how the tens
  andwf	    CT_WORK_TENS,f	; place is organized in

  lsrf	    CT_WORK_TENS,f
  lsrf	    CT_WORK_TENS,f
  lsrf	    CT_WORK_TENS,f
  lsrf	    CT_WORK_TENS,f	; CT_WORK now contains the ones value, CT_WORK_TENS now contains the tens value of the seconds

  movlw	    H'0A'
  BANKSEL	    MATH_X
  movwf	    MATH_X
  BANKSEL	    CT_WORK_TENS
  movf	    CT_WORK_TENS,w
  BANKSEL	    MATH_Y
  movwf	    MATH_Y
  call	    MULV8
  BANKSEL	    MATH_RESULT
  movf	    MATH_RESULT,w
  BANKSEL	    CT_WORK
  addwf	    CT_WORK,f		; multiply CT_WORK_TENS by 10 and add CT_WORK to it
  ;BANKSEL	    CT_SEC
  ;movwf	    CT_SEC		; Current seconds of the AM1805 are now in W
  ;RETURN
  return

AM1805_GET_HOURS
  BANKSEL	    LATEST_I2C_BYTE
  movf	    LATEST_I2C_BYTE,w		; am1805 seconds are now in W in the form xTTTOOOO (x = na,T = tens places, O=ones place)
  BANKSEL	    CT_WORK
  movwf	    CT_WORK
  BANKSEL	    CT_WORK_TENS
  movwf	    CT_WORK_TENS	; the seconds value is now in CT_WORK and CT_WORK_TENS
  movlw	    b'00001111'
  BANKSEL	    CT_WORK
  andwf	    CT_WORK,f
  BANKSEL	    CT_WORK_TENS
  movlw	    b'00110000'
  andwf	    CT_WORK_TENS,f

  lsrf	    CT_WORK_TENS,f
  lsrf	    CT_WORK_TENS,f
  lsrf	    CT_WORK_TENS,f
  lsrf	    CT_WORK_TENS,f	; CT_WORK now contains the ones value, CT_WORK_TENS now contains the tens value of the seconds

  movlw	    H'0A'
  BANKSEL	    MATH_X
  movwf	    MATH_X
  BANKSEL	    CT_WORK_TENS
  movf	    CT_WORK_TENS,w
  BANKSEL	    MATH_Y
  movwf	    MATH_Y
  call	    MULV8
  BANKSEL	    MATH_RESULT
  movf	    MATH_RESULT,w
  BANKSEL	    CT_WORK
  addwf	    CT_WORK,w		; multiply CT_WORK_TENS by 10 and add CT_WORK to it
  BANKSEL	    CT_HR
  movwf	    CT_HR		; Current seconds of the AM1805 are now in CT_SEC

  return

AM1805_WRITE_PGM_TIME
  call	I2C_SEND_START
  call	I2C_SEND_AM1805_WRITE
  call	I2C_WAIT_FOR_ACK

  BANKSEL	NEXT_I2C_ADDRESS
  movlw	.1
  movwf	NEXT_I2C_ADDRESS
  call	I2C_WRITE_NEXT_ADDRESS
  call	I2C_WAIT_FOR_ACK

  BANKSEL	NEXT_I2C_DATA
  movlw	PgmSec
  movwf	NEXT_I2C_DATA
  call	I2C_WRITE_NEXT_DATA
  call	I2C_WAIT_FOR_ACK

  BANKSEL	NEXT_I2C_DATA
  movlw	PgmMin
  movwf	NEXT_I2C_DATA
  call	I2C_WRITE_NEXT_DATA
  call	I2C_WAIT_FOR_ACK

  BANKSEL	NEXT_I2C_DATA
  movlw	PgmHr
  movwf	NEXT_I2C_DATA
  call	I2C_WRITE_NEXT_DATA
  call	I2C_WAIT_FOR_ACK

  BANKSEL	NEXT_I2C_DATA
  movlw	PgmDay
  movwf	NEXT_I2C_DATA
  call	I2C_WRITE_NEXT_DATA
  call	I2C_WAIT_FOR_ACK

  BANKSEL	NEXT_I2C_DATA
  movlw	PgmMo
  movwf	NEXT_I2C_DATA
  call	I2C_WRITE_NEXT_DATA
  call	I2C_WAIT_FOR_ACK

  BANKSEL	NEXT_I2C_DATA
  movlw	PgmYr
  movwf	NEXT_I2C_DATA
  call	I2C_WRITE_NEXT_DATA
  call	I2C_WAIT_FOR_ACK

  call	I2C_SEND_STOP

  return

AM1805_WRITE_ALARM_TIME
  call	I2C_SEND_START
  call	I2C_SEND_AM1805_WRITE
  call	I2C_WAIT_FOR_ACK

  BANKSEL	NEXT_I2C_ADDRESS
  movlw	.9
  movwf	NEXT_I2C_ADDRESS
  call	I2C_WRITE_NEXT_ADDRESS
  call	I2C_WAIT_FOR_ACK

  BANKSEL	NEXT_I2C_DATA
  movlw	PgmAlmSec
  movwf	NEXT_I2C_DATA
  call	I2C_WRITE_NEXT_DATA
  call	I2C_WAIT_FOR_ACK

  BANKSEL	NEXT_I2C_DATA
  movlw	PgmAlmMin
  movwf	NEXT_I2C_DATA
  call	I2C_WRITE_NEXT_DATA
  call	I2C_WAIT_FOR_ACK

  BANKSEL	NEXT_I2C_DATA
  movlw	PgmAlmHr
  movwf	NEXT_I2C_DATA
  call	I2C_WRITE_NEXT_DATA
  call	I2C_WAIT_FOR_ACK

  BANKSEL	NEXT_I2C_DATA
  movlw	PgmAlmDay
  movwf	NEXT_I2C_DATA
  call	I2C_WRITE_NEXT_DATA
  call	I2C_WAIT_FOR_ACK

  BANKSEL	NEXT_I2C_DATA
  movlw	PgmAlmMo
  movwf	NEXT_I2C_DATA
  call	I2C_WRITE_NEXT_DATA
  call	I2C_WAIT_FOR_ACK

  call	I2C_SEND_STOP

  return

I2C_AM1805_UPDATE_CTDWN_VAL

  BANKSEL	SLEEP_INTERVAL_DURATION
  movf	SLEEP_INTERVAL_DURATION,w
  BANKSEL	NEXT_CTDWN_VAL
  movwf	NEXT_CTDWN_VAL

  call	AM1805_START_CTDN_TMR

  return

I2C_AM1805_UPDATE_CTDWN_VAL_FIN

  BANKSEL	SLEEP_INTERVAL_FINAL
  movf	SLEEP_INTERVAL_FINAL,w
  BANKSEL	NEXT_CTDWN_VAL
  movwf	NEXT_CTDWN_VAL

  call	AM1805_START_CTDN_TMR

  return
