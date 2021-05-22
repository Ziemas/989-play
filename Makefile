#test _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

EE_BIN = play.elf
EE_OBJS = play.o

EE_CFLAGS += -std=c99

EE_LIBS = -ldebug
EE_LDFLAGS += -L$(PS2SDK)/ports/lib


all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS) romdisk.img

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

sim: $(EE_BIN)
	PCSX2 --elf=$(PWD)/$(EE_BIN) --nogui

reset:
	ps2client reset

include $(PS2SDK)/Defs.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
