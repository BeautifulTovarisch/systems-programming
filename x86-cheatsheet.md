# Assembly Cheatsheet

This note contains some basic information about reading assembly instructions.

## Table

A simple table annotating IA32/x86 registers and their function.

|x86 |IA32|Function     |
|----|----|-------------|
|%rax|%eax|Return value |
|%rbx|%ebx|Callee Saved |
|%rcx|%ecx|4th Argument |
|%rdx|%edx|3rd Argument |
|%rsi|%esi|2nd Argument |
|%rdi|%edi|1st Argument |
|%rbp|%ebp|Callee Saved |
|%rsp|%esp|Stack Pointer|
|%r8 |%r8d|5th Argument |
|%r9 |%r9d|6th Argument |
|%r10|%r10d|Callee Saved|
|%r11|%r11d|Callee Saved|
|%r12|%r13d|Callee Saved|
|%r13|%r14d|Callee Saved|
|%r14|%r15d|Callee Saved|
|%r15|%r16d|Callee Saved|

## Operations

- **R** - Retrieves the value at a register
- **M** - Retrieves the values at a memory address
- **Imm** - A constant value

|Type     |Form            |Operand Value              |Name               |
|---------|----------------|---------------------------|-------------------|
|Immediate|$Imm            |Imm                        |Immediate          |
|Register |r_a             |R[r_a]                     |Register           |
|Memory   |Imm             |M[Imm]                     |Absolute           |
|Memory   |(r_a)           |M[R[r_a]]                  |Indirect           |
|Memory   |Imm(r_b)        |M[Imm + R[r_b]]            |Base + Displacement|
|Memory   |(r_b, r_i)      |M[R[r_b] + R[r_i]]         |Indexed            |
|Memory   |Imm(r_b, r_i)   |M[Imm + R[r_b] + R[r_i]]   |Indexed            |
|Memory   |(,r_i,s)        |M[R[r_i]\*s]               |Scaled Indexed     |
|Memory   |Imm(,r_i,s)     |M[Imm + R[r_i]\*s]         |Scaled Indexed     |
|Memory   |(r_b, r_i, s)   |M[R[r_b] + R[r_i]\*s]      |Scaled Indexed     |
|Memory   |Imm(r_b, r_i, s)|M[Imm + R[r_b] + R[r_i]\*s]|Scaled Indexed     |
