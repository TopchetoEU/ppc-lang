# Try-catch statements

**NOTE**: Try-catches are not yet supported. They are a planned feature, but will take a long time until they are implemented

A try-catch statement will try running the code in the `try` block, and if any exceptions are thrown, they are going to be handled by the `catch` statement (if the types of the exceptions match, of course).

## Catches

A catch is a part of the try-catch statement, that consists of a single "parameter" of sorts, that is the type of exception that this catch is going to handle. This can be a contract as well, but due to some inefficiencies in that regard, it's recommended to handle separate classes.

### Nameless catches

If a catch doesn't specify the name of the exception it's getting, then the catch will only listen for the exception thrown. Such catches can specify multiple types they're listening for.

### Typeless catch

Such catch doesn't have any parenthesis and it listens for all exceptions. 

### Syntax

```c++
catch (Type name) statement;

// Or
catch (Type1, Type2, Type3, Type3, ... TypeN) statement;

// Or
catch statement;
```

### Order of catch executions

Since a try-catch can have multiple catches, they will be executed in order. If one of them breaks out of the try-catch statement, via `break`, `goto`, another `throw` or `return`, the chain of catches is going to be broken and the try-catch statement is going to be left.

## Throw

The throw statement accepts an expression of any type, or no expression. The throw statement acts like the return statement, but instead of a meaningful return value, it communicates to the caller that something has went wrong, and sends the provided value to the caller. The thrown value propagates the call stack, until a caller that has provided an appropriate catch is reached.

## Possible optimizations

Two catches that have the same types will be combined, for example:

```c++
void main() {
    try func();
    catch (int err) ...;
    catch (int) ...;
}
```

Will result in code which looks like this:

```c++
void main() {
    try func();
    catch (int err) {
        catch1 ...;
        catch2 ...;
    }
}
```

If a catch catches a type that the function doesn't throw, the catch will be omitted.

## ++C architecture for catching exceptions

In the ++C ecosystem, throw-catch works by passing catch callbacks to the function calls inside the `try` statement.