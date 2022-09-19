# Members

In ++C, there are two main types of members: fields and functions. Anything else is derived from those types of members, so they'll be the first ones to be included in this document:

## Fields

The fields are the simplest types of members, they basically contain any type of data in them. They are stored in the structure (class or struct) sequentially, and are the first members to be stored in a structure.

Fields can be readonly, which means that although the memory that keeps the field can be set, it won't be set by the ++C lang (no guarantee for anything outside ++C).

Fields that are readonly and are set to a constant (compile-time know value), are considered a const. This means that they won't be included in the memory layout of the structure.

Example fields:

```c#
export class MyClass {
    float field1;
    int field2;
    char[] field3;
}
```

## Functions

Functions are more in-depth explained [here](../functions.md).

Functions as members have a `this` specifier, where the `this` type is the container type for the function.

```c#
import System.Console;

export class MyClass {
    export int x, y;
    export void myFunc() {
        WriteLine("%d", x + y);
    }
}
```

Adding a `this` specifier to a member function is illegal, as you can see:

```c#
export class MyClass {
    void myfunc this int() {
        //      ^^^^^^^^ Error here
    }
}
```

## Operators

Operators, like functions, take as a `this` specifier the container type. Operators are used in expressions 

## Constructors

Constructors in ++C are nameless member functions. They're called when an object has been allocated, but not yet instantiated. The constructor's job is to initialize the object, according to the provided parameters.

Example constructor:
```ts
export class MyClass {
    export int x, y;
    export (int x, int y) {
        this.x = x;
        this.y = y;
    }
}
```

This is one of the many major differences from most languages: in ++C, constructors don't have a name, as well as a return type. In result, this makes the constructor just an argument list with a body
