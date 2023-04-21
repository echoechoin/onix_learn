
global task_switch
task_switch:
    ; 调用task_switch的时候会将eip压入栈中
    ; 保存栈帧
    push ebp 
    mov ebp, esp

    ; 保存当前任务的寄存器 这里会将当前任务的寄存器保存到task_frame_t结构体中
    push ebx
    push esi
    push edi

    ; 获取当前任务的栈指针
    mov eax, esp;
    ; 获取当前任务的task_t结构体
    and eax, 0xfffff000; current
    ; 保存栈指针到task_t->stack中
    mov [eax], esp

    ; void task_switch(task_t *next);
    ; 这里获取到的是下一个任务的task_t结构体的地址，里面存储了下一个任务的栈顶
    mov eax, [ebp + 8]; next
    ; 将下一个任务的栈指针赋值给esp
    mov esp, [eax]

    ; 因为我们在栈顶保存了task_frame_t结构体，
    ; 所以这里直接pop就可以切换到新的任务的寄存器上下文里了！
    pop edi
    pop esi
    pop ebx
    pop ebp

    ; 从task_switch返回, 因为栈中还有eip的值；
    ; 所以这里的ret指令会直接跳转到新任务的eip处。
    ret

    ; 非常巧妙!!!
