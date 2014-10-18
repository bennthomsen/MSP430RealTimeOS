#
# Makefile for msp430
#
# 'make' builds everything
# 'make clean' deletes everything except source files and Makefile
# You need to set TARGET, MCU and SOURCES for your project.
# TARGET is the name of the executable file to be produced
# $(TARGET).elf $(TARGET).hex and $(TARGET).txt nad $(TARGET).map are all generated.
# The TXT file is used for BSL loading, the ELF can be used for JTAG use
#
# Adapted by Alessandro Pasotti - 2013 - from original:
#   http://sourceforge.net/apps/mediawiki/mspgcc/index.php?title=Example:Makefile
#

# Adapt the following to your path

COMPILER_PATH=//Applications/Energia.app/Contents/Resources/Java/hardware/tools/msp430/bin/
INCLUDES :=-I //Applications/Energia.app/Contents/Resources/Java/hardware/tools/msp430/msp430/include/
UPLOADER_PATH=//Applications/Energia.app/Contents/Resources/Java/hardware/tools/msp430/mspdebug/

# Target MCU
MCU        = msp430g2553

# USB port for upload
PORT = /dev/tty.uart*


######################################################################################

INOFILE := main.c

TARGET	:= $(basename $(INOFILE))

# List all the source files here
# eg if you have a source file foo.c then list it here
SOURCES = $(INOFILE) RealTimeClock.c UART.c ADC.c print.c LCDDisplay.c i2c.c
# Include are located in the Include directory
#INCLUDES = -IInclude
# Add or subtract whatever MSPGCC flags you want. There are plenty more
#######################################################################################
CFLAGS   = -mmcu=$(MCU) -g -Os -Wall -Wunused $(INCLUDES)
ASFLAGS  = -mmcu=$(MCU) -x assembler-with-cpp -Wa,-gstabs
LDFLAGS  = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map
########################################################################################


CC       = $(COMPILER_PATH)msp430-gcc
LD       = $(COMPILER_PATH)msp430-ld
AR       = $(COMPILER_PATH)msp430-ar
AS       = $(COMPILER_PATH)msp430-gcc
GASP     = $(COMPILER_PATH)msp430-gasp
GDB      = $(COMPILER_PATH)msp430-gdb
NM       = $(COMPILER_PATH)msp430-nm
OBJCOPY  = $(COMPILER_PATH)msp430-objcopy
RANLIB   = $(COMPILER_PATH)msp430-ranlib
STRIP    = $(COMPILER_PATH)msp430-strip
SIZE     = $(COMPILER_PATH)msp430-size
READELF  = $(COMPILER_PATH)msp430-readelf
MAKETXT  = $(COMPILER_PATH)srec_cat
CP       = $(COMPILER_PATH)cp -p
RM       = rm -f
MV       = $(COMPILER_PATH)mv
MSPDEBUG = $(UPLOADER_PATH)mspdebug
########################################################################################
# the file which will include dependencies
DEPEND = $(SOURCES:.c=.d)
# all the object files
OBJECTS = $(SOURCES:.c=.o)
all: $(TARGET).elf $(TARGET).hex
$(TARGET).elf: $(OBJECTS)
	echo "Linking $@"
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@
	echo
	echo ">>>> Size of Firmware <<<<"
	$(SIZE) $(TARGET).elf
	echo

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

upload: $(TARGET).elf
	@echo "\nUploading to board..."
	$(MSPDEBUG) rf2500 'erase' "load $(TARGET).elf" 'exit'

%.o: %.c
	echo "Compiling $<"
	$(CC) -c $(CFLAGS) -o $@ $<
# rule for making assembler source listing, to see the code
%.lst: %.c
	$(CC) -c $(ASFLAGS) -Wa,-anlhd $< > $@
	# include the dependencies unless we're going to clean, then forget about them.
	ifneq ($(MAKECMDGOALS), clean)
	-include $(DEPEND)
	endif
	# dependencies file
	# includes also considered, since some of these are our own
	# (otherwise use -MM instead of -M)
%.d: %.c
	echo "Generating dependencies $@ from $<"
	$(CC) -M ${CFLAGS} $< >$@
.SILENT:
.PHONY:	clean
clean:
	-$(RM) $(OBJECTS)
	-$(RM) $(TARGET).o $(TARGET).elf $(TARGET).lss $(TARGET).map $(TARGET).hex

debug:
	$(MSPDEBUG) rf2500 gdb
	cgdb  -d $(GDB) $(TARGET).elf