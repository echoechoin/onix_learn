
global task_switch
task_switch:
    ; 保存栈帧 task_frame
    push ebp
    mov ebp, esp

    ; 保存上下文
    push ebx
    push esi
    push edi

    mov eax, esp;
    and eax, 0xfffff000; 获取到当前task的基地址
    mov [eax], esp;      将 ESP 的值存储到task的基地址中(task->stack)

    mov eax, [ebp + 8];  获取下一个task
    mov esp, [eax];      将task->stack 赋值给esp

    ; 恢复栈帧 task_frame
    pop edi
    pop esi
    pop ebx
    pop ebp

    ; 恢复eip
    ret
