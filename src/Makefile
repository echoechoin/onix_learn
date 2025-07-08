TARGET = ../target

.PHONY: all clean

all: $(TARGET) $(TARGET)/boot.bin

$(TARGET):
	@mkdir -p $(TARGET)

$(TARGET)/boot.bin: boot.asm
	nasm -f bin boot.asm -o $(TARGET)/boot.bin

# -hda 告诉 QEMU 将指定的文件（file）作为虚拟机的第一个 IDE 硬盘
# qemu会将硬盘的第一个扇区加载到0x7c00位置
run: $(TARGET)/boot.bin
	qemu-system-i386 -hda $(TARGET)/boot.bin

clean:
	rm -rf $(TARGET)