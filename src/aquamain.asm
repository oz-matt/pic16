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

RES_VECT      CODE    0x0000
    goto        START

; TODO ADD INTERRUPTS HERE IF USED

MAIN_PROG CODE

START

    PIC_INIT                          ; Initialize clock, I/O, analog input, WDT, I2C

    call        IS_THIS_FIRST_PWRUP   ; 1 in w = first powerup, 0 in w = not first powerup
    iorlw       .0                    ; Update Z bit in status flag
    btfss       STATUS,Z
    call        FIRST_PWRUP_CODE      ; w is 1, so this means it is the first powerup. Init the am1805
                                    ; This line reached if it is not the first powerup, or the first
                                    ; powerup init code has been executed.
	call        DOUBLEBLINKLED
    clrwdt
	goto	    $-2

	END
