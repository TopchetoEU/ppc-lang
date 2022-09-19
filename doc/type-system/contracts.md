# Contracts

Contracts are like interfaces in other languages: they are a list of functions, that a type must implement in order to "comply" with the contract. Still, there are a few differences, that will be cleared later in this document.

**ONE IMPORTANT THING**: Unlike most languages, contracts are their own data structure, much like `struct` and `class`. They are not used to "cover up" specific about an object, they are used to keep the functions of an object we need, basically a list of delegates. Contracts are slower than regular class function calls, so using contracts sparingly is recommended.

## Syntax and naming conventions

The syntax of the contract is similar to the C#'s syntax, but instead of `interface`, the keyword `contract` is used.

```c#
[export] contract (name) [: (contract1), (contract2), ...] {
    (definition1);
    (definition2);
    ...
    (definitionN);
}
// or
[export] contract (name) [: (contract1), (contract2), ...];
```

## Dynamic contracts

A major difference that sets apart the contracts from the interfaces is that contracts can be "dynamically" created. That is so, because contracts just contain function pointers and the instance of the contractor. That means that even if the contractor doesn't know about the contract, it still can comply with it, so a contract can still be derived from it. That is done compile-time, despite the implied dynamicity of the dynamic contracts. Basically, if a type has all the functions a contract requires, a contract can still be instantiated, via an explicit cast. Dynamic contracts are particularly useful for requiring any sort of operators.

## Determining if a type complies with a contract

It is important to know how the compiler knows if a type compiles with a contract. A core ideology of this compiler is that there should be no unnecessary limitations, so keep that in mind:

1. The target type has to contain all the names of the functions that the contract requires
2. The overload count of each function should be matched
3. The target function has to have the exact same signature as the contract function

## In-memory structure

A contract consists of two "fields": a pointer to the `this` context and a pointer to an ordered array of all the function pointers that the contract requires. When a static cast occurs, the contractor has already allocated a space in the executable for its contract function pointers (CFP), so it only needs to give a pointer to itself and to the CFP.

## Contract reflection

Since a contract hides an object behind it, we only know the type of the contract, not the contractor. Still, there's an option to enable full reflection, which will create an additional field in the contract, that will keep the contractor's type ID. Then, whenever you use `typeof contract`, instead of getting the contract type, you'll get the type of the contractor.
