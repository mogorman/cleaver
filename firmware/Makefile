ARDUINO_DIR=~/nonapt/arduino-1.6.3
ARDUINO=$(ARDUINO_DIR)/arduino
AVRDUDE=$(ARDUINO_DIR)/hardware/tools/avr/bin/avrdude
AVR_GCC=$(ARDUINO_DIR)/hardware/tools/avr/bin/avr-gcc
AVR_OBJCOPY=$(ARDUINO_DIR)/hardware/tools/avr/bin/avr-objcopy
PROGRAMMER=buspirate
PROGRAMMER_PORT=/dev/buspirate
ARDUINO_PORT=/dev/arduino

all: check_submodule compile_app


check_submodule:
	@echo ""
ifeq ("$(wildcard ../meatstand_common/README )", "")

	@echo "I didn't have libraries i need to compile cloned."
	@echo "I will get it for you now."
	../update_submodule.sh
	@echo "I got my dependencies I am ready to go"
	@make
endif

compile_app:
ifeq ("$(wildcard ./bin/application.hex )", "")
	@rm -f ./build/app.md5
endif
	@mkdir -p build
	@mkdir -p bin
	@md5sum application/application.ino | cmp -s build/app.md5 -; \
	 if test $$? -ne 0; then \
	  ls build; \
	  cp ./preferences.txt build/; \
	  echo "sketchbook.path="`pwd`"/" >> build/preferences.txt; \
	  $(ARDUINO) \
	   --verbose-build \
	   --preferences-file ./build/preferences.txt \
	   --pref build.path=`pwd`/build \
	   --verify `pwd`/application/application.ino; \
	  echo Ardiono $$? result; \
	  md5sum application/application.ino > build/app.md5; \
	  $(ARDUINO_DIR)/hardware/tools/avr/bin/avr-objcopy -O srec -R .eeprom \
	              build/application.cpp.elf bin/application.hex; \
	fi

app: compile_app
	$(AVRDUDE) -pm328p -c arduino -P $(ARDUINO_PORT) -b9600 -U flash:w:./bin/application.hex:s

clean:
	@rm -rf build
	@rm -rf bin
