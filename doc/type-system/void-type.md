# The `void` type

In ++C, there's an integrated `void` type. The `void` type is an empty structure, and has a special way of passing around: it doesn't get passed around, since there's only one possible value of `void`. It can be converted from any value, and to any value. The conversion of a `void` to a type results in a value of that type, filled with zeroes.

# The `null` constant

This constant is the only possible value of `null`. It can be returned by `void` functions. `null` can be used to initialize a value to a zeroed-out version of itself. If a struct wants to implement a different default value, it should implement a conversion from `void` to itself.