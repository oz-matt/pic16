#include "p16lf1937.inc"
#include "delay.inc"
#include "udata.inc"
#include "io.inc"
#include "wdt.inc"
#include "i2c.inc"
#include "macro.inc"
#include "ichex.inc"
#include "eeprom.inc"
#include "brex.inc"
#include "am1805.inc"
#include "math.inc"
#include "eusart.inc"

RES_VECT      CODE    0x0000
    goto        START

; TODO ADD INTERRUPTS HERE IF USED

MAIN_PROG CODE

START

    PIC_INIT                          ; Initialize clock, I/O, analog input, WDT, I2C

    call        IS_THIS_FIRST_PWRUP   ; 1 in w = first powerup, 0 in w = not first powerup
    iorlw       .0                    ; Update Z bit in status flag
    BANKSEL     STATUS
    btfss       STATUS,Z
    call        FIRST_PWRUP_CODE      ; w is 1, so this means it is the first powerup. Init the am1805
                                    ; This line reached if it is not the first powerup, or the first
                                    ; powerup init code has been executed.
    call        IS_USBON_HIGH
    iorlw       .0
    BANKSEL     STATUS
    btfss       STATUS,Z
    call        USB_PLUGGED_AT_START  ; w is 1, USB is plugged in
                                    ; w is 0, so USB is not plugged in

    call        ENSUREPSW_LOW         ; Make sure we don't get shut off by AM1805 inadvertently

    call        IS_TC_HIGH
    iorlw       .0
    BANKSEL     STATUS
    btfss       STATUS,Z
    call        TC_HIGH               ; w is 1, TC is high, AM1805 just woke us up.
                                    ; w is 0, TC is low, Main power turn-on just woke us up.

    call        AM1805_ARE_WE_ASLEEP
    iorlw       .0
    BANKSEL     STATUS
    btfss       STATUS,Z
    call        SLEEP_ERROR           ; w is 1, we are sleeping
                                    ; w is 0, we are not sleeping
    call        ALLOW_UPDATE_SD_TO_EEPROM

    call        RESET_INSTRUCTION_PTR

	call        DOUBLEBLINKLED
    clrwdt
	goto	    $-2

	END
