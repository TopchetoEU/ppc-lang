# Integrated types

In ++C, there are several integrated types:
- `void`
- `int8_t`, `int16_t`, ... `int256_t`
- `fl16_t`, `fl32_t`, `bigfl_t`

## Void type

The void type is for all intents and purposes an empty structures. All other empty structs are convertible from and to a `void`. There is also a literal `null`, that represents a value of type `void`. All types that want to implement a default value will implement an explicit cast from `void`. `void` is the return value of all procedures (functions with no return values). All `void` variables/arguments will be omitted.

## Integral types

An integral type is an integer, made up of any amount of bytes. It will by default define all integral operators:
- Addition / subtraction
- Multiplication / division / modulo
- Negating
- Bitwise AND, OR, XOR and flipping
- Comparisons

Although the underlying language/architecture might not support a certain size of integers, the translator will guarantee that these types will work as expected.

## Float types

A float type is a number with a floating fraction point. Although it is not required, it is almost guaranteed that the float types are an implementation of the IEEE float standard. Still, some platforms that don't support float maths will have to implement functions to do so, which leaves it up to the translator author to decide how the floats will work. Of course, the float type implements all algebraic operations for integers, including some float-specific:
- Floor, ceiling, round
- Square root
- Logarithm
- Exponentiation
- Sine, cosine, tangent
- Int to float conversion
- Float to int conversion

There are tree variants of the float: `fl32_t`, `fl64_t` and `bigfl_t`. `fl32_t` is the closest representation of the IEEE single float that the system can support. `fl64_t` is the same as `single`, but for double floats. `bigfl_t` is the biggest float that the system supports. Usually, `fl64_t` and `bigfl_t` are the same, but sometimes `fl32_t`, `fl64_t` and `bigfl_t` overlap. `fl32_t` and `fl64_t` are guaranteed to be 32-bit and 64-bit respectively. `bigfl_t` is used for systems that natively work with bigger floats (for example, the x86_64 architecture supports 80-bit floats).

## How it works

These types don't actually exist, but rather are converted to special statements (native operators), that later will be transpiled to the underlying language's counterparts.
