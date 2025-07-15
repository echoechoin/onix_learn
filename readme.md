# build

```bash
# build.
make all

# run in qemu.
make run

# wait for make gdb connection.
make debug

# gdb connection to qemu.
make gdb
```

主要的逻辑包括：
- 全局描述符：在进入保护模式前将内存映射为代码段和数据段，段寄存器使用段选择子
- 分页：将页目录索引基地址放到cr3然后启动分段
- tss：
