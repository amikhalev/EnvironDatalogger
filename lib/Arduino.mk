ARDUINO_DIR   :=$(LIB_DIR)/Arduino/hardware/arduino/avr
ARDUINO_BUILD :=$(BUILD_LIB_DIR)/Arduino
# ARDUINO_MKDEPS :=Makefile lib/Arduino.mk

ARDUINO_INCLUDES :=-I $(ARDUINO_DIR)/cores/arduino -I$(ARDUINO_DIR)/variants/standard \
-I$(ARDUINO_DIR)/libraries/EEPROM/src \
-I$(ARDUINO_DIR)/libraries/Wire/src \
-I$(ARDUINO_DIR)/libraries/SPI/src

ARDUINO_CPP_SRCS :=$(ARDUINO_DIR)/cores/arduino/abi.cpp \
$(ARDUINO_DIR)/cores/arduino/HardwareSerial.cpp \
$(ARDUINO_DIR)/cores/arduino/HardwareSerial0.cpp \
$(ARDUINO_DIR)/cores/arduino/main.cpp \
$(ARDUINO_DIR)/cores/arduino/new.cpp \
$(ARDUINO_DIR)/cores/arduino/Print.cpp \
$(ARDUINO_DIR)/cores/arduino/Stream.cpp \
$(ARDUINO_DIR)/cores/arduino/WMath.cpp \
$(ARDUINO_DIR)/cores/arduino/WString.cpp \
$(ARDUINO_DIR)/libraries/Wire/src/Wire.cpp \
$(ARDUINO_DIR)/libraries/SPI/src/SPI.cpp
ARDUINO_C_SRCS :=$(ARDUINO_DIR)/cores/arduino/hooks.c \
$(ARDUINO_DIR)/cores/arduino/WInterrupts.c \
$(ARDUINO_DIR)/cores/arduino/wiring_analog.c \
$(ARDUINO_DIR)/cores/arduino/wiring_digital.c \
$(ARDUINO_DIR)/cores/arduino/wiring_pulse.c \
$(ARDUINO_DIR)/cores/arduino/wiring_shift.c \
$(ARDUINO_DIR)/cores/arduino/wiring.c \
$(ARDUINO_DIR)/libraries/Wire/src/utility/twi.c
ARDUINO_S_SRCS :=$(ARDUINO_DIR)/cores/arduino/wiring_pulse.S
ARDUINO_CPP_OBJS :=$(patsubst $(ARDUINO_DIR)/%.cpp,$(ARDUINO_BUILD)/%.cpp.o,$(ARDUINO_CPP_SRCS))
ARDUINO_C_OBJS 	 :=$(patsubst $(ARDUINO_DIR)/%.c,$(ARDUINO_BUILD)/%.c.o,$(ARDUINO_C_SRCS))
ARDUINO_S_OBJS   :=$(patsubst $(ARDUINO_DIR)/%.S,$(ARDUINO_BUILD)/%.S.o,$(ARDUINO_S_SRCS))
ARDUINO_OBJS     :=$(ARDUINO_CPP_OBJS) $(ARDUINO_C_OBJS) $(ARDUINO_S_OBJS)

ARDUINO_LIB := $(BUILD_LIB_DIR)/libArduino.a

.PHONY: arduino_config arduino_build arduino_clean

arduino_config:
	@echo $$\(ARDUINO_DIR\)=$(ARDUINO_DIR)
	@echo $$\(ARDUINO_INCLUDES\)=$(ARDUINO_INCLUDES)
	@echo $$\(ARDUINO_CPP_SRCS\)=$(ARDUINO_CPP_SRCS)
	@echo $$\(ARDUINO_C_SRCS\)=$(ARDUINO_C_SRCS)
	@echo $$\(ARDUINO_S_SRCS\)=$(ARDUINO_S_SRCS)
	@echo $$\(ARDUINO_CPP_OBJS\)=$(ARDUINO_CPP_OBJS)
	@echo $$\(ARDUINO_C_OBJS\)=$(ARDUINO_C_OBJS)
	@echo $$\(ARDUINO_S_OBJS\)=$(ARDUINO_S_OBJS)
	@echo $$\(ARDUINO_OBJS\)=$(ARDUINO_OBJS)
	@echo $$\(ARDUINO_LIB\)=$(ARDUINO_LIB)

arduino_build: $(ARDUINO_LIB)

arduino_clean:
	$(RM) $(ARDUINO_OBJS) $(ARDUINO_LIB)

$(ARDUINO_BUILD)/%.cpp.o: $(ARDUINO_DIR)/%.cpp $(ARDUINO_MKDEPS)
	$(MKDIR) $(dir $@)
	$(CXX) -o "$@" $(ARDUINO_INCLUDES) -c "$<"

$(ARDUINO_BUILD)/%.c.o: $(ARDUINO_DIR)/%.c $(ARDUINO_MKDEPS)
	$(MKDIR) $(dir $@)
	$(CC) -o "$@" $(ARDUINO_INCLUDES) -c "$<"

$(ARDUINO_BUILD)/%.S.o: $(ARDUINO_DIR)/%.S $(ARDUINO_MKDEPS)
	$(MKDIR) $(dir $@)
	$(AS) -o "$@" -c "$<"

$(ARDUINO_LIB): $(ARDUINO_OBJS) $(ARDUINO_MKDEPS)
	$(AR) rc $(ARDUINO_LIB) $(ARDUINO_OBJS)
	$(RANLIB) $(ARDUINO_LIB)
