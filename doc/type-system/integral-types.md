# Integral types

Despite what you might think, the usual `char`, `int`, `short`, etc. are not integral types in ++C. Rather, there's a special type, called an integral type.

## Basics

An integral type is an integer, made up of any amount of bytes. It will by default define all required integral operators.

## Syntax

The name of this type in-lang is `$_int_x` (signed) and `$_uint_x` (unsigned), where x is any integer, bigger than 0, that is a multiple of 8. Any specified int will be automatically created.

```
$_int_16 myShort = 25000;
$_int_24 weird = 2000;
```

## How it works

Behind the scenes, those types don't actually exist. Rather, they will be compiled to their according native instructions. In [PPCIL](../citation-needed.md), this will be outputted to their dedicated instructions (`ADD`, `SUB`, `MUL`, `DIV`, `MOD`, etc.), and then will be either interpreted, or translated to their according assembly instructions.

## Limits

Integral types of absurd sizes (for example, 69 bytes) are allowed and won't throw an error, but when the code is either 1. compiled to native code or 2. interpreted usage of such absurd integral types will result in a runtime / compilation error or the type will get truncated to the system's maximum integer size. The max size of an integral type is the longest integer a processor provides native instructions for. Still, the integral types `$_int_8`, `$_int_16`, `$_int_32`, `$_int_64` (and their respective unsigned counterparts) are always present, no matter what the system is.

## Optimizations

The compiler might decide to increase the width of an integer, for performance sake. Most compilers will have an option to disable that, but its good to keep that in mind. This is especially true about integers of absurd sizes (`$_int_56`, for example). However, normal integral types may be widened, too: `$_int_8` might be widened to a 16-bit number to better match the underlying system's word size.
