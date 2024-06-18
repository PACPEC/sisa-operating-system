AS = sisa-as
CC = sisa-gcc
LD = sisa-ld

CFLAGS = -g3 -O0 -Wall --std=c99
LDFLAGS = -T system.lds

OBJECTS += $(patsubst %.c,%.o,$(wildcard *.c))
OBJECTS += $(patsubst %.S,%.o,$(wildcard *.S))
OBJECTS += $(patsubst %.c,%.o,$(wildcard programs/*.c))

.PHONY: all
all: kernel.code.DE1.hex kernel.data.DE1.hex kernel.usercode.DE1.hex kernel.userdata.DE1.hex

kernel: $(OBJECTS) system.lds
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

%.code: %
	sisa-objdump -d --section=.system $^ > $@

%.code.rom %.code.DE1.hex %.code.DE2-115.hex: %.code
	./limpiar.pl codigo $^

%.data: %
	sisa-objdump -z -D --section=.sysdata $^ > $@

%.data.rom %.data.DE1.hex %.data.DE2-115.hex: %.data
	# limpiar.pl falla si hi ha molts zeros sintetitzats a la sortida de sisa-objdump
	# substituït per commanda awk + grep directament, molt més senzilla
	# ./limpiar.pl datos $^
	awk '{print $$2}' $^ | grep '^[0-9a-f][0-9a-f]*$$' >$@

%.usercode: %
	sisa-objdump -d --section=.usercode $^ > $@

%.usercode.rom %.usercode.DE1.hex %.usercode.DE2-115.hex: %.usercode
	./limpiar.pl codigo $^

%.userdata: %
	sisa-objdump -z -D --section=.userdata $^ > $@

%.userdata.rom %.userdata.DE1.hex %.userdata.DE2-115.hex: %.userdata
	# ./limpiar.pl datos $^
	awk '{print $$2}' $^ | grep '^[0-9a-f][0-9a-f]*$$' >$@

.PHONY: run
run: kernel.data.rom kernel.code.rom kernel.usercode.rom kernel.userdata.rom
	sisa-emu -s 2500 -v -e -l addr=0x8000,file=kernel.data.rom -l addr=0xC000,file=kernel.code.rom -l addr=0x0000,file=kernel.usercode.rom -l addr=0x4000,file=kernel.userdata.rom

.PHONY: run_novga
run_novga: kernel.data.rom kernel.code.rom kernel.usercode.rom kernel.userdata.rom
	sisa-emu -s 1 -e -l addr=0x8000,file=kernel.data.rom -l addr=0xC000,file=kernel.code.rom -l addr=0x0000,file=kernel.usercode.rom -l addr=0x4000,file=kernel.userdata.rom

.PHONY: clean
clean:
	rm -vf $(OBJECTS) *.hex *.rom *.code *.data kernel
