PRG            = main

OBJ            = $(PRG).o

MCU_TARGET     = atmega128
OPTIMIZE       = -O2    # options are 1, 2, 3, s
CC             = avr-gcc
LIBS           = lib/led_driver.o \
					lib/button_driver.o \
					lib/spi_driver.o \
					lib/graph_driver.o \
					lib/encoder_driver.o \
					lib/lcd_driver.o \
					lib/adc_driver.o \
					lib/clock.o \
					lib/twi_master.o \
					lib/thermo_driver.o \
					lib/uart_driver.o \
					lib/si4734_driver.o

DEPS 		   = encoder_driver.o \
					led_driver.o \
					button_driver.o \
					spi_driver.o \
					graph_driver.o \
					clock.o \
					adc_driver.o \
					lcd_driver.o \
					twi_master.o \
					thermo_driver.o \
					uart_driver.o \
					si4734_driver.o

DEFS           = -I lib
F_CPU          = 16000000L

override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -DF_CPU=$(F_CPU) -DMEGA128=1
override LDFLAGS       = -Wl,-Map,$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf lst text eeprom

%.o: lib/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o lib/$@ $^

$(PRG).elf: $(OBJ) $(DEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIB) -o $@ $(OBJ) $(LIBS)

clean: 
	rm -rf *.o $(PRG).elf *.bin *.hex *.srec *.bak  
	rm -rf $(PRG)_eeprom.bin $(PRG)_eeprom.hex $(PRG)_eeprom.srec
	rm -rf *.lst *.map 
	rm -rf lib/*.o

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
