# Switch statements

**NOTE:** Switch statements are not yet supported. They are a planned feature, but will take a long time until they are implemented

## Syntax

Switch statements differ quite a lot from other languages, as you can see. First, we have the `switch` keyword, followed by an expression (the value) in parenthesis. Then, we have a series of `case` statements, and optionally, a `default` statement (not necessarily the last one, but the compiler will warn you about it).

In contrast to other C-like languages, cases in a switch statemnets are more like simplified if statements, so they are written with first the `case` (or `default`) keyword, followed (if a statement) by the value in parenthesis and after that the statement of the case.

In real code, this would look like this:

```c#
switch (value)
case(val1) statement;
case(val2) statement;
...
default statement;
```

## Behavior

Behaves just like an if-elseif-else chain. Compares the value to each given case's values, and runs each case which contains the value in its value list.

## Possible optimizations

If a value is determined to not be possible, yet is a case in the switch statement, its going to be removed. For example, the following code:

```c
if (val != 3) {
    switch (val)
    case (1) printf("1");
    case (2) printf("2");
    case (3) printf("3");
}
```

Will be converted to:

```c
if (val != 3) {
    switch (val)
    case (1) printf("1");
    case (2) printf("2");
}
```