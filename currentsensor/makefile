# name of this project (used for the output files)
TARGET = firmware
MCU = atmega32u4
CORE_NAME = atmega32u4

# a list of the local project folders containing source
# files relative to this makefile
PROJECT_DIRS = . cfg

# libraries to include in this project,
# remove any you're not using
BASE_LIBRARIES = StdDef Scheduler Timer Dio Arduino	Adc LCD
ARDUINO_LIBRARIES = 				

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

# location of any other header files to include
USER_INCLUDES =	

USER_CFLAGS = -DF_CPU=16000000L -DARDUINO=100 -DUSB_VID=0x2341 -DUSB_PID=0x8036

include ../base/makefile.master
