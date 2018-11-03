# Filename:	Makefile
# Author:   Alex Mikhalev
# Date:     December 11, 2017

-include config.mk

SRCS        := \
$(SRC_DIR)/datetime_util.cpp \
$(SRC_DIR)/dust_sensor.cpp \
$(SRC_DIR)/main.cpp \
$(SRC_DIR)/MultichannelGasSensor.cpp \
$(SRC_DIR)/printf.cpp \
$(SRC_DIR)/sd_files.cpp \
$(SRC_DIR)/sensor_data.cpp \
$(SRC_DIR)/sensors.cpp

OBJS        :=$(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(SRCS)))
DEPS		:=$(addprefix $(BUILD_DIR)/,$(addsuffix .d,$(SRCS)))
OUTPUT_ELF  :=$(BUILD_DIR)/$(NAME).elf
OUTPUT_HEX	:=$(BUILD_DIR)/$(NAME).hex
OUTPUT_LSS	:=$(BUILD_DIR)/$(NAME).lss

INCLUDES	:=-I $(SRC_DIR)

.PHONY: all config disassemble build clean size upload
.DEFAULT_GOAL := all

# GCC generated dependencies
-include $(DEPS)

# Libraries
-include lib/Arduino.mk
-include lib/Adafruit_BME280.mk
-include lib/SDFat.mk
-include lib/RTClib.mk

LIB_OBJS :=$(ARDUINO_OBJS) $(BME280_OBJS) $(SDFAT_OBJS) $(RTCLIB_OBJS)
INCLUDES +=$(ARDUINO_INCLUDES) $(BME280_INCLUDES) $(SDFAT_INCLUDES) $(RTCLIB_INCLUDES)

# Phony targets
all: build disassemble size
	
config:
	@echo BUILD_DIR=$(BUILD_DIR)
	@echo SRCS=$(SRCS)
	@echo OBJS=$(OBJS)
	@echo DEPS=$(DEPS)
	@echo CFLAGS=$(CFLAGS)
	@echo CXXFLAGS=$(CXXFLAGS)
	@echo ASFLAGS=$(ASFLAGS)
	@echo LIB_OBJS=$(LIB_OBJS)
	@echo INCLUDES=$(INCLUDES)

disassemble: $(OUTPUT_LSS)

build: $(OUTPUT_HEX)

clean: 
	@echo === Removing build files ===
	$(RM) $(OBJS) $(DEPS) $(OUTPUT_ELF) $(OUTPUT_HEX) $(OUTPUT_LSS) 
	$(RM) -r $(BUILD_DIR)/$(SRC_DIR)

lib_clean: arduino_clean bme280_clean sdfat_clean rtclib_clean
	$(RM) -r $(BUILD_DIR)/$(LIB_DIR)

size: $(OUTPUT_ELF)
	@echo === Printing output size ===
	avr-size --format=avr --mcu=$(MCU) $(OUTPUT_ELF)

upload: $(OUTPUT_HEX)
	@echo === Uploading "$(OUTPUT_HEX)" to the AVR using "$(AVRDUDE)" ===
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(AVRDUDE_PORT) $(AVRDUDE_FLAGS) -U flash:w:$(OUTPUT_HEX)

monitor:
	miniterm.py $(AVRDUDE_PORT) 115200

# File targets
$(BUILD_DIR):
	@echo === Creating "$(BUILD_DIR)" directory ====
	$(MKDIR) $(BUILD_DIR)

$(BUILD_DIR)/$(SRC_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(MKDIR) $(dir $@)
	$(CXX) "$@" $(INCLUDES) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c "$<"

$(OUTPUT_ELF): $(OBJS) $(LIB_OBJS)
	$(ELF) "$(OUTPUT_ELF)" $(OBJS) $(LIB_OBJS)
	
$(OUTPUT_HEX): $(OUTPUT_ELF)
	@echo === Copying binary to "$(OUTPUT_HEX)" in intel hex format ====
	$(OBJCOPY) -R .eeprom -R .fuse -R .lock -R .signature -O ihex $(OUTPUT_ELF) "$(OUTPUT_HEX)"

$(OUTPUT_LSS): $(OUTPUT_ELF)
	@echo === Disassembling "$(OUTPUT_ELF)" to "$(OUTPUT_LSS)" ====
	$(OBJDUMP) -h -S $(OUTPUT_ELF) >"$(OUTPUT_LSS)"
