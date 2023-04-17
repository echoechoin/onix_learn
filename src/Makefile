NASM = nasm
BXIMAGE = bximage
BOCHS = bochs
BOCHS_GDB = bochs-gdb
DD = dd
GCC = gcc
QEMU = qemu-system-i386
NM = nm
OBJCOPY = objcopy


MULTIBOOT2 := 0x10000
# 64字节对齐
ENTRYPOINT := $(shell python -c "print(f'0x{$(MULTIBOOT2) + 64:x}')")

BUILD=./build
SRC=.
BOCHSRC = bochsrc
BOCHSRC_GDB = bochsrc.gdb
DISK_IMG = master.img

DEBUG := -g                    # 指定调试模式
CFLAGS := -m32                 # 指定编译器生成32位代码
CFLAGS += -fno-builtin         # 不需要gcc内置函数
CFLAGS += -nostdinc            # 不需要标准头文件
CFLAGS += -fno-pic             # 不需要位置无关的代码
CFLAGS += -fno-pie             # 不需要位置无关的可执行程序
CFLAGS += -nostdlib            # 不需要标准库
CFLAGS += -fno-stack-protector # 不需要栈保护
CFLAGS := $(strip ${CFLAGS})

INCLUDE := -I$(SRC)/include

all:$(BUILD)/${DISK_IMG} $(BUILD)/kernel.iso

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

$(BUILD)/kernel.iso: $(BUILD)/kernel/kernel.bin $(SRC)/utils/grub.cfg
	@# 1. 检测kernel.bin的开始卫视是否有multiboot2头部
	grub-file --is-x86-multiboot2 $<
	
	@# 2. 创建一个iso目录
	mkdir -p $(BUILD)/iso/boot/grub

	@# 3. 拷贝内核文件到iso目录
	cp $< $(BUILD)/iso/boot

	@# 4. 拷贝grub配置文件到iso目录
	cp $(SRC)/utils/grub.cfg $(BUILD)/iso/boot/grub

	@# 生成iso文件
	grub-mkrescue -o $@ $(BUILD)/iso

$(BUILD)/boot/%.bin: $(SRC)/boot/%.asm
	$(shell mkdir -p $(dir $@))
	$(NASM) -f bin -o $@ $<

$(BUILD)/system.bin: $(BUILD)/kernel/kernel.bin
	$(OBJCOPY) -O binary $< $@
	@# 将elf格式的文件转换为二进制文件
	@# 主要是为了将bss段的数据直接写入到文件中，而不是表示留一个标志，因为在这里我们的内核还没有实现执行elf文件的功能

$(BUILD)/system.map: $(BUILD)/kernel/kernel.bin
	$(NM) $< | sort > $@

# -m elf_i386: 指定生成的目标文件格式为elf32
# -static: 静态链接
# -Ttext $(ENTRYPOINT): 指定text段的起始地址
# --section-start=.multiboot2=$(MULTIBOOT2): 指定multiboot2段的起始地址
LDFLAGS := -m elf_i386 \
		-static \
		-Ttext $(ENTRYPOINT) \
		--section-start=.multiboot2=$(MULTIBOOT2)

$(BUILD)/kernel/kernel.bin: $(BUILD)/kernel/start.o \
	$(BUILD)/kernel/main.o \
	$(BUILD)/kernel/io.o \
	$(BUILD)/kernel/console.o \
	$(BUILD)/kernel/printk.o \
	$(BUILD)/kernel/assert.o \
	$(BUILD)/kernel/debug.o \
	$(BUILD)/kernel/global.o \
	$(BUILD)/kernel/task.o \
	$(BUILD)/kernel/schedule.o \
	$(BUILD)/kernel/interrupt.o \
	$(BUILD)/kernel/handler.o \
	$(BUILD)/kernel/clock.o \
	$(BUILD)/kernel/time.o \
	$(BUILD)/kernel/rtc.o \
	$(BUILD)/kernel/memory.o \
	$(BUILD)/kernel/gate.o \
	$(BUILD)/kernel/syscall.o \
	$(BUILD)/kernel/thread.o \
	$(BUILD)/kernel/mutex.o \
	$(BUILD)/kernel/keyboard.o \
	$(BUILD)/lib/string.o \
	$(BUILD)/lib/vsprintf.o \
	$(BUILD)/lib/stdlib.o \
	$(BUILD)/lib/bitmap.o \
	$(BUILD)/lib/list.o \
	$(BUILD)/lib/fifo.o \

	$(shell mkdir -p $(dir $@))
	$(LD) ${LDFLAGS} -o $@ $^

$(BUILD)/kernel/%.o: $(SRC)/kernel/%.asm
	$(shell mkdir -p $(dir $@))
	$(NASM) -f elf32 $(DEBUG) -o $@ $<

$(BUILD)/kernel/%.o: $(SRC)/kernel/%.c
	$(shell mkdir -p $(dir $@))
	$(GCC) $(CFLAGS) $(DEBUG) $(INCLUDE) -c -o $@ $<

$(BUILD)/lib/%.o: $(SRC)/lib/%.c
	$(shell mkdir -p $(dir $@))
	$(GCC) $(CFLAGS) $(DEBUG) $(INCLUDE) -c -o $@ $<

PHONY: bochs
bochs: $(BUILD)/${DISK_IMG}
	$(BOCHS) -q -f $(BOCHSRC)

PHONY: bochsgdb
bochs-gdb: $(BUILD)/${DISK_IMG}
	$(BOCHS_GDB) -q -f $(BOCHSRC_GDB)

# -m 32M: 内存大小为32M
# -boot c: 从第一个硬盘启动
# -hda $<: 指定硬盘镜像文件
# -rtc base=localtime 设置时间为本地时间（UTC+8）
QEMU := $(QEMU) -audiodev pa,id=hda -machine pcspk-audiodev=hda -rtc base=localtime
QEMU_DISK := -boot c -drive file=$(BUILD)/master.img,if=ide,index=0,media=disk,format=raw
QEMU_CDROM := -boot d -drive file=$(BUILD)/kernel.iso,media=cdrom
QEMU_DEBUG := -s -S


PHONY: qemu
qemu: $(BUILD)/${DISK_IMG}
	$(QEMU) $(QEMU_DISK)

PHONY: qemu-gdb
qemu-gdb: $(BUILD)/${DISK_IMG}
	@# -s: 开启gdb调试。监听1234端口
	@# -S: 暂停虚拟机，等待gdb连接
	$(QEMU) $(QEMU_DISK) $(QEMU_DEBUG)

PHONY: qemu-cdrom
qemu-cdrom: $(BUILD)/kernel.iso
	$(QEMU) $(QEMU_CDROM)



PHONY: qemu-cdrom-gdb
qemu-cdrom-gdb: $(BUILD)/kernel.iso
	$(QEMU) $(QEMU_CDROM) $(QEMU_DEBUG)


$(BUILD)/master.vmdk: $(BUILD)/${DISK_IMG}
	qemu-img convert -pO vmdk $< $@

PHONY: vmdk
vmdk: $(BUILD)/master.vmdk


PHONY: clean
clean:
	rm -rf $(BUILD)


