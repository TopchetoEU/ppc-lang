# Constructs

The constructs are a unique take on the meta programming concept. A construct will get an access to your source code in the form of parsed tokens (identifiers and operators) and will output one or more constructs in your code. This can be used to define custom type structures (i. e., enums, interfaces, delegates, etc.), define inline functions with compile-time errors, and so on. The possibilites of constructs are pretty much limitless.

## How to define a construct

Construct definitions are stored in special files (.cppc, or construct ++C). The name of the file will be the name of the construct. The name of the construct file doesn't matter, but it is recommended that the name matches the keyword of the construct.

The structure of such a file is like a ++C file, but with a few major differences: