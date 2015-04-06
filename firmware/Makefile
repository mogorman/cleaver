ARDUINO_DIR=~/nonapt/arduino-1.6.0
ARDUINO=$(ARDUINO_DIR)/arduino
AVRDUDE=/usr/bin/avrdude
AVR_GCC=/usr/bin/avr-gcc
AVR_OBJCOPY=/usr/bin/avr-objcopy
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
	  xvfb-run -a $(ARDUINO) \
	   --verbose-build \
	   --pref target_platform=avr \
	   --pref build.path=`pwd`/build \
	   --pref sketchbook.path=`pwd` \
	   --verify `pwd`/application/application.ino; \
	  md5sum application/application.ino > build/app.md5; \
	  $(ARDUINO_DIR)/hardware/tools/avr/bin/avr-objcopy -O srec -R .eeprom \
	              build/application.cpp.elf bin/application.hex; \
	fi

app: compile_app
	$(AVRDUDE) -pm328p -c arduino -P $(ARDUINO_PORT) -b9600 -U flash:w:./bin/application.hex:s

clean:
	@rm -rf build
	@rm -rf bin
