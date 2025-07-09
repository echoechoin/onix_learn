import json
import os
import re

def parse_make_output(make_output_file):
    compile_commands = []
    # 正则表达式匹配编译命令（以 x86_64-elf-gcc 开头，包含 -c）
    compile_pattern = re.compile(r'^(.*?x86_64-elf-gcc.*? -c .*?\.c)')

    with open(make_output_file, 'r') as f:
        for line in f:
            line = line.strip()
            match = compile_pattern.match(line)
            if match:
                command = match.group(1)
                # 提取源文件路径
                src_file = re.search(r'(\S+\.c)', command)
                if src_file:
                    src_file = src_file.group(1)
                    # 获取当前工作目录
                    directory = os.getcwd()
                    # 构建 compile_commands.json 条目
                    compile_commands.append({
                        "directory": directory,
                        "command": command,
                        "file": src_file
                    })

    return compile_commands

def write_compile_commands(compile_commands, output_file="compile_commands.json"):
    with open(output_file, 'w') as f:
        json.dump(compile_commands, f, indent=2)

if __name__ == "__main__":
    make_output_file = "make_output.txt"
    compile_commands = parse_make_output(make_output_file)
    write_compile_commands(compile_commands)
