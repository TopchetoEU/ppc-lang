# Functions

Functions are the main units in ++C, containing executable code. Functions are used to represent code, so are really important for the language. It is important to understand that functions are everything that isn't a field, variable or a type - operators, constructors, methods, etc. are all functions.

## Syntax

A function is defined like in most C-like languages, but with a few twists:

```c
return_type_t func_name [this this_type_t](type1_t arg1, type2_t arg2, ... typen_t argn [, params type_t variadicArg]) body_statement;
```

## Calling

Calling of a function is the action of executing the body of a function, by first providing it with the needed context.


A call will jump to the beginning of the callee (the called function)'s body, execute the code, and when the function finishes its executing, returns to the location of the call. Calling is a little bit more involved when including arguments.

### Syntax

In ++C, calling is done via the standard C-like syntax:

```c
func_name(arg1, arg2, arg3, ... argn);
```

## `this` context

A `this` context is required by some functions, that will be called in the format `noun.verb(arguments)`. In practice, a pointer to the object upon which the function is called is just passed as a first argument to the function. The caller is responsible for providing an adequate pointer to the object, for example in the following case:

```c
(10).myFunc();
```

The integer `10` upon which `myFunc` is being called has no real representation in the heap, nor the stack. In such cases, the compiler will allocate space for the integer somewhere, and then will free it.

### Calling functions with a `this` context

The `this` context is passed last on the stack, or first as an argument. Note that a function may be called upon a `null` object, which may break some functions. Also note that when calling a function without a `this` context (or a `this void` context), nothing is passed for the `this` context.


## Parameters

The parameters of a function are a set of values, which are being passed by the caller to the callee, which can then be used as local variables inside the callee. Any modifications to the parameters don't affect the passed values by the caller, although generally it's a bad practice to assign values to the parameters. In the following example:

```c++
import System.Console;

void func(int a, int b) {
    WriteLine(a + b);
}
void main() {
    func1(10, 5); // 15
    func1(10 + 1 /* 11 */, 5 - 3/* 2 */); // 13
    func1(10); // Compiler error: mismatch between func's parameter list and passed arguments
}
```

Parameters can also be looked at like template values, that are being replaced by the given values by the caller, before the code is executed. Of course, this is not exactly like that, but is a nice analogy to keep in mind for beginners.

In the syntax, parameters consist of a comma-separated list of type-name pairs, encapsulated in parenthesis.

### Calling with arguments

All rules for calling are followed, but before jumping to the callee, the caller will push to the call stack the parameters in reverse order. Then, the function will be executed. In the end, the caller is responsible for cleaning up anything that was required to call the function, for example the stack.

One important part of this specification is that arguments may not exceed the native register's size. If so, the callee is responsible for allocating space for the argument, passing a pointer instead and then freeing the memory (the memory may be taken from the stack and is what this compiler will do).

## Return value

A function can return a value too, which means that the callee is going to perform some calculations, and then, the last thing it does is putting the calculated value in stack and jumping back to the caller. The caller then can use this value however he pleases.

A return takes an expression, that is being first evaluated, THEN passed to the caller.

When a function returns a value, the call of that function can take part in an expression. When the expression is being evaluated, the return value of the function is going to "replace" the call to the function, like this:

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
15 + 1 // Note that it doesn't get replaced by 10 + 5, but with 15, because the expression in the return statement evaluates before being returned
// Perform addition
16
```

### Calling with return value

The return value of a function will be pushed to the stack by the callee, and that will be the last thing it does, before returning to the caller. The caller is responsible to handle the return value adequately, whether or not that means to ignore it, or store it somewhere. In all cases, the return value's size must be accounted for when freeing the stack. If the return value exceeds the register size, the callee will allocate a dangling pointer that may be used only once to copy the data from it to somewhere safe. Usually, this is from a part of the stack that is already freed, so don't rely on the given pointer.

The return type in the syntax is the first part of a function, right before the name

## Table of contents

More of this part of the documentation is to be written soon, there is more to be said