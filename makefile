SRC_DIR			:= /home/matt/aqua/pic16lf1938/src
LIB_DIR			:= /home/matt/aqua/pic16lf1938/lib
INC_DIR			:= /home/matt/aqua/pic16lf1938/inc
LKR_DIR			:= /home/matt/aqua/pic16lf1938/lkr
OUT_DIR			:= /home/matt/aqua/pic16lf1938/out
SCR_DIR			:= /home/matt/aqua/pic16lf1938/scr
SYS_DIR			:= /home/matt/aqua/pic16lf1938/sys

SRC_FILES		:= $(wildcard $(SRC_DIR)/*.asm)
LIB_FILES		:= $(wildcard $(LIB_DIR)/*.inc)
INC_FILES		:= $(wildcard $(INC_DIR)/*.inc)
OBJ_FILES		:= $(wildcard $(OUT_DIR)/*.o)
LKR_FILE		:= $(LKR_DIR)/16lf1937_g.lkr

MBD					:= /opt/microchip/mplabx/v3.20/mplab_ide/bin/mdb.sh
PROG_SCR		:= $(SCR_DIR)/prog.cmd

default: $(OUT_DIR)/aquamain.hex

install: $(OUT_DIR)/aquamain.hex
	$(MBD) $(PROG_SCR)

clean:
	rm out/*.o

$(OUT_DIR)/aquamain.o: $(SRC_DIR)/aquamain.asm
	gpasm -c -p pic16lf1937 -I$(LIB_DIR) -I$(INC_DIR) -I$(SYS_DIR) $< -o $@

$(OUT_DIR)/aquamain.hex: $(OUT_DIR)/aquamain.o
	gplink --map -c -s $(LKR_FILE) -I$(OUT_DIR) $< -o $@
	rm out/*.o
