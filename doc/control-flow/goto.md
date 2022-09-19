# Goto and labels

**NOTE:** goto statements and labels are not yet supported. They are a planned feature, but will take a long time until they are implemented

## Labels

A label is used to name a line of code. It is used as a point to which `goto` can go.

### Syntax

The syntax of a label is an identifier, followed by a colon. A label must be between the end of one and the start of another statement. For example, this is a valid label:

```c
int a = 10; label: a++;
```

But this is not:

```
int a = label: 10;
```

## Goto

A goto statement is used to redirect the flow of execution to another part of the code. It can be used to skip or loop parts of the code.

### Syntax

A goto statement consists of the `goto` keyword, followed by an identifier (the name of a label). A label may be referenced by a goto statement before or after the label's definition.

## Possible compiler optimizations

If the compiler determines that a `goto` statement makes a part of the code inaccessible, for example:

```c
// first example

goto label;
printf("I hate ++C");
label:

// second example

label:
printf("I love ++C");
goto label;
printf("I hate ++C");
```