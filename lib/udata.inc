
AQUA_UDATA                    UDATA
MSTMR                         RES		1   ; Free register address where MSTMR value is stored
MSLEFT                        RES		1   ; Free register address where MSLEFT value is stored
MS100LEFT                     RES		1   ; Free register address where MS100LEFT value is stored

NEXT_UART_CHAR_BEFORE_ADJ	  RES		1
NEXT_UART_CHAR                RES		1

NEXT_I2C_ADDRESS		      RES		1   ; Register where we are storing the next address in the am1805 that we want to write to
NEXT_I2C_DATA			      RES		1   ; Register where we are storing the next byte of data that we want to send to the above address

NEXT_AM1805_VAL			      RES		1

LATEST_I2C_BYTE			      RES		1

TENS_CTR                      RES		1
ONES_CTR                      RES		1
HUNS_CTR                      RES		1

MATH_X				          RES		1
MATH_Y				          RES		1
MATH_RESULT			          RES		1

NEXT_CTDWN_VAL                RES		1
PRINT_REGISTER                RES		1

FOUT_CTR                      RES		1

TEST_CTR                      RES		1

SLEEP_REG			          RES		1

SLEEP_INTERVALS_LEFT		  RES		1   ; EEPROM address 0x00
SLEEP_INTERVAL_DURATION		  RES		1   ; EEPROM address 0x01
SLEEP_INTERVAL_FINAL		  RES		1   ; EEPROM address 0x02
PICALWAYSON_BIT               RES		1   ; EEPROM address 0x03

NEXT_MASK			          RES		1

CT_WORK                       RES		1
CT_WORK_TENS                  RES		1

CT_SEC                        RES		1
CT_MIN				          RES		1
CT_HR                         RES		1
CT_DAY				          RES		1
CT_MO                         RES		1
CT_YR                         RES		1
