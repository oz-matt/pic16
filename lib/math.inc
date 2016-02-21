
MATH_FXNS	          CODE

MULV8
		clrf            MATH_RESULT
MULU8LOOP:
		movf	          MATH_X,W
		btfsc	          MATH_Y,0
		addwf          MATH_RESULT,f
		bcf	    		  STATUS,C
		rrf	    		  MATH_Y,F
		bcf	    		  STATUS,C
		rlf	    		  MATH_X,F
		movf	    	  MATH_Y,F
		btfss	    	  STATUS,Z
		goto	    	  MULU8LOOP
		return
