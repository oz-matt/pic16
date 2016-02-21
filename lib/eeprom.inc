
EEPROM_FXNS   CODE

EEPROM_WRITE_ADDRESS					equ	H'A0'
EEPROM_READ_ADDRESS						equ	H'A1'

EEPROM_ADDRESS_OF_SLEEP_VARS	equ	H'00'

I2C_SEND_EEPROM_READ
    BANKSEL             SSPBUF
    movlw               EEPROM_READ_ADDRESS	; Read '0xD3' into W
    movwf				SSPBUF
    BANKSEL             SSPSTAT
    btfsc				SSPSTAT,R_NOT_W		; Wait until transmit is not in progress
    goto				$-1
    return

I2C_SEND_EEPROM_WRITE
    BANKSEL			    SSPBUF
    movlw				EEPROM_WRITE_ADDRESS	; Read '0xD2' into W
    movwf				SSPBUF
    BANKSEL			    SSPSTAT
    btfsc				SSPSTAT,R_NOT_W		; Wait until transmit is not in progress
    goto				$-1
    return			; If WDT doesn't timeout, transmission was successful

I2C_EEPROM_WRITE_DATA_TO_ADDRESS
    call				I2C_SEND_START
    call				I2C_SEND_EEPROM_WRITE
    call				I2C_WAIT_FOR_ACK
    call				I2C_WRITE_NEXT_ADDRESS
    call				I2C_WAIT_FOR_ACK
    call				I2C_WRITE_NEXT_DATA
    call				I2C_WAIT_FOR_ACK
    call				I2C_SEND_STOP
    return

EEPROM_WRITE_DEFAULTS
        ;Write 0 to 0x00 -- num sleep intervals remaining = 0
        ;Write 255 to 0x01 -- set 'standard' deep sleep interval
        ;Write 0 to 0x02 -- length of final deep sleep interval = 0
    BANKSEL             NEXT_I2C_ADDRESS
    movlw               .0
    movwf               NEXT_I2C_ADDRESS
    BANKSEL             NEXT_I2C_DATA
    movlw               .0
    movwf               NEXT_I2C_DATA

    call				I2C_SEND_START
    call				I2C_SEND_EEPROM_WRITE
    call				I2C_WAIT_FOR_ACK
    call				I2C_WRITE_NEXT_ADDRESS
    call				I2C_WAIT_FOR_ACK
    call				I2C_WRITE_NEXT_DATA         ; 0x00
    call				I2C_WAIT_FOR_ACK

    BANKSEL             NEXT_I2C_DATA
    movlw               H'FF'
    movwf               NEXT_I2C_DATA
    call				I2C_WRITE_NEXT_DATA         ; 0x01
    call				I2C_WAIT_FOR_ACK

    BANKSEL             NEXT_I2C_DATA
    movlw               .0
    movwf               NEXT_I2C_DATA
    call				I2C_WRITE_NEXT_DATA         ; 0x02
    call				I2C_WAIT_FOR_ACK
    call				I2C_SEND_STOP

    ; ------------------------------------------------------------
    ; Init values relating to GMT start mode to zero (0x40 - 0x46)

    BANKSEL             NEXT_I2C_ADDRESS
    movlw               H'40'
    movwf               NEXT_I2C_ADDRESS
    BANKSEL             NEXT_I2C_DATA
    movlw               .0
    movwf               NEXT_I2C_DATA

    call				I2C_SEND_START
    call				I2C_SEND_EEPROM_WRITE
    call				I2C_WAIT_FOR_ACK
    call				I2C_WRITE_NEXT_ADDRESS
    call				I2C_WAIT_FOR_ACK
    call				I2C_WRITE_NEXT_DATA         ; 0x40
    call				I2C_WAIT_FOR_ACK
    call				I2C_WRITE_NEXT_DATA         ; 0x41
    call				I2C_WAIT_FOR_ACK
    call				I2C_WRITE_NEXT_DATA         ; 0x42
    call				I2C_WAIT_FOR_ACK
    call				I2C_WRITE_NEXT_DATA         ; 0x43
    call				I2C_WAIT_FOR_ACK
    call				I2C_WRITE_NEXT_DATA         ; 0x44
    call				I2C_WAIT_FOR_ACK
    call				I2C_WRITE_NEXT_DATA         ; 0x45
    call				I2C_WAIT_FOR_ACK
    call				I2C_WRITE_NEXT_DATA         ; 0x46
    call				I2C_WAIT_FOR_ACK
    call				I2C_SEND_STOP

    ; ------------------------------------------------------------
    ; Write 1 to 0x0F -- Every subsequent powerup is not the first powerup

    BANKSEL             NEXT_I2C_ADDRESS
    movlw               H'0F'
    movwf               NEXT_I2C_ADDRESS
    BANKSEL             NEXT_I2C_DATA
    movlw               .1
    movwf               NEXT_I2C_DATA
    call                I2C_EEPROM_WRITE_DATA_TO_ADDRESS

    return

I2C_EEPROM_READ_SLEEP_VARS
    call				I2C_SEND_START
    call				I2C_SEND_EEPROM_WRITE
    call				I2C_WAIT_FOR_ACK

    movlw				H'00'
    BANKSEL			    NEXT_I2C_ADDRESS
    movwf				NEXT_I2C_ADDRESS

    call				I2C_WRITE_NEXT_ADDRESS
    call				I2C_WAIT_FOR_ACK

    call				I2C_SEND_START
    call				I2C_SEND_EEPROM_READ
    call				I2C_WAIT_FOR_ACK
    call				I2C_GEN_READ
    call				I2C_GET_READ_DATA
    call				I2C_SEND_ACK

    BANKSEL			    LATEST_I2C_BYTE
    movf				LATEST_I2C_BYTE,w
    BANKSEL			    SLEEP_INTERVALS_LEFT
    movwf				SLEEP_INTERVALS_LEFT

    call				I2C_GEN_READ
    call				I2C_GET_READ_DATA
    call				I2C_SEND_ACK

    BANKSEL			    LATEST_I2C_BYTE
    movf				LATEST_I2C_BYTE,w
    BANKSEL			    SLEEP_INTERVAL_DURATION
    movwf				SLEEP_INTERVAL_DURATION

    call				I2C_GEN_READ
    call				I2C_GET_READ_DATA
    call				I2C_SEND_NACK

    BANKSEL			    LATEST_I2C_BYTE
    movf				LATEST_I2C_BYTE,w
    BANKSEL			    SLEEP_INTERVAL_FINAL
    movwf				SLEEP_INTERVAL_FINAL

    call				I2C_SEND_STOP
    return

I2C_EEPROM_DEC_NUM_INTERVALS ; Sleep vars must be updated first
    movlw				.1
    BANKSEL			    SLEEP_INTERVALS_LEFT
    subwf				SLEEP_INTERVALS_LEFT,w
    BANKSEL			    NEXT_I2C_DATA
    movwf				NEXT_I2C_DATA

    BANKSEL	NEXT_I2C_ADDRESS
    movlw				H'00'
    movwf				NEXT_I2C_ADDRESS

    call				I2C_EEPROM_WRITE_DATA_TO_ADDRESS
    return
