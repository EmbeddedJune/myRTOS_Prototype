ARCH = armv7-a
MCPU = cortex-a8
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
#LD = arm-none-eabi-ld
LD = arm-none-eabi-gcc
OC = arm-none-eabi-objcopy

TARGET = rvpb

LINKER_SCRIPT = ./script.ld
MAP_FILE = build/navilous.map

VPATH = boot			\
		hal/$(TARGET)	\
		lib				\
		kernel


ASM_SRCS = $(wildcard boot/*.S)
ASM_OBJS = $(patsubst boot/%.S, build/%.os, $(ASM_SRCS))

C_SRCS = $(notdir $(wildcard boot/*.c))
C_SRCS+= $(notdir $(wildcard hal/$(TARGET)/*.c))
C_SRCS+= $(notdir $(wildcard lib/*.c))
C_SRCS+= $(notdir $(wildcard kernel/*.c))
C_OBJS = $(patsubst %.c, build/%.o, $(C_SRCS))


INC_DIR =	-I include			\
			-I hal				\
			-I hal/$(TARGET)	\
			-I lib				\
			-I kernel

CFLAGS	= -c -g -std=c11
LDFLAGS = -nostartfiles -nostdlib -nodefaultlibs -static -lgcc

EXECUTABLE = build/navilous.axf
BINARY = build/navilous.bin

.PHONY: all clean run debug gdb

all: $(EXECUTABLE)

clean:
	@rm -rf build

run:
	qemu-system-arm -M realview-pb-a8 -kernel $(EXECUTABLE) -nographic

debug:
	qemu-system-arm -M realview-pb-a8 -kernel $(EXECUTABLE) -S -gdb tcp::1234,ipv4

gdb:
	arm-none-eabi-gdb

# To make executable file(.axf)
$(EXECUTABLE): $(ASM_OBJS) $(C_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(EXECUTABLE) $(ASM_OBJS) $(C_OBJS) \
			-Wl,-Map=$(MAP_FILE) $(LDFLAGS)
	$(OC) -O binary $(EXECUTABLE) $(BINARY)

# To make ELF object files by using assembler 
build/%.os: %.S
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) $(INC_DIR) $(CFLAGS) -o $@ $<

# To make ELF object files by using compiler 
build/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) $(INC_DIR) $(CFLAGS) -o $@ $<
