# References

A big part of what makes ++C tick are references. They are very similar to Rust's idea of references, and that's the language, from which inspiration for the references has been taken. 

## What are they?

References are basically pointers, but have no arithmetic representation. They are used to point to an object of any type and any size. Usual use cases for references are to mutate an object with a reflection in all "users" of the reference. A practical example would be the following:

```cpp
import System.Console; // System.Console, in contrast to C#, is a namespace

// Classes are ref by default, if this was a struct,
// ref would have to be specified in front of MyClass
class MyClass {
    int Field1 = 0;
    int Field2 = 0;
}

void main() {
    List<MyClass> a = new var(), b = new var(); // Types are implied, like in C#
    MyClass obj = new var();

    a.Add(obj);
    b.Add(obj);

    WriteLine(a[0].Field1); // 0
    WriteLine(a[0].Field2); // 0

    obj.Field1 = 10;

    WriteLine(a[0].Field1); // 10
    WriteLine(a[0].Field2); // 10
}
```

Another reason to use references are to output multiple values or to change a value that is in the callee's scope:

```cpp
import System;
import System;

void split(float number, ref int whole, ref float fractional) {
    whole = (int)number;
    fractional = number - whole;
}

void main() {
    float mynum = CLI.AskFloat("Input a number"); // The built in CLI library

    // When a ref is required, new variables can be passed as well
    split(mynum, int whole, float fractional);

    CLI.Print("Whole:", whole);
    CLI.Print("Fractional:", fractional);
}
```

## Difference between references and values

Although references are used to make pointers work more like values, they are still different in a major way: after you've passed a reference to a function and that function takes ownership of that reference (or in Rust slang, "borrows" it), you can no longer use it. This is so because someone (after all) has to clean up the memory of the reference, and that is the current owner of the reference: once it falls out of the function scope (the function returns and it still owns the reference), the reference will be freed.

## Sharing references

