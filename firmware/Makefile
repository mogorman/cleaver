ARDUINO_DIR=~/nonapt/arduino-1.6.3
ARDUINO=$(ARDUINO_DIR)/arduino
AVRDUDE=/usr/bin/avrdude
AVR_GCC=$(ARDUINO_DIR)/hardware/tools/avr/bin/avr-gcc
AVR_OBJCOPY=$(ARDUINO_DIR)/hardware/tools/avr/bin/avr-objcopy
PROGRAMMER=buspirate
PROGRAMMER_PORT=/dev/buspirate
ARDUINO_PORT=/dev/arduino
GIT=`git rev-parse --short HEAD`
HEXTIME=`printf "%x\n" $(shell date +%s)`

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
	@mkdir -p build/application
	@mkdir -p build/build
	@mkdir -p bin
	@md5sum application/application.ino | cmp -s build/app.md5 -; \
	 if test $$? -ne 0; then \
	  ls build; \
	  cp ./preferences.txt build/; \
	  echo "sketchbook.path="`pwd`"/" >> build/preferences.txt; \
	  echo "#define GIT \""$(GIT)"\"\n#define TIME \""$(HEXTIME)"\"" | cat -  application/application.ino > build/application/application.ino; \
	  xvfb-run $(ARDUINO) \
	   --verbose-build \
	   --preferences-file ./build/preferences.txt \
	   --pref build.path=`pwd`/build/build \
	   --verify `pwd`/build/application/application.ino; \
	md5sum application/application.ino > build/app.md5; \
	$(AVR_OBJCOPY) -O srec -R .eeprom build/build/application.cpp.elf bin/application.hex; \
	fi

app: compile_app
ifeq (buspirate,$(PROGRAMMER))
		@echo "#" > $(PROGRAMMER_PORT)
		@echo -ne "$(printf '\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x0F')" > $(PROGRAMMER_PORT)
endif
	$(AVRDUDE) -pt85 -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) -U flash:w:./bin/application.hex:s


on:
ifeq (buspirate,$(PROGRAMMER))
	@echo "m2" > $(PROGRAMMER_PORT)
	@echo "W" > $(PROGRAMMER_PORT)
endif

off:
ifeq (buspirate,$(PROGRAMMER))
	@echo "#" > $(PROGRAMMER_PORT)
	@echo -ne "$(printf '\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x0F')" > $(PROGRAMMER_PORT)
	@echo "C" > $(PROGRAMMER_PORT)
	@echo "a" > $(PROGRAMMER_PORT)
	@sleep 1
	@echo "@" > $(PROGRAMMER_PORT)
	@echo "#" > $(PROGRAMMER_PORT)
endif

fuses:
	echo $(GIT)
#	$(AVRDUDE) -pt85 -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m 

clean:
	@rm -rf build
	@rm -rf bin
