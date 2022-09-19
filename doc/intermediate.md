# ++C Intermediate Lang (PPCIL 1.0)

This is a read-only binary format, used in the ++C ecosystem to represent executable code. It has an assembly-like structure (opcode, followed by operands), so that a translation to a native instruction set can be more robust. Still, this IL's instruction set implements instructions, not found in some instruction sets. The idea behind that decision is to use native optimizations where possible (for example, using the square root instruction of the x86_64 instruction set, instead of a common algorithm for all instruction sets). One important thing to note is that PPCIL is not just an itermediate for ++C. A compiler could be written that converts any language to PPCIL, for example: Java, C#, C++, C, etc. 

## Format

The format of a PPCIL blob is the following:

```c
struct ppcil_t {
    version_t version;
    uint64_t consts_size;
    uint8_t consts[consts_size];
    uint64_t instructions_n;
    uint8_t instructions[instructions_n];
}
```

### Version section

The first thing in a PPCIL blob is the version. The version format is identical with [the one](./versions.md) used ++C ecosystem. It is the first 64-bits in a PPCIL blob. It is used to specify the PPCIL format version used in the blob. An interpreter with an incompatible version can try to interpret the given PPCIL, but should give a warning if it will do so.

### Constants section

This section is immediately after the version section. It is used to contain the constants, used by the code. The first part of it is an unsigned QWORDs integer, indicating the amount of bytes, following it, used to keep the constant data, used by the code.

### Code section

This section is immediately after the constants section. It is used to contain the actual code. The first part of it is an unsigned QWORDs integer, indicating the count of instructions, following it. The code itself is an array of instructions (unsigned 8-bit integers).

## The stack and the heap

The memory layout of a PPCIL interpreter is the usual stack/heap layout, with the only differnce that the stack is a part of the heap, so addresses from the stack can be obtained, like in the heap (if you work with the stack via pointers, all stack guarantees are overriden).

The stack contains the variables and any temporary calculation data. The heap contains all the constants, static variables and dynamically allocated memory.

## Variables and parameters

The variables are contained on the stack and are used to contain temporary data druing the execution of the program. Parameters are the first n variables, that are automatically created and initialized with values by the interpreter. The parameters are passed by the caller of the piece of code. The parameters are the first n variables (where n is the amount of parameters that the function accepts). In some cases, the first variable (parameter) is the `this` context.

## Instructions set

The instructions in PPCIL are stack-based. This is so to increase parsing speed of the intermediate language and to bring ++C and PPCIL closer. An instruction consists of an opcode, which is an unsigned 8-bit integer. The following table contains all the operators in PPCIL:

**NOTE:** the operand notation is as follows: `size(name)`, so a 64-bit operand named count would be notated as follows: `8(count)`

### Commons

| Name  | Opcode | Operands | Description 
|-------|--------|----------|-------------
| NOP   | 0x00   |          | Does nothing
| PAD   | 0x01   |          | Pops two variables: a, b and if b < sizeof a, the last sizeof a - b bytes get trimmed from a. Else, b - sizeof a bytes get added at the end of a. The result is pushed
| PADC  | 0x02   | 8(n)     | Executes `PAD` with b = n
| EOF   | 0xFF   |          | Ends execution

### Arithmetics (operandless)

| Name  | Opcode | Description 
|-------|--------|-------------
| ADD   | 0x10   | Pops two variables, adds them and pushes the result
| SUB   | 0x11   | Pops two variables, subtracts them and pushes the result
| NEG   | 0x12   | Flips the sign of the top variable in the stack
| MUL   | 0x13   | Pops two variables, multiplies them and pushes the result
| IMUL  | 0x14   | Pops two variables, multiplies them (signed) and pushes the result
| DIV   | 0x15   | Pops two variables, divides them and pushes the remainder, then the result
| IDIV  | 0x16   | Pops two variables, divides them (signed) and pushes the remainder, then the result
| AND   | 0x17   | Pops two variables, ANDs (a & b) them and pushes the remainder, then the result
| OR    | 0x18   | Pops two variables, ORs (a | b) them and pushes the remainder, then the result
| XOR   | 0x19   | Pops two variables, XORs (a ^ b) them and pushes the remainder, then the result
| NOT   | 0x1A   | Flips the bits of the top variable
| SL    | 0x1B   | Pops two variables: a (first popped), b and performs the operation `a >> b`. Pushes the result back on the stack
| SR    | 0x1C   | Pops two variables: a (first popped), b and performs the operation `a << b`. Pushes the result back on the stack
| INC   | 0x1D   | Adds one to the top variable
| DEC   | 0x1E   | Subtracts one from the top variable

**NOTE:** All operations pick the size of the bigger variable, and do not expand it if an overflow occurs.

### Float arithmetics (operandless)

| Name  | Opcode | Description
|-------|--------|-------------
| ADDF  | 0x20   | Pops two variables, adds them as floats, and pushes the result
| SUBF  | 0x21   | Pops two variables, subtracts them as floats, and pushes the result
| MULF  | 0x23   | Pops two variables, multiplies them as floats, and pushes the result
| DIVF  | 0x24   | Pops two variables, divides them as floats, and pushes the result
| SQRT  | 0x25   | Pops a variable and pushes its square root
| ITOF  | 0x26   | Converts the top variable from a signed int to a 64-bit float
| FTOI  | 0x27   | Converts the top variable from a signed float to an int
| F32   | 0x28   | Converts the top variable from a 64-bit float to a 32-bit float
| F64   | 0x29   | Converts the top variable from a 32-bit float to a 64-bit float (first `PAD 4` is executed)

**NOTE:** All instructions with floats first execute `PAD 8`, unless specified otherwise

### Stack operations

| Name  | Opcode | Operands | Description
|-------|--------|----------|-------------
| PUSH  | 0x30   | 8(n)     | Allocates n bytes on the stack
| PUSHC | 0x31   | 8(size), size(val) | Pushes val to the stack
| PUSHA | 0x32   | 8(size), size(val) | Pushes val to the stack
| LOAD  | 0x33   |          | Pops a variable = a and pushes `*a`
| LOADC | 0x34   | 8(addr)  | Executes `LOAD` with a = addr
| PUSHW | 0x35   | 2(val)   | Executes `PUSHC 2 val`
| PUSHD | 0x36   | 4(val)   | Executes `PUSHC 4 val`
| PUSHQ | 0x37   | 8(val)   | Executes `PUSHC 8 val`
| VADDR | 0x38   | 8(n)     | Pushes the addres of the variable with index n
| CADDR | 0x39   | 8(n)     | Calculates `n + consts`, where `consts` is the start of the constants blob and pushes that address
| DUP   | 0x3A   |          | Duplicates the top variable in the stack
| POP   | 0x3B   |          | Pops the last allocated batch of memory from the stack
| MOVA  | 0x3C   |          | Pops two variables: a, b and executes `*b = a`
| MOVAC | 0x3D   | 8(addr)  | Executes `SAVE` with b = addr
| MOVV  | 0x3E   | 8(var)   | Executes `SAVE` with b = address of var

### Function operations

| Name  | Opcode | Operands | Description
|-------|--------|----------|-------------
| CALL  | 0x40   | 8(ptr)   | Considering ptr points to the body of a function, calls the function
| CALLP | 0x41   |          | Pops a variable = a an executes `CALL a`
| CALLC | 0x43   |          | Pops a variable = a and calls a native function in the cdecl convention.
| RET   | 0x44   |          | Pops a variable = a, executes the ++C function end procedure and pushes a to the stack

### Jumps (operandless)

| Name  | Opcode | Description 
|-------|--------|-------------
| JMP   | 0x50   | Pops a variable: n, and offsets the code execution n amount of bytes (relative to the end of the instruction)
| JEQ   | 0x51   | Pops two variables, and if they're equal, executes `JMP`
| JNE   | 0x52   | Pops two variables, and if they're not equal, executes `JMP`
| JL    | 0x53   | Pops two variables, and if the first one is less than the second, executes `JMP`
| JG    | 0x54   | Pops two variables, and if the first one is greater than the second, executes `JMP`
| JLE   | 0x55   | Pops two variables, and if the first one is less or equal to the second, executes `JMP`
| JGE   | 0x56   | Pops two variables, and if the first one is greater or equal to the second, executes `JMP`
| JLU   | 0x57   | Pops two variables, and if the first one is less than the second (unsigned), executes `JMP`
| JGU   | 0x58   | Pops two variables, and if the first one is greater than the second (unsigned), executes `JMP`
| JLEU  | 0x59   | Pops two variables, and if the first one is less or equal to the second (unsigned), executes `JMP`
| JGEU  | 0x5A   | Pops two variables, and if the first one is greater or equal to the second (unsigned), executes `JMP`
| JZ    | 0x5B   | Pops a variable, and if it's zero, executes `JMP`
| JNZ   | 0x5C   | Pops a variable, and if it's not a zero, executes `JMP`

**NOTE:** All comparasion jumps will first pad the smaller variable to the size of the bigger, then compare them like integers.

**NOTE2:** All jumps must be within the boundaries of the PPCIL blob.

**NOTE3:** All jumps are preformed after the address has been padded to the native pointer size
