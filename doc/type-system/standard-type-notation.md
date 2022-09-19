# Standard type notation (STN)

This is a system, by which each type in the ++C language can be recognized and is used on a very low level to identify types quickly.

## Structure layout

Name      | Type   | Description
----------|--------|-------------
name      | char[] | A null-terminated string, containing the name of the type. This must be a legal ++C identifier (citation needed)
genTypes  | STN[]  | An array, containing the same amount of STNs, as type parameters in the specified type
genConsts | any[]  | An array, of undefined length, containing the same amount of tightly-packed constants (in their little-endian byte representation) as const parameters in the specified type

## In-language representation

In ++C, there's a special type, called `$_stn`, that is basically a dynamic byte array (`$uint_8[]`), that keeps a byte-wise 

## Metadata representation

In metadata, types of fields, properties, functions and arguments are represented in this way.

Also, in metadata, there's a sorted index of all names, followed by an offset pointer to the associated definition (might not be a type)
