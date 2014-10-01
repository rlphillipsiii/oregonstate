PRG            = adc_test

OBJ            = $(PRG).o

MCU_TARGET     = atmega128
OPTIMIZE       = -O2    # options are 1, 2, 3, s
CC             = avr-gcc
LIBS           = ../../alarm_code/lib/adc_driver.o \
					../../alarm_code/lib/led_driver.o \

DEFS           = -I ../../alarm_code/lib

override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf lst text eeprom

led_driver.o: ../../alarm_code/lib/led_driver.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o ../../alarm_code/lib/$@ $^

adc_driver.o: ../../alarm_code/lib/adc_driver.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o ../../alarm_code/lib/$@ $^

$(PRG).elf: $(OBJ) adc_driver.o led_driver.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIB) -o $@ $(OBJ) $(LIBS)

clean: 
	rm -rf *.o $(PRG).elf *.bin *.hex *.srec *.bak  
	rm -rf $(PRG)_eeprom.bin $(PRG)_eeprom.hex $(PRG)_eeprom.srec
	rm -rf *.lst *.map 
	rm -rf ../../alarm_code/lib/spi_driver.o
	rm -rf ../../alarm_code/lib/graph_driver.o
	rm -rf ../../alarm_code/lib/led_driver.o
	rm -rf ../../alarm_code/lib/encoder_driver.o

#setup for for USB programmer
#may need to be changed depending on your programmer
program: $(PRG).hex
	sudo avrdude -c osuisp2 -p m128 -e -U flash:w:$(PRG).hex  -v

lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

# Rules for building the .text rom images

text: hex bin srec

hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images

eeprom: ehex ebin esrec

ehex:  $(PRG)_eeprom.hex
ebin:  $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

%_eeprom.srec: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@
