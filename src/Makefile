# 获取系统类型
UNAME_S := $(shell uname -s)

CC      = gcc
LD      = ld
OBJCOPY = objcopy
NM      = nm
NASM    = nasm
QEMU    = qemu-system-i386
DD      = dd

# 判断是否为macOS并设置编译器
ifeq ($(UNAME_S), Darwin)
    CC      = /opt/homebrew/Cellar/x86_64-elf-gcc/15.1.0/bin/x86_64-elf-gcc
	LD      = /opt/homebrew/Cellar/x86_64-elf-binutils/2.44/bin/x86_64-elf-ld
	OBJCOPY = /opt/homebrew/Cellar/x86_64-elf-binutils/2.44/bin/x86_64-elf-objcopy
	NM      = /opt/homebrew/Cellar/x86_64-elf-binutils/2.44/bin/x86_64-elf-nm
	NASM    = /opt/homebrew/bin/nasm
	QEMU    = /opt/homebrew/Cellar/qemu/10.0.0/bin/qemu-system-i386
endif

ENTRYPOINT:=0x10000

CFLAGS:= -m32 # 32 位的程序
CFLAGS+= -fno-builtin	# 不需要 gcc 内置函数
CFLAGS+= -nostdinc		# 不需要标准头文件
CFLAGS+= -fno-pic		# 不需要位置无关的代码  position independent code
CFLAGS+= -fno-pie		# 不需要位置无关的可执行程序 position independent executable
CFLAGS+= -nostdlib		# 不需要标准库
CFLAGS+= -fno-stack-protector	# 不需要栈保护
CFLAGS:=$(strip ${CFLAGS})

DEBUG:= -g -O0
INCLUDE:=-I./include

TARGET = ../target
BOOTLOADER = bootloader
KERNEL = kernel
LIB = lib

.PHONY: all clean

all: ${TARGET}/image.bin

# -f bin 生成纯二进制文件
$(TARGET)/bootloader/%.bin: $(BOOTLOADER)/%.asm
	@$(shell mkdir -p $(dir $@))
	$(NASM) -f bin $< -o $@

# -f elf32 生成 ELF（Executable and Linkable Format） 格式的目标文件，32 位版本。
# 包含文件头、段表、重定位信息等元素，因此支持ld命令链接
$(TARGET)/kernel/%.o: $(KERNEL)/%.asm
	@$(shell mkdir -p $(dir $@))
	$(NASM) -f elf32 $(DEBUG) $< -o $@

$(TARGET)/kernel/%.o: $(KERNEL)/%.c
	@$(shell mkdir -p $(dir $@))
	$(CC) $(CFLAGS) $(DEBUG) $(INCLUDE) -c $< -o $@

$(TARGET)/lib/%.o: $(LIB)/%.c
	@$(shell mkdir -p $(dir $@))
	$(CC) $(CFLAGS) $(DEBUG) $(INCLUDE) -c $< -o $@

# -m elf_i386 指定生成的目标文件的目标体系结构
# -Ttext $(ENTRYPOINT) 用于指定程序的 代码段（.text 段）的起始地址。
$(TARGET)/kernel.bin: \
     $(TARGET)/kernel/start.o \
	 $(TARGET)/kernel/memory.o \
	 $(TARGET)/kernel/main.o \
	 $(TARGET)/kernel/io.o \
	 $(TARGET)/kernel/console.o \
	 $(TARGET)/kernel/printk.o \
	 $(TARGET)/kernel/assert.o \
	 $(TARGET)/kernel/debug.o \
	 $(TARGET)/kernel/global.o \
	 $(TARGET)/kernel/task.o \
	 $(TARGET)/kernel/schedule.o \
	 $(TARGET)/kernel/handler.o \
	 $(TARGET)/kernel/interrupt.o \
	 $(TARGET)/kernel/clock.o \
	 $(TARGET)/kernel/rtc.o \
	 $(TARGET)/kernel/time.o \
	 $(TARGET)/lib/string.o \
	 $(TARGET)/lib/stdlib.o \
	 $(TARGET)/lib/vsprintf.o \

	$(shell mkdir -p $(dir $@))
	$(LD) -m elf_i386 -static $^ -o $@ -T linker.ld

# -O binary 将 elf 格式的文件转换为纯二进制文件
$(TARGET)/system.bin: $(TARGET)/kernel.bin
	$(OBJCOPY) -O binary $< $@

$(TARGET)/system.map: $(TARGET)/kernel.bin
	$(NM) $< | sort > $@

# 初始化5个扇区（2560字节）
# 写第0个扇区boot.bin
# seek=2 从第二个扇区开始 count=4 写入 loader.bin
# 结果如下: boot(1) empty(1) loader(1) system(n)
$(TARGET)/image.bin: \
	 $(TARGET)/bootloader/boot.bin \
	 $(TARGET)/bootloader/loader.bin \
	 $(TARGET)/system.bin \
	 $(TARGET)/system.map \

	$(DD) if=/dev/zero of=$@ bs=512 count=256
	$(DD) if=$(TARGET)/bootloader/boot.bin   of=$@ bs=512 count=1 seek=0 conv=notrunc
	$(DD) if=$(TARGET)/bootloader/loader.bin of=$@ bs=512 count=1 seek=2 conv=notrunc
	$(DD) if=$(TARGET)/system.bin of=$@ bs=512 count=200 seek=3 conv=notrunc

# -hda 告诉 QEMU 将指定的文件（file）作为虚拟机的第一个 IDE 硬盘
# qemu会将硬盘的第一个扇区加载到0x7c00位置
run: $(TARGET)/image.bin
	$(QEMU) -m 16M -hda $(TARGET)/image.bin

# -S 表示阻塞等待 gdb 执行 continue
debug: $(TARGET)/image.bin
	$(QEMU) -m 128M -gdb tcp::1234 -hda $(TARGET)/image.bin -S

gdb:
	gdb -x gdb_script.gdb

clean:
	rm -rf $(TARGET)
