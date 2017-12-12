RTCLIB_DIR      :=$(LIB_DIR)/RTClib
RTCLIB_BUILD    :=$(BUILD_LIB_DIR)/RTClib
# RTCLIB_MKDEPS   :=Makefile lib/RTClib.mk
RTCLIB_INCLUDES :=-I $(RTCLIB_DIR)

RTCLIB_CPP_SRCS :=$(RTCLIB_DIR)/RTClib.cpp
RTCLIB_CPP_OBJS :=$(patsubst $(RTCLIB_DIR)/%.cpp,$(RTCLIB_BUILD)/%.cpp.o,$(RTCLIB_CPP_SRCS))
RTCLIB_OBJS     :=$(RTCLIB_CPP_OBJS)
RTCLIB_LIB      :=$(BUILD_LIB_DIR)/libRTClib.a

.PHONY: rtclib_config rtclib_build rtclib_clean

rtclib_config:
	@echo $$\(RTCLIB_DIR\)=$(RTCLIB_DIR)
	@echo $$\(RTCLIB_INCLUDES\)=$(RTCLIB_INCLUDES)
	@echo $$\(RTCLIB_CPP_SRCS\)=$(RTCLIB_CPP_SRCS)
	@echo $$\(RTCLIB_CPP_OBJS\)=$(RTCLIB_CPP_OBJS)
	@echo $$\(RTCLIB_OBJS\)=$(RTCLIB_OBJS)
	@echo $$\(RTCLIB_LIB\)=$(RTCLIB_LIB)

rtclib_build: $(RTCLIB_LIB)

rtclib_clean:
	$(RM) $(RTCLIB_OBJS) $(RTCLIB_LIB)

$(RTCLIB_BUILD)/%.cpp.o: $(RTCLIB_DIR)/%.cpp $(RTCLIB_MKDEPS)
	$(MKDIR) $(dir $@)
	$(CXX) -o "$@" $(ARDUINO_INCLUDES) $(RTCLIB_INCLUDES) -c "$<"

$(RTCLIB_LIB): $(RTCLIB_OBJS) $(RTCLIB_MKDEPS)
	$(AR) rc $(RTCLIB_LIB) $(RTCLIB_OBJS)
	$(RANLIB) $(RTCLIB_LIB)
