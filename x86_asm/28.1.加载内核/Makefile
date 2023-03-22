NASM = nasm
BXIMAGE = bximage
BOCHS = bochs
DISK_IMG = ../build/master.img
BOCHSRC = ../build/bochsrc

CFLAGS := -m32 \
	 -fno-asynchronous-unwind-tables \
	 -fno-pic \
	 -mpreferred-stack-boundary=2 \
	 -fno-builtin \
	 -fno-stack-protector \
	 -O0
CFLAGS := $(CFLAGS) -nostartfiles
CFLAGS := $(CFLAGS) -nostdlib
CFLAGS := $(CFLAGS) -nostdinc
CFLAGS := $(CFLAGS) -fno-pie

all: boot.bin loader.bin kernel.bin
	@ rm -rf $(DISK_IMG)
	@ $(BXIMAGE) -q -hd=16 -func=create -sectsize=512 -imgmode=flat $(DISK_IMG)
	@ echo "Copying boot.bin to master.img"
	@ dd if=boot.bin of=$(DISK_IMG) bs=512 count=1 conv=notrunc
	@ echo "Copying loader.bin to master.img"
	@ dd if=loader.bin of=$(DISK_IMG) bs=512 count=4 seek=1 conv=notrunc
	@ echo "Copying kernel.bin to master.img"
	@ dd if=kernel.bin of=$(DISK_IMG) bs=512 count=100 seek=10 conv=notrunc

boot.bin: boot.asm
	$(NASM) -f bin boot.asm -o boot.bin

loader.bin: loader.asm
	$(NASM) -f bin loader.asm -o loader.bin

ENTRY_POINT := 0x11000
# 编译后的kernel.bin使用readelf -e查看，发现入口地址是0x11000，但是elf的头部中的入口地址是0x10000
# 因此需要将kernel.bin从磁盘中复制到内存中的0x10000处，然后跳转到0x11000处执行

kernel.bin: start.o main.o
	ld -m elf_i386 -static $^ -o $@ -Ttext $(ENTRY_POINT) 

main.o: main.c
	gcc -c main.c -o main.o $(CFLAGS)

start.o: start.asm
	$(NASM) -f elf32 start.asm -o start.o

PHONY: clean
clean:
	rm -f boot.bin loader.bin kernel.bin start.o main.o

PHONY: run
run:
	$(BOCHS) -q -f $(BOCHSRC)