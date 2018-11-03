# Filename: config.mk

# General configuration
NAME        :=EnvironDatalogger
MCU         :=atmega328p
F_CPU       :=16000000UL
ARDUINO     :=10620
BOARD		:=AVR_UNO
PROGRAMMER  :=arduino

# Directories and files
SRC_DIR     :=src
LIB_DIR     :=lib
BUILD_DIR   :=build
BUILD_LIB_DIR :=$(BUILD_DIR)/lib

ARDUINO_FLAGS=-DF_CPU=$(F_CPU) -DARDUINO_ARCH_AVR -DARDUINO_$(BOARD) -DARDUINO=$(ARDUINO) -mmcu=$(MCU)
override GCC_FLAGS   +=-Wall -Wextra -Os -g -fpack-struct -fshort-enums -ffunction-sections -fdata-sections \
-fno-exceptions -funsigned-char -funsigned-bitfields -flto -fno-fat-lto-objects
CFLAGS      =$(ARDUINO_FLAGS) $(GCC_FLAGS) -std=gnu11  
CXXFLAGS    =$(ARDUINO_FLAGS) $(GCC_FLAGS) -std=gnu++11 -fno-threadsafe-statics -fpermissive -felide-constructors -fno-exceptions -Wno-error=narrowing
ASFLAGS		=$(ARDUINO_FLAGS) $(GCC_FLAGS) -x assembler-with-cpp
ELF_FLAGS   =-Os -Wl,--gc-sections -mmcu=$(MCU) -fuse-linker-plugin

AVRDUDE_PORT =$(shell ./scripts/find_port.sh)

# Tool configs
RM          =@rm -rf
MKDIR       =@mkdirp() { [ -d "$$1" ] || echo "==> mkdir creating directory $$1"; mkdir -p $$@; }; mkdirp
ELF         =@elf() { echo "==> elf linking $$1"; avr-gcc $(ELF_FLAGS) -o $$@; }; elf
CC          =@cc() { echo "==> gcc compiling $$1"; avr-gcc $(CFLAGS) -o $$@; }; cc
CXX         =@cxx() { echo "==> g++ compiling $$1"; avr-g++ $(CXXFLAGS) -o $$@; }; cxx
AS          =@as() { echo "==> g++ assembling $$1"; avr-g++ $(ASFLAGS) -o $$@; }; as
AR          =@ar() { echo "==> ar archiving $$3"; avr-ar $$@; }; ar
RANLIB      =@ranlib() { echo "==> ranlib indexing $$1"; avr-ranlib $$@; }; ranlib
OBJDUMP     =@avr-objdump
OBJCOPY     =@avr-objcopy
AVRDUDE     =avrdude
