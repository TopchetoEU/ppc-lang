# Generics

In ++C, generics are a language feature (they don't have an assembly representation). For each combination of generic parameters, basically a new type is being created (most compilers, including this one, will link the newly created type statically, so if two libraries use a definition with the same generic arguments, the template code produced will be present in both libraries). In the final assembly, no generic functions exist, neither do any generic constants. Only definition templates are being exported (don't worry, decompiling them is as hard as any other language).

The generics have two major groups: generic types and generic constants.

## Generic types

The generic types are template types that, in the scope of the template definition, behave as regular types. They are used as placeholders to any type the user may pass to the generic type parameter.

## Generic constants

Generic constants are quite interesting. They are basically a private const field / variable for the type / function they're defined in, so they may be referenced as constants anywhere where a constant would be referenced. Generic consts' values may be only pure expressions (one that can be resolved compile-time). Still, calls to functions that are pure is allowed.

## Constraints

Constraints may be put to generic types and constants. They define what may and may not be passed as a generic argument. Depending on the constraints, the language is going to allow the usage of some specific API of the types.

### Generic types

For generic types, constraints can specify what operators the type must provide, what contracts the type must implement. Depending on that, you will be able to use the specified required API.

### Generic constants

For constants, constraints consist of a pure boolean expression, which specified whether or not the constant is allowed. This allows for great flexibility when using generic constants.

### Multiple definitions with non-overlapping constraints

In ++C (like in C++), you can define a definition multiple times, if the generic constraints never overlap. This is more or less like overloading functions, but on steroids. This makes the generic system turing complete (by the way), and allows for some interesting stuff.

## Syntax

In ++C, the generics syntax is similar to the C# syntax. For example, you'd go about making a generic function like this:

```c++
export void myfunc<T1, T2><int const1, int const2>()
    where T1: { void increase(); }
    where T2: IEnumerable
    where (const1 < 10)
    where (const2 % 2 == 0) {
        // Do some stuff
}
```

And calling this function in the following manner:

```c++
class temp {
    int val;
    export int Val { get val; }

    void increase() {
        val++;
    }
}
void main() {
    myfunc<temp, int[]><5, 8>();
}
```

## Exporting a template definition

This will basically construct a template with the specified generic arguments and export it to the public API. This can be a really good optimization when you're making an API and know which generic arguments are going to be used the most. Still, don't overdo it. For example, a theoretical list implementation would do the following:

```c++
class list<T> { ... }

export list<float>;
export list<int>;
export list<object>; // object => export contract object;
```

Of course, as any definition, this could as well not be exported, but that will be useless, since this will just keep the template definitions local to your assembly, so at best they will all be used, and worse scenarios would be that these templates won't be used and will add bloat to your code. The compiler will warn you that just because you can do it, doesn't mean you should and will automatically remove such definitions.

## Compile-time optimizations you need to be aware of

Most generics-related optimizations are to do with limiting the amount of templates that have to be constructed. To achieve this, a compiler will cut out some templates it deems unnececeary.

If there are passed const parameters, they will just be converted to constructor parameters, if they aren't used in the size specifier of an array.

## Waning

Please, don't overuse the templates. They are a really powerful tool, but if you make too much code templated, you will blow up your binary size exponentially. They are made with the idea to copy-paste the code for you, but be aware that behind the scenes, for each generic combination, a new definition is being constructed.

## Optimizations the programmer can do

1. Using templates with less parameters (to decrease possible combinations)
2. Exporting commonly used generic definitions (be careful and don't overdo it)
