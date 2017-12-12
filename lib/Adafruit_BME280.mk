ADA_SENSOR_DIR  :=$(LIB_DIR)/Adafruit_Sensor
BME280_DIR      :=$(LIB_DIR)/Adafruit_BME280_Library
BME280_BUILD    :=$(BUILD_LIB_DIR)/Adafruit_BME280_Library
BME280_MKDEPS   :=Makefile lib/Adafruit_BME280.mk
BME280_INCLUDES :=-I $(BME280_DIR) -I $(ADA_SENSOR_DIR)

BME280_CPP_SRCS :=$(BME280_DIR)/Adafruit_BME280.cpp
BME280_CPP_OBJS :=$(patsubst $(BME280_DIR)/%.cpp,$(BME280_BUILD)/%.cpp.o,$(BME280_CPP_SRCS))
BME280_OBJS     :=$(BME280_CPP_OBJS)
BME280_LIB      :=$(BUILD_LIB_DIR)/libAdafruitBME280.a

.PHONY: bme280_config bme280_build bme280_clean

bme280_config:
	@echo $$\(BME280_DIR\)=$(BME280_DIR)
	@echo $$\(BME280_INCLUDES\)=$(BME280_INCLUDES)
	@echo $$\(BME280_CPP_SRCS\)=$(BME280_CPP_SRCS)
	@echo $$\(BME280_CPP_OBJS\)=$(BME280_CPP_OBJS)
	@echo $$\(BME280_OBJS\)=$(BME280_OBJS)
	@echo $$\(BME280_LIB\)=$(BME280_LIB)

bme280_build: $(BME280_LIB)

bme280_clean:
	$(RM) $(BME280_OBJS) $(BME280_LIB)

$(BME280_BUILD)/%.cpp.o: $(BME280_DIR)/%.cpp $(BME280_MKDEPS)
	$(MKDIR) $(dir $@)
	$(CXX) -o "$@" $(ARDUINO_INCLUDES) $(BME280_INCLUDES) -c "$<"

$(BME280_LIB): $(BME280_OBJS) $(BME280_MKDEPS)
	$(AR) rc $(BME280_LIB) $(BME280_OBJS)
	$(RANLIB) $(BME280_LIB)
