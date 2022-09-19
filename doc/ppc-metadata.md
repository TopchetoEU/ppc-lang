# The ++C metadata file format

## What is this?

Basically, ++C stores data about its type structure, so that reflection can be provided. Each ++C binary exports a segment in its binary, called `ppc_meta`. If the segment is not present, obviously, this is not a ++C library / executable. Still, the compiler has the option to not include this metadata, which is applicable for embedded, but if you decide to do that, you won't be able to use this library in other ++C libraries / executables. If you want to hide your API, this is a great way to do so.

## General concepts

The goal of this format is for it to be as straightforward to parse as possible, so this will be pretty close to the structures, representing the definitions.

In this document, we're going to use C-like syntax to represent structures.

It is also important to note that all pointers are relative to the beginning of the metadata, so that copying of the metadata can happen with `memcpy`.

## The version structure

You will see this quite often throughout this specification, so let me explain it:

```c
struct version_t {
    ushort major;
    ushort minor;
    uint revision;
};
```

A version is a 64-bit digit, consisting of a major, minor and revision components. If the major components don't match, the two versions are incompatible. A change in the major version signifies a breaking change (the major version is appended at the end of library file names, so there are no conflicts between them). The minor version signifies incremental changes (additions to the library), so the provided version's minor version must be bigger or equal to the target minor version for compatibility. The revision component specifies changes, that don't modify the exposed API, nor the behavior. The revision is ignored when checking version compatibility.

Any of the version's components may be -1 (the max value of the type), it is altogether ignored (a library may have only a major and a revision component)

## Layout
o describe the metadata's byte layout in order

Each table will consist of the
The following sections are going t following:

```c
size_t n; // The amount of entries
size_t size; // The byte size of everything that follows
```

### Binary attributes

This will specify all needed attributes for the binary, like version, name, author, etc.

```c
struct attributes_t {
    version_t version;
};
```

### String literals

This table contains all string literals, used in the metadata. They're recorded like this:

```c
struct literals_t {
    size_t n; // Amount of literals
    size_t size; // The byte size of the following array
    char data[size];
};
```

### Definition table

Since a definition can appear only once (overloads don't add definitions), you can have a full record of all definitions. This is an alphabetically sorted table in which to find the address of a definition

```c
struct entry_t {
    const char *name; // From the literals table
    void *def_ptr; // The pointer to the definition (relative to the pointer)
};
struct entry_table_t {
    size_t n;
    size_t size; // The byte size of the following array
    entry_t entries[n];
};
```

### Module table

This table contains all the modules, that are included in the executable. They will be referenced afterwards.

```c
struct module_t {
    const char *name;
    version_t version;
};
struct module_table_t {
    size_t n, size;
    module_t modules[size];
};
```

### Type table

The type table contains all type identifier that are going to be used throughout the metadata. They consist of the following structure:

```c
struct type_id_t {
    const char *name; // This is the full name, with the namespace and name
    module_t *module; // The module from which this type originates

};
```


## Function notations

Each function notation is done in the following way:

|    Name    | Description | Syntax |
|------------|-------------|--------|
| this       | Specifies which is the this type. | T\[type]
| return     | Specifies the return value | R\[value]
| args       | Specifies the arguments | A\[type1]\[type2]...\[typeN]$

## Declarations
Each declaration ends in the following way:

| Name       | Syntax      | Description |
|------------|-------------|-------------|
| Identifier | namespace.name | The namespace of the definition
| Name       | name\|      | The name of the declarable. Includes just the name, without the namespace and the type-container. It is terminated with a pipe char |
|  Custom flags | ...      | The declaration-specific custom flags. Exactly 2 chars. Most will fill that space with underscores |
| Children   | [child1,child2,...] | Not mandatory, some declarations may replace this with some other data |
| End        | ; |

| Letter | Name           | Flags  | Description |
|--------|----------------|--------|-------------|
| R      | Reference type |        | Declares a type that is passed by its reference. |
| V      | Value type     |        | Declares a type that is passed by its value |
| C      | Contract       |        | A contract type, containing method and property declarations.  |
| E      | Enum           |        | A type, containing constants of a certain type. |
| D      | Delegate       |        | A type, specifying a function pointer. It has the same declaration as a function |
| P      | Property       | ?S, R/W | Declares a property, may be static or instance, may be read-only or read-write. Instead of children, a single type specifier is given |
| F      | Function       | ?S/E, ?V | Declares a function, may be static or not. It has no children list, instead, it has a return type specifier, followed by its parameters as type specifiers. The last argument is a variadic argument if V flag is specified. If E is specified, the function is a static extension function |
| I      | Field          | ?S, ?C | Declares a field, which may be static or not, depending on whether or not S or I flag is specified. It may be constant if the flag C is specified. Instead of a body, it has a type specifier, specifying the type of the field |
| N      | Event          | ?S, A/R | Declares an event, that may be static, if S is specified. If A flag is specified handlers may be just added, if R is specified, handlers may be removed as well. The children list is replaced by a delegate type specifier |

**NOTE:** Before the children list of all type declarations, except for enums and delegates, a list of all base classes is defined in the following manner: `[base1, base2, ...]`

## General structure

Each metadata file/sector follows the following structure:

| Type        | Name          | Description |
|-------------|---------------|-------------|
| int16_t     | majorVersion  | The major version of the metadata format used |
| int16_t     | minorVersion  | The minor version of the metadata format used |
| declaration... | declarations | A list of all present declarations, separated by new lines '\n' and terminated by two new lines ('\n\n') |
