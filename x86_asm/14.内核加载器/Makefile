NASM = nasm
BXIMAGE = bximage
BOCHS = bochs
DISK_IMG = ../build/master.img
BOCHSRC = ../build/bochsrc

all: boot.bin loader.bin
	@ echo "Copying boot.bin to master.img"
	@ dd if=boot.bin of=$(DISK_IMG) bs=512 count=1 conv=notrunc
	@ echo "Copying loader.bin to master.img"
	@ dd if=loader.bin of=$(DISK_IMG) bs=512 count=4 seek=1 conv=notrunc

boot.bin: boot.asm
	$(NASM) -f bin boot.asm -o boot.bin

loader.bin: loader.asm
	$(NASM) -f bin loader.asm -o loader.bin

PHONY: clean
clean:
	rm -f boot.bin loader.bin

PHONY: run
run:
	$(BOCHS) -q -f $(BOCHSRC)