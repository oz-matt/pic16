
WDT_FXNS	    CODE

WatchdogPeriodInit
	movlw 			b'00011100'
	BANKSEL	    	WDTCON
	movwf	    	WDTCON
			; Watchdog Timer now set to reset MCU
			; after a 16s idle (with no CLRWDT instruction)
	return

WDTest
	btfsc	    	STATUS,NOT_TO    	; if we have a regular power reset, led blinks once
	call	    	DOUBLEBLINKLED  	; if we have a wdt reset, it blinks twice
	call	    	SINGLEBLINKLED
	return
