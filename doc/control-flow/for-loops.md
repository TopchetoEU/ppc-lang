# For loops

## Syntax

For loops consist of a declaration, condition, assignment and a statement. The declaration is a declaration statement, the condition is a expression of type `bool` or a type from which `bool` can be derived and the assignment may be any expression.

```c
for (declaration; condition; assignment) statement
```

## Behavior

For loops are syntax sugar for while loops, and they can roughly translate to the following code:

```c
declaration;
while (condition) {
    statement;
    assignment;
}
```

Where the only difference is that the declaration is scoped inside the for loop, instead in the outside variable scope.

## Optimizations

If the assignment statement is deemed pure (doesn't affect the outside environment), then it will be omitted. The same optimizations that are made for the while loop are in full action here.