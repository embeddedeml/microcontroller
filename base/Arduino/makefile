# location of the arduino install folder
ARDUINO_DIR = /home/mepping/Software/arduino

# name of this project (used for the output files)
TARGET = atmega32u4
MCU = atmega32u4

# libraries to include in this project,
# remove any you're not using

#LIBRARIES = EEPROM				\
#			Ethernet			\
#			Ethernet/utility	\
#			Firmata				\
#			LiquidCrystal		\
#			SD					\
#			SD/utility			\
#			Servo				\
#			SoftwareSerial		\
#			SPI					\
#			Stepper				\
#			Wire				\
#			Wire/utility

# target folder location
OUTPUT_DIR = obj

# compiler flags for c and cpp files
CFLAGS = -c -g -Os -Wall -ffunction-sections -fdata-sections -mmcu=$(MCU) -DF_CPU=16000000L -DARDUINO=100 -DUSB_VID=0x2341 -DUSB_PID=0x8036 -DARDUINO_LIB

CPPFLAGS = -fno-exceptions $(CFLAGS)

# linker flags
LINK_FLAGS = -Os -Wl,--gc-sections -mmcu=$(MCU) -L$(OUTPUT_DIR) -lm

# location of various tools
AVRDUDE = $(ARDUINO_DIR)/hardware/tools/avrdude
BIN = $(ARDUINO_DIR)/hardware/tools/avr/bin
CORE_DIR = $(ARDUINO_DIR)/hardware/arduino/cores/arduino
VAR_DIR = $(ARDUINO_DIR)/hardware/arduino/variants/leonardo
LIBS_DIR = $(ARDUINO_DIR)/libraries
GCC = $(BIN)/avr-gcc
AR = $(BIN)/avr-ar
COPY = $(BIN)/avr-objcopy

# set up all the include folders
INCLUDES =	-I. -I$(CORE_DIR) -I$(VAR_DIR)										\
			$(foreach lib,$(LIBRARIES), -I$(ARDUINO_DIR)/libraries/$(lib))

# create a list of all the obj files to create for the core library
CORE_OBJS = $(addprefix $(OUTPUT_DIR)/, $(addsuffix .o, $(notdir $(wildcard $(CORE_DIR)/*.cpp) $(wildcard $(CORE_DIR)/*.c) )))
CORE_LIB = $(OUTPUT_DIR)/$(TARGET).a

# create a list of all the obj files to create for the library folders
LIB_OBJS = $(foreach lib, $(LIBRARIES), $(addprefix $(OUTPUT_DIR)/$(lib)/, $(addsuffix .o, $(notdir $(wildcard $(LIBS_DIR)/$(lib)/*.cpp) $(wildcard $(LIBS_DIR)/$(lib)/*.c)))))

# main build target
.PHONY: all
all: $(CORE_LIB)
	@echo Finished building $<
	@echo
	
# rule to build the core.a library file	
$(CORE_LIB): $(CORE_OBJS) $(LIB_OBJS) 
	@for core_file in $(CORE_OBJS) ; do 		\
		echo Storing $$core_file ;				\
		$(AR) rcs $(CORE_LIB) $$core_file ;		\
	done
	@for lib_file in $(LIB_OBJS) ; do 		\
		echo Storing $$lib_file ;				\
		$(AR) rcs $(CORE_LIB) $$lib_file ;		\
	done
	@echo "C Flags:    $(CFLAGS)" > $(TARGET).build
	@echo "CPP Flags:  $(CPPFLAGS)" >> $(TARGET).build
	@echo "Link Flags: $(LINK_FLAGS)" >> $(TARGET).build

# rule to build the core .cpp files
$(OUTPUT_DIR)/%.cpp.o: $(CORE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(GCC) $(CPPFLAGS) -I$(CORE_DIR) -I$(VAR_DIR) $(INCLUDES) $< -o $@
	
# rule to build the core .c files
$(OUTPUT_DIR)/%.c.o: $(CORE_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(GCC) $(CFLAGS) -I$(CORE_DIR) -I$(VAR_DIR) $(INCLUDES) $< -o $@
	
# rule to build the library source files
.SECONDEXPANSION:
LIB_SRC = $(LIBS_DIR)/$(basename $(subst $(OUTPUT_DIR)/,,$@))
$(LIB_OBJS): $$(LIB_SRC)
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(GCC) $(CPPFLAGS) -I$(CORE_DIR) -I$(VAR_DIR) $(INCLUDES) $< -o $@

clean:
	@echo Cleaning...
	@-rm -f $(CORE_LIB) $(CORE_OBJS) $(LIB_OBJS)

rebuild: clean all

