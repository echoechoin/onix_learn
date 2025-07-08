TARGET = ../target

.PHONY: all clean

all: $(TARGET) $(TARGET)/bootloader.bin

$(TARGET):
	mkdir -p $(TARGET)

$(TARGET)/boot.bin: boot.asm
	nasm -f bin $< -o $@

$(TARGET)/loader.bin: loader.asm
	nasm -f bin $< -o $@

# 初始化5个扇区（2560字节）
# 写第0个扇区boot.bin
# seek=2 从第二个扇区开始 count=4 写入 loader.bin
# 结果如下: boot 0 loader 0 0 0
$(TARGET)/bootloader.bin: $(TARGET)/boot.bin $(TARGET)/loader.bin
	dd if=/dev/zero of=$@ bs=512 count=5
	dd if=$(TARGET)/boot.bin   of=$@ bs=512 count=1 seek=0 conv=notrunc
	dd if=$(TARGET)/loader.bin of=$@ bs=512 count=1 seek=2 conv=notrunc

# -hda 告诉 QEMU 将指定的文件（file）作为虚拟机的第一个 IDE 硬盘
# qemu会将硬盘的第一个扇区加载到0x7c00位置
run: $(TARGET)/bootloader.bin
	qemu-system-i386 -hda $<

clean:
	rm -rf $(TARGET)