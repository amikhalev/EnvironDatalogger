# Filename: config.mk

# General configuration
NAME        :=EnvironDatalogger
MCU         :=atmega328p
F_CPU       :=16000000UL
ARDUINO     :=10620
PROGRAMMER  :=arduino

# Directories and files
SRC_DIR     :=src
LIB_DIR     :=lib
BUILD_DIR   :=build
BUILD_LIB_DIR :=$(BUILD_DIR)/lib

GCC_FLAGS   :=-Wall -Os -g -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -fno-exceptions -funsigned-char \
-funsigned-bitfields -flto -mmcu=$(MCU) \
-DF_CPU=$(F_CPU) -DARDUINO_ARCH_AVR -DARDUINO_AVR_UNO -DARDUINO=$(ARDUINO)
CFLAGS      :=$(GCC_FLAGS) -std=c99  
CXXFLAGS    :=$(GCC_FLAGS) -fno-threadsafe-statics -fpermissive -felide-constructors -fno-exceptions -std=gnu++11
ASFLAGS     :=-x assembler-with-cpp $(GCC_FLAGS)

AVRDUDE_PORT =$(shell ./scripts/find_port.sh)

# Tool configs
RM          =@rm -f
MKDIR       =@mkdirp() { [ -d "$$1" ] || echo "==> mkdir creating directory $$1"; mkdir -p $$@; }; mkdirp
LD          =@ld() { echo "==> ld linking $$2"; avr-gcc $$@; }; ld
CC          =@cc() { echo "==> gcc compiling $$2"; avr-gcc $(CFLAGS) $$@; }; cc
CXX         =@cxx() { echo "==> g++ compiling $$2"; avr-g++ $(CXXFLAGS) $$@; }; cxx
AS          =@as() { echo "==> g++ assembling $$2"; avr-g++ $(ASFLAGS) $$@; }; as
AR          =@ar() { echo "==> ar archiving $$2"; avr-ar $$@; }; ar
RANLIB      =@ranlib() { echo "==> ranlib indexing $$1"; avr-ranlib $$@; }; ranlib
OBJDUMP     =@avr-objdump
OBJCOPY     =@avr-objcopy
AVRDUDE     =avrdude
