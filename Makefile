######################################
# target
######################################
TARGET = DMPY

######################################
# building variables
######################################
# debug build?
ifdef DEBUG
DEBUG = 1
endif

#######################################
# paths
#######################################

PROJECT_ROOT = $(realpath .)
SRC_DIR = $(PROJECT_ROOT)/src
DMCP_SDK_DIR = $(PROJECT_ROOT)/DMCP_SDK

# Build path
BUILD_DIR = $(PROJECT_ROOT)/build

# Path to aux build scripts (including trailing /)
# Leave empty for scripts in PATH
BIN_DIR = $(DMCP_SDK_DIR)/bin/

MICROPY_TOP=$(PROJECT_ROOT)/micropython
LIBMICROPYTHON_PORT=$(MICROPY_TOP)/ports/libmicropython
LIBMICROPYTHON_BUILD=$(PROJECT_ROOT)/build-libmicropython
LIBMICROPYTHON_A=$(LIBMICROPYTHON_BUILD)/libmicropython.a

######################################
# System sources
######################################
C_INCLUDES += -Idmcp
C_SOURCES += $(DMCP_SDK_DIR)/dmcp/sys/pgm_syscalls.c
ASM_SOURCES = $(DMCP_SDK_DIR)/dmcp/startup_pgm.s

#######################################
# Custom section
#######################################

# Includes
C_INCLUDES += -I. -I$(SRC_DIR) -I$(DMCP_SDK_DIR)/dmcp

# C sources
C_SOURCES += $(SRC_DIR)/main.c $(SRC_DIR)/module_dmcp.c $(SRC_DIR)/module_dmpy.c $(SRC_DIR)/reader.c

# C++ sources
#CXX_SOURCES += src/xxx.cc

# ASM sources
#ASM_SOURCES += src/xxx.s

# Additional defines
#C_DEFS += -DMODULE_EXAMPLE_ENABLED

# Libraries
ifeq ($(DEBUG), 1)
#LIBS += lib/libxxx_dbg.a
else
#LIBS += lib/libxxx.a
endif

# ---

# MICROPYTHON


# For mpconfigport.h libmicropython headers.
MPY_INC += -I$(LIBMICROPYTHON_PORT)

# For generated headers from the libmicropython build.
MPY_INC += -I$(LIBMICROPYTHON_BUILD)

# For all MicroPython headers.
MPY_INC += -I$(MICROPY_TOP)



#######################################
# binaries
#######################################
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
AS = arm-none-eabi-gcc -x assembler-with-cpp
OBJCOPY = arm-none-eabi-objcopy
AR = arm-none-eabi-ar
SIZE = arm-none-eabi-size
HEX = $(OBJCOPY) -O ihex
BIN = $(OBJCOPY) -O binary -S

#######################################
# CFLAGS
#######################################
# macros for gcc
AS_DEFS =
C_DEFS += -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))"
AS_INCLUDES =


CPUFLAGS += -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16


# compile gcc flags
ASFLAGS = $(CPUFLAGS) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS  = $(CPUFLAGS) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS += -Wno-misleading-indentation
CFLAGS += $(MPY_INC)
DBGFLAGS = -g 

ifeq ($(DEBUG), 1)
CFLAGS += -O0 -DDEBUG
else
CFLAGS += -O2
endif

CFLAGS  += $(DBGFLAGS)
LDFLAGS += $(DBGFLAGS)

# Generate dependency information
CFLAGS += -MD -MP -MF .dep/$(@F).d

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = $(DMCP_SDK_DIR)/stm32_program.ld
LIBDIR =
LDFLAGS = $(CPUFLAGS) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections \
  -Wl,--wrap=_malloc_r


# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf 

# libmicropython:
# Overrides:
#  BUILD to set the directory for compile artifacts.
#  LIBMICROPYTHON to set the output path for libmicropython.a
#  MICROPY_QSTRDEFSEMBED_H to use our custom qstrdefsembed.h
#  MICROPY_MPCONFIGEMBED_H to use our custom mpconfigembed.h
#  V to inherit our verbosity setting
$(LIBMICROPYTHON_A): qstrdefsembed.h mpconfigembed.h
	@$(MAKE) \
            -C $(LIBMICROPYTHON_PORT) \
            BUILD=$(LIBMICROPYTHON_BUILD) \
            LIBMICROPYTHON=$(LIBMICROPYTHON_A) \
            MICROPY_QSTRDEFSEMBED_H=$(PROJECT_ROOT)/qstrdefsembed.h \
            MICROPY_MPCONFIGEMBED_H=$(PROJECT_ROOT)/mpconfigembed.h \
            CROSS_COMPILE=arm-none-eabi- \
            CFLAGS_EXTRA="-DPATH_MAX=256 -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16" \
            lib

#######################################
# build the application
#######################################
# list of objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# C++ sources
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CXX_SOURCES:.cc=.o)))
vpath %.cc $(sort $(dir $(CXX_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cc Makefile | $(BUILD_DIR) 
	$(CXX) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cc=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(LIBMICROPYTHON_A) $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -L $(LIBMICROPYTHON_BUILD) -l:libmicropython.a -o $@
	$(OBJCOPY) --remove-section .qspi -O ihex    $@  $(BUILD_DIR)/$(TARGET)_flash.hex
	$(OBJCOPY) --remove-section .qspi -O binary  $@  $(BUILD_DIR)/$(TARGET)_flash.bin
	$(OBJCOPY) --only-section   .qspi -O ihex    $@  $(BUILD_DIR)/$(TARGET)_qspi.hex
	$(OBJCOPY) --only-section   .qspi -O binary  $@  $(BUILD_DIR)/$(TARGET)_qspi.bin
	$(BIN_DIR)check_qspi_crc $(TARGET) qspi_crc.h || ( $(MAKE) clean-light && false )
	$(BIN_DIR)add_pgm_chsum build/$(TARGET)_flash.bin build/$(TARGET).pgm
	$(SIZE) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@
	
$(BUILD_DIR):
	mkdir -p $@

#######################################
# clean up
#######################################
clean-light:
	-rm -fR .dep $(BUILD_DIR)/*.o $(BUILD_DIR)/*.lst

clean:
	-rm -rf .dep $(BUILD_DIR) $(LIBMICROPYTHON_BUILD)

#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

.PHONY: clean all

