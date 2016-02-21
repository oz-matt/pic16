BREX_FXNS     CODE

FIRST_PWRUP_CODE

    call                AM1805_WRITE_PGM_TIME
    call                I2C_AM1805_WRITE_DEFAULTS
    call                EEPROM_WRITE_DEFAULTS

    return

USB_PLUGGED_AT_START

    ; TODO: Create 'USBLOOP' function -- a formal function
    ; that waits for an interrupt

    goto                $       ; Loop forever for now

    return

TC_HIGH

    ; TODO: Go through TC-HIGH branch, read sleep vars, etc.
    ; for now, just loop forever

    goto                $

    return

ALLOW_UPDATE_SD_TO_EEPROM

    ; This function waits for the NRF52 to update the EEPROM with
    ; all of the SD card parameters. This function returns to Main
    ; after the EEPROM has been updated.

    call                PULL_EEPROM_MASTER_SEL_HIGH

    call                ENABLE_NRF52

                        DELAY_1_SEC

    call                PULL_EEPROM_MASTER_SEL_LOW

    btfss               PORTA,1
    goto                $-1

    call                DISABLE_NRF52

    return

SLEEP_ERROR

    ; An EXTI interrupt was missed by the am1805.
    ; We need to take the am1805 out of sleep.

    ; TODO - Write this fxn.

    return

RESET_INSTRUCTION_PTR

    BANKSEL             NEXT_I2C_ADDRESS
    movlw               H'4F'
    movwf               NEXT_I2C_ADDRESS
    BANKSEL             NEXT_I2C_DATA
    movlw               H'50'
    movwf               NEXT_I2C_DATA
    call                I2C_EEPROM_WRITE_DATA_TO_ADDRESS

    return
