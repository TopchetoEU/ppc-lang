# Functions

Functions are the main units in ++C, containing executable code. Functions are used to represent code, so are really important for the language. It is important to understand that functions are everything that isn't a field, variable or a type - operators, constructors, methods, etc. are all functions.

## The difference between arguments and parameters

Before reading further, you need to understand, before continuing further.

The parameters are the local variables in the function, that get assigned by the caller, and the arguments are the values you pass via the call. When talking about parameters, we're talking about the callee function, and when talking about arguments. we're talking about a call.

## Syntax

A function may start with the `export` keyword. After that, is the name of the function, followed by the parameter list. The parameter list is a comma-separated list of the parameters. A parameter consists of its name, a type (`name: type`), and optionally a default value (`name: type = defval`). The parameter list is in parentheses. After the parameter list, a return type is optionally specified (if none is specified, `void` is implied). After that, the body of the function is either written with curly braces, or with a lambda arrow and a return expression following it.

```c
func_name(arg1: type1_t, arg2: type2_t, ... argn: typen_t): return_type_t { statement1; statement2; ... }
// or
func_name(arg1: type1_t, arg2: type2_t, ... argn: typen_t) { statement1; statement2; ... }
// or
func_name(arg1: type1_t, arg2: type2_t, ... argn: typen_t): return_type_t => expression;
// or
func_name(arg1: type1_t, arg2: type2_t, ... argn: typen_t) => expression;
```

The signature of a function is everything up to (and including) the return type.

## Calling

Calling of a function is the action of executing the body of a function, by first providing it with the needed context.


A call will jump to the beginning of the callee's (the called function) body, execute the code, and when the function finishes its executing, returns to the location of the call. Calling is a little bit more involved when including arguments.

### Syntax

A call consists of a callable expression (most of the times, a function name), followed by a comma-separated argument list, in parentheses.

```c
func_name(arg1, arg2, arg3, ... argn);
```

### Arguments and argument passing

The arguments of a call are a sequence of values, that match the signature's parameter types. When the call occurs, the parameters get assigned to the respective values, passed by the caller. After that, inside the callee, the parameters act like locals. Modifying their values won't affect the values, passed in by the callee

```c
import std::io;

func(a: int, b: int) {
    printf("%d", a + b);
}
export main() {
    func1(10, 5); // a = 10; b = 5 // 15
    func1(10 + 1, 5 - 3); // a = 11; b = 2 // 13
    func1(10); // Compiler error: mismatch between func's parameter list and passed arguments
}
```

## Return value

A function can have a return value. This is used when the function has some data it wants to give back to the callee. When the callee returns, it stops execution and returns to the point at which the callee was called. The call expression will have the value of the return value of the function

```c++
int func(int a, int b) {
    return a + b;
}
void main() {
    int a = func1(10, 5) + 1;
}
```

The call of `func1` will evaluate like this:

```c++
func1(10, 5) + 1;
// Calculate func1 result
15 + 1
// Perform addition
16
```
The return type in the syntax is the first part of a function, right before the name
