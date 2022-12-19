NASM = nasm
BXIMAGE = bximage
BOCHS = bochs
BOCHS_GDB = bochs-gdb
DD = dd
GCC = gcc
QEMU = qemu-system-i386
NM = nm
OBJCOPY = objcopy

ENTRYPOINT = 0x10000
BUILD=./build
SRC=.
BOCHSRC = bochsrc
BOCHSRC_GDB = bochsrc.gdb
DISK_IMG = master.img

DEBUG := -g
CFLAGS := -m32
CFLAGS += -fno-builtin # 不需要gcc内置函数
CFLAGS += -nostdinc    # 不需要标准头文件
CFLAGS += -fno-pic     # 不需要位置无关的代码
CFLAGS += -fno-pie     # 不需要位置无关的可执行程序
CFLAGS += -nostdlib    # 不需要标准库
CFLAGS += -fno-stack-protector # 不需要栈保护
CFLAGS := $(strip ${CFLAGS})

INCLUDE := -I$(SRC)/include

all:$(BUILD)/${DISK_IMG}

$(BUILD)/$(DISK_IMG): $(BUILD)/boot/boot.bin \
	$(BUILD)/boot/loader.bin \
	$(BUILD)/system.bin \
	$(BUILD)/system.map

	@# -q: 安静模式
	@# -hd: 创建一个硬盘， -fd表示创建一个软盘
	@# -func: 表示的是使用创建功能
	@# -sectsize: 每个扇区的大小
	@# -imgmode: 硬盘的格式，flat表示扁平格式。
	
	yes | $(BXIMAGE) -q -hd=16 -func=create -sectsize=512 -imgmode=flat $@
	$(DD) if=$(BUILD)/boot/boot.bin of=$@ bs=512 count=1 conv=notrunc
	$(DD) if=$(BUILD)/boot/loader.bin of=$@ bs=512 count=4 seek=1 conv=notrunc
	$(DD) if=$(BUILD)/system.bin of=$@ bs=512 count=200 seek=10 conv=notrunc

$(BUILD)/boot/%.bin: $(SRC)/boot/%.asm
	$(shell mkdir -p $(dir $@))
	$(NASM) -f bin -o $@ $<

$(BUILD)/system.bin: $(BUILD)/kernel/kernel.bin
	$(OBJCOPY) -O binary $< $@
	@# 将elf格式的文件转换为二进制文件
	@# 主要是为了将bss段的数据直接写入到文件中，而不是表留一个标志，因为在这里我们的内核还没有实现执行elf文件的功能

$(BUILD)/system.map: $(BUILD)/kernel/kernel.bin
	$(NM) $< | sort > $@

$(BUILD)/kernel/kernel.bin: $(BUILD)/kernel/start.o \
	$(BUILD)/kernel/main.o \
	$(BUILD)/kernel/io.o

	$(shell mkdir -p $(dir $@))
	$(LD) -m elf_i386 -static -Ttext $(ENTRYPOINT) -o $@ $^

$(BUILD)/kernel/%.o: $(SRC)/kernel/%.asm
	$(shell mkdir -p $(dir $@))
	$(NASM) -f elf32 $(DEBUG) -o $@ $<

$(BUILD)/kernel/%.o: $(SRC)/kernel/%.c
	$(shell mkdir -p $(dir $@))
	$(GCC) $(CFLAGS) $(DEBUG) $(INCLUDE) -c -o $@ $<


PHONY: bochs
bochs: $(BUILD)/${DISK_IMG}
	$(BOCHS) -q -f $(BOCHSRC)

PHONY: bochs-gdb
bochs-gdb: $(BUILD)/${DISK_IMG}
	$(BOCHS_GDB) -q -f $(BOCHSRC_GDB)

PHONY: qemu
qemu: $(BUILD)/${DISK_IMG}
	@# -m 32M: 内存大小为32M
	@# -boot c: 从第一个硬盘启动
	@# -hda $<: 指定硬盘镜像文件

	$(QEMU) -m 32M -boot c -hda $<

PHONY: qemu-gdb
qemu-gdb: $(BUILD)/${DISK_IMG}
	@# -s: 开启gdb调试。监听1234端口
	@# -S: 暂停虚拟机，等待gdb连接

	$(QEMU) -m 32M -boot c -hda $< -s -S


$(BUILD)/master.vmdk: $(BUILD)/${DISK_IMG}
	qemu-img convert -pO vmdk $< $@

PHONY: vmdk
vmdk: $(BUILD)/master.vmdk


PHONY: clean
clean:
	rm -rf $(BUILD)


