
EUSART_FXNS	    CODE

EUSARTInit

		movlw	    			H'0C'
		BANKSEL	    		SP1BRGL
		movwf	    			SP1BRGL	    ; Put 12(base 10) into SPBRGL. Now we can
					    			; have a ~150 baud

		BANKSEL	    		TXSTA
		bsf	    				TXSTA,BRGH
		nop
		bcf	    				TXSTA,SYNC
		nop
		BANKSEL	    		BAUDCON
		bcf	    				BAUDCON,BRG16   ; SYNC = 0, BRG16 = 0 and BRGH = 1, this
					    ; selects 8-bit/Asynchronous with a baud of
					    ; Fosc/(16(n+1)) where n is the value of SPBRG
					    ; (set above to 12). The resulting baud is ~150
					    ; since 31000/208 ~= 150
		nop
		BANKSEL	    		RCSTA
		bsf	    				RCSTA,SPEN	    ; enable serial port
		nop
		BANKSEL	    		TXSTA
		bsf	    				TXSTA,TXEN	    ; enable transmits on serial port

		return

EUSART_TRANSMIT			    ; Transmits character in 'NEXT_UART_CHAR'
		BANKSEL	    		NEXT_UART_CHAR
		movf	    			NEXT_UART_CHAR,w
		BANKSEL	    		TXREG
		movwf	    			TXREG
		BANKSEL	    		PIR1
		btfsc	    			PIR1,TXIF
		goto	    			$-1
		return

EUSART_TRANSMIT_CHAR_ADJ

		BANKSEL	    		HUNS_CTR
		movlw	    			H'00'
		movwf	    			HUNS_CTR			    ; Clear the hundreds counter

		BANKSEL	    		TENS_CTR
		movlw	    			H'00'
		movwf	    			TENS_CTR			    ; Clear the tens counter

		BANKSEL	    		ONES_CTR
		movlw	    			H'00'
		movwf	    			ONES_CTR			    ; Clear the ones counter

SUB_HUN_FROM_CHAR:
		BANKSEL	    		NEXT_UART_CHAR_BEFORE_ADJ
		movlw	    			H'64'
		subwf	    			NEXT_UART_CHAR_BEFORE_ADJ,w	    ; Result of NEXTCHAR - 100 is now in W
		btfss	    			STATUS,C			    ; Skip if NEXTCHAR >= 100
		goto	    			SUB_TEN_FROM_CHAR
		incf	    			HUNS_CTR,f
		BANKSEL	    		NEXT_UART_CHAR_BEFORE_ADJ
		movwf	    			NEXT_UART_CHAR_BEFORE_ADJ
		goto	    			SUB_HUN_FROM_CHAR

SUB_TEN_FROM_CHAR:
		BANKSEL	    		NEXT_UART_CHAR_BEFORE_ADJ
		movlw	    			H'0A'
		subwf	    			NEXT_UART_CHAR_BEFORE_ADJ,w	    ; Result of NEXTCHAR - 10 is now in W
		btfss	    			STATUS,C			    ; Skip if NEXTCHAR >= 10
		goto	    			ONES_LESS_THAN_TEN		    ; NEXTCHAR Remainder is < 10
		incf	    			TENS_CTR,f			    ; NEXTCHAR Remainder is >= 10, so we continue to subtract 10 until C is zero
		BANKSEL	    		NEXT_UART_CHAR_BEFORE_ADJ
		movwf	    			NEXT_UART_CHAR_BEFORE_ADJ
		goto	    			SUB_TEN_FROM_CHAR

ONES_LESS_THAN_TEN:
		BANKSEL	    		NEXT_UART_CHAR_BEFORE_ADJ
		movf	    			NEXT_UART_CHAR_BEFORE_ADJ,w
		BANKSEL	    		ONES_CTR
		movwf	    			ONES_CTR
		goto	    			READY_TO_PRINT

READY_TO_PRINT:

		TSTF	    			HUNS_CTR
		btfss	    			STATUS,Z
		goto	    			PRINT_HUNDREDS		;Hundreds place is not zero
		TSTF	    			TENS_CTR
		btfss	    			STATUS,Z
		goto	    			PRINT_TENS			;Hundreds place is zero, but tens place is not
		goto	    			PRINT_ONES			;Hundreds and tens place are zero

PRINT_HUNDREDS:
		movlw	    			H'30'
		BANKSEL	    		HUNS_CTR
		addwf	    			HUNS_CTR,w
		BANKSEL	    		NEXT_UART_CHAR
		movwf	    			NEXT_UART_CHAR
		call	    			EUSART_TRANSMIT

		BANKSEL	    		MSLEFT
		movlw	    			H'60'
		movwf	    			MSLEFT
		call	    			DELAYMS

PRINT_TENS:
		movlw	    			H'30'
		BANKSEL	    		TENS_CTR
		addwf	    			TENS_CTR,w
		BANKSEL	    		NEXT_UART_CHAR
		movwf	    			NEXT_UART_CHAR
		call	    			EUSART_TRANSMIT

		BANKSEL	    		MSLEFT
		movlw	    			H'60'
		movwf	    			MSLEFT
		call	    			DELAYMS

PRINT_ONES:
		movlw	    			H'30'
		BANKSEL	    		ONES_CTR
		addwf	    			ONES_CTR,w
		BANKSEL	    		NEXT_UART_CHAR
		movwf	    			NEXT_UART_CHAR
		call	    			EUSART_TRANSMIT
		return
