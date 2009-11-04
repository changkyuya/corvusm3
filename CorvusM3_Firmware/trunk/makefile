# setup environment

#DEBUG = 1	# or include DEBUG=1 in the command line
MESSAGES = -fmessage-length=0

export DEBUG
export MESSAGES

TARGET_ARCH = -mcpu=cortex-m3 -mthumb

INCLUDE_DIRS = -I . -I fwlib -I fwlib/inc
LIBRARY_DIRS = -L lanchon-stm32 -L fwlib

LINK_FWLIB = $(FWLIB)	# comment this if not using the firmware library

COMPILE_OPTS = $(WARNINGS) $(TARGET_OPTS) $(MESSAGES) $(INCLUDE_DIRS)
WARNINGS = -Wall -W $(W_CONVERSION) -Wshadow -Wcast-qual -Wwrite-strings -Winline
#W_CONVERSION = -Wconversion

WARNINGS_CXX = -Weffc++ $(W_OLD_STYLE_CAST)
#W_OLD_STYLE_CAST = -Wold-style-cast
#F_NO_EXCEPTIONS = -fno-exceptions	# disabling exceptions saves code space

ifdef DEBUG
 TARGET_OPTS = -O0 -g3
 DEBUG_MACRO = -DDEBUG
else
 TARGET_OPTS = -O2 $(F_INLINE) $(F_INLINE_ONCE) $(F_UNROLL_LOOPS)
 F_INLINE = -finline
 F_INLINE_ONCE = -finline-functions-called-once
 #F_UNROLL_LOOPS = -funroll-loops
endif

CPPFLAGS = $(DEBUG_MACRO)

CC = arm-none-eabi-gcc
CFLAGS = -std=gnu99 $(COMPILE_OPTS)

CXX = arm-none-eabi-g++
CXXFLAGS = -std=gnu++98 $(COMPILE_OPTS) $(WARNINGS_CXX) $(F_NO_EXCEPTIONS)

AS = $(CC) -x assembler-with-cpp -c $(TARGET_ARCH)
ASFLAGS = $(COMPILE_OPTS)

LD = $(CC)
#LD = $(CXX)
LDFLAGS = -Wl,--gc-sections,-Map=$(MAIN_MAP),-cref -T lanchon-stm32.ld $(INCLUDE_DIRS) $(LIBRARY_DIRS)


AR = arm-none-eabi-ar
ARFLAGS = cr

OBJCOPY = arm-none-eabi-objcopy
OBJCOPYFLAGS = -O binary


MAIN_OUT = main.elf
MAIN_MAP = $(MAIN_OUT:%.elf=%.map)
MAIN_BIN = $(MAIN_OUT:%.elf=%.bin)

MAIN_OBJS = $(sort \
 $(patsubst %.cpp,%.o,$(wildcard *.cpp)) \
 $(patsubst %.cc,%.o,$(wildcard *.cc)) \
 $(patsubst %.c,%.o,$(wildcard *.c)) \
 $(patsubst %.s,%.o,$(wildcard *.s)))

FWLIB = fwlib/libstm32fw.a


# all

.PHONY: all
all: $(MAIN_BIN)


# main

$(MAIN_OUT): $(MAIN_OBJS) $(LINK_FWLIB)
	$(LD) $(LDFLAGS) $(TARGET_ARCH) $^ -o $@

$(MAIN_OBJS): $(wildcard *.h) $(wildcard fwlib/*.h) $(wildcard fwlib/inc/*.h)

$(MAIN_BIN): $(MAIN_OUT)
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@


# flash

.PHONY: flash
flash: flash-elf
#flash: flash-bin

.PHONY: flash-elf
flash-elf: all
	@cp $(MAIN_OUT) jtag/flash.elf
	@cd jtag && openocd -f flash-elf.cfg
	@rm jtag/flash.elf

.PHONY: flash-bin
flash-bin: all
	@cp $(MAIN_BIN) jtag/flash.bin
	@cd jtag && openocd -f flash-bin.cfg
	@rm jtag/flash.bin


# fwlib

.PHONY: fwlib
fwlib: $(FWLIB)

$(FWLIB): $(wildcard fwlib/*.h) $(wildcard fwlib/inc/*.h)
	@cd fwlib && $(MAKE)


# clean

.PHONY: clean
clean:
	-rm -f $(MAIN_OBJS) $(MAIN_OUT) $(MAIN_MAP) $(MAIN_BIN) jtag/flash.elf jtag/flash.bin
	@cd fwlib && $(MAKE) clean
