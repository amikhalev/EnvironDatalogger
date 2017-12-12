SDFAT_DIR      :=$(LIB_DIR)/SDFat
SDFAT_BUILD    :=$(BUILD_LIB_DIR)/SDFat
# SDFAT_MKDEPS   :=Makefile lib/SDFat.mk
SDFAT_INCLUDES :=-I $(SDFAT_DIR)/src

SDFAT_CPP_SRCS :=$(wildcard $(SDFAT_DIR)/src/*.cpp) $(wildcard $(SDFAT_DIR)/src/SdCard/*.cpp) $(wildcard $(SDFAT_DIR)/src/FatLib/*.cpp)
SDFAT_CPP_OBJS :=$(patsubst $(SDFAT_DIR)/%.cpp,$(SDFAT_BUILD)/%.cpp.o,$(SDFAT_CPP_SRCS))
SDFAT_OBJS     :=$(SDFAT_CPP_OBJS)
SDFAT_LIB      :=$(BUILD_LIB_DIR)/libSDFat.a

.PHONY: sdfat_config sdfat_build sdfat_clean

sdfat_config:
	@echo $$\(SDFAT_DIR\)=$(SDFAT_DIR)
	@echo $$\(SDFAT_INCLUDES\)=$(SDFAT_INCLUDES)
	@echo $$\(SDFAT_CPP_SRCS\)=$(SDFAT_CPP_SRCS)
	@echo $$\(SDFAT_CPP_OBJS\)=$(SDFAT_CPP_OBJS)
	@echo $$\(SDFAT_OBJS\)=$(SDFAT_OBJS)
	@echo $$\(SDFAT_LIB\)=$(SDFAT_LIB)

sdfat_build: $(SDFAT_LIB)

sdfat_clean:
	$(RM) $(SDFAT_OBJS) $(SDFAT_LIB)

$(SDFAT_BUILD)/%.cpp.o: $(SDFAT_DIR)/%.cpp $(SDFAT_MKDEPS)
	$(MKDIR) $(dir $@)
	$(CXX) -o "$@" $(ARDUINO_INCLUDES) $(SDFAT_INCLUDES) -c "$<"

$(SDFAT_LIB): $(SDFAT_OBJS) $(SDFAT_MKDEPS)
	$(AR) rc $(SDFAT_LIB) $(SDFAT_OBJS)
	$(RANLIB) $(SDFAT_LIB)
