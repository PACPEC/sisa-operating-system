AS = sisa-as
CC = sisa-gcc
LD = sisa-ld

CFLAGS = -g3 -O0 -Wall
LDFLAGS = -T system.lds

OBJECTS = entry.o kernel.o

.PHONY: all
all: kernel.code.DE1.hex kernel.data.DE1.hex

kernel: $(OBJECTS) system.lds
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

%.code: %
	sisa-objdump -d --section=.system $^ > $@

%.code.rom %.code.DE1.hex %.code.DE2-115.hex: %.code
	./limpiar.pl codigo $^

%.data: %
	sisa-objdump -d --section=.sysdata$^ > $@

%.data.rom %.data.DE1.hex %.data.DE2-115.hex: %.data
	./limpiar.pl datos $^

.PHONY: run
run: kernel.data.DE1.hex kernel.code.DE1.hex
	sisa-emu -v -l addr=0x8000,file=kernel.data.rom -l addr=0xC000,file=kernel.code.rom

.PHONY: clean
clean:
	rm -vf *.o *.hex *.rom *.code *.data kernel
