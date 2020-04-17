PROJECT:=blinky
ARCH:=STM32F103xB

# shared directory between lessons
COMMON:=../Common
include $(COMMON)/Common.make

# all source files used for compiling applicaton
SOURCES:= \
	$(PROJECT).c \
	$(SOURCES_HELPER)

# derived variables
OBJECTS:=$(SOURCES:.c=.o)
.PHONY: all clean

all: $(PROJECT).asm $(PROJECT).bin

%.bin: %.elf
	$(OBJCOPY) $< -O binary $@

%.asm: %.elf
	$(OBJDUMP) -S $< > $@

$(PROJECT).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.map *.elf *.bin *.asm *.hex $(OBJECTS)

# add debug & flashing support
include ../Common/Debug.make
