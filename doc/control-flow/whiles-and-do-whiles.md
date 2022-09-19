# While loops

## Syntax

They consist of a `while` keyword, followed by an expression (the condition), in parenthesis, followed by a statement (body).

```c
while (expression) statement
```

## Behavior

The condition can be of any type. The loop will continue looping until the condition becomes zero. The chech whether or not the condition is zero occurs before the execution of the body, so if the condition of a while loop is initially zero, the body will get skipped.

## Optimizations

If the condition is non-zero, any comparison will be omitted, and instead a simple jump function will occur. Anything after a while-true loop will be omitted. If the condition is zero, the while loop is omitted.

## Equivalent PPCIL assembly

For any given while loop:

```c
while (condition) body;
```

Its equivalent assembly is:

```
start:
; condition
jz end
; body
jmp start
end:
```

# Do-while loops

## Syntax

They consist of a `do` keyword, followed by a statement, which is followed by a `while` keyword and an expression (condition) in parenthesis. The statement is followed by a semicolon.

```c
do statement while (expression);
```

## Behavior

The condition can be of any type.
First, the body is executed. Then, the condition is tested. If it's `false`, then execution of the code continues after the loop. Otherwise, the whole process returns

## Equivalent PPCIL assembly

For any given do-while loop:

```c
do body while (condition);
```

Its equivalent assembly is:

```
start:
; body
; condition
jnz start
```

## Optimizations

If the condition is `true`, then the loop is transformed into a while-true one. If the condition is `false`, instead of the do-while loop, the statement is executed just once. This makes for good encapsulation, but since this is one of the few languages that supports block statements as a statement on their own, this feature is deemed useless and just an optimization for bad code.

### While loop

The while 

# Difference between the two loops

The while loop is called a pre-conditional, which means that first the condition is tested, then the body is executed, whereas in the do-while loop the body is executed first and then the condition is tested.
