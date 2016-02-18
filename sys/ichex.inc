ICHEX_FXNS          CODE

IS_THIS_FIRST_PWRUP

    ; This function returns a 0 in w if this powerup is NOT the first powerup
    ; and a 1 in w if it IS the first powerup.

    movlw               H'0F'
    BANKSEL             NEXT_I2C_ADDRESS
    movwf				NEXT_I2C_ADDRESS

    call				I2C_SEND_START
    call				I2C_SEND_EEPROM_WRITE
    call				I2C_WAIT_FOR_ACK
    call				I2C_WRITE_NEXT_ADDRESS
    call				I2C_WAIT_FOR_ACK

    call				I2C_SEND_START
    call				I2C_SEND_EEPROM_READ
    call				I2C_WAIT_FOR_ACK
    call				I2C_GEN_READ
    call				I2C_GET_READ_DATA
    call				I2C_SEND_NACK

    call				I2C_SEND_STOP

    BANKSEL			    LATEST_I2C_BYTE
    movlw               .1
    subwf               LATEST_I2C_BYTE,f
    tstf				LATEST_I2C_BYTE

    btfss				STATUS,Z
    retlw				.1
    retlw				.0
