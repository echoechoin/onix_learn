# gdb_script.gdb
file ../target/kernel.bin
target remote localhost:1234
break print
continue