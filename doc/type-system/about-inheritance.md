# About inheritance

Now, you might be wondering why is there no inheritance in ++C. There's a really simple reason: I don't think an OOP language needs it. It makes your code very rigid and generally harder to understand. But that's not the only reason: the ++C's architecture is made to be simple, and the inheritance, architecturally, make the architecture very complicated.

# How to use inheritance patterns

There is a saying in the programmer community: composition over inheritance. What does this mean? In short, it means that instead of inheritance, you'll have a reference to your super object as a field. An equivalent of the abstract method would be to pass the `this` (as a contract) to the super object.

# Class-contract hybrid

What does this mean? This means that there will be an option to define `contract` functions and properties, which will be exported in a separate contract type. That type will be exported, but it can't be referenced, unless inside the class-contract hybrid. Then, the contract can be accessed with the `contract` keyword, which usually is invalid in a class definition. This will be useful for accepting the contract as a constructor parameter, which will be useful for composition (over inheritance).
