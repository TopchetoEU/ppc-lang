# Versions in ++C

This version format will be used all troughout the ++C ecosystem, so it is important to be familiar with this

## Segments

A version in ++C is made up of three segments: major, minor and revision.

### Major

The major segment is a 16-bit unsigned integer. It is used to represent breaking changes, which means that 2.0.0, 1.0.0 and 3.0.0 are incompatible. It is reccomended that a project starts from major version 0 (alpha), until a stable state is achieved. Then, release all breaking changes in big groups, and increase the major version.

#### What is a breaking change in ++C?

A breaking change is a behavioural change (a change in the body of a function that is 1. not a bug fix or 2. a fix of a bug that the users relied upon, or 3. a change that leads to different outputs with the same given inputs), change of the signature of a function, removing definitions.

### Minor

the minor segment is a 16-bit unsigned integer. It is used to represent incremental changes, so bigger minor versions are compatible with bigger requirements (if a reqirement is 0.1.0, then 0.5.0 and 0.1.0 are compatible, but 0.0.0 is not). It is reccomended that after an increase of the major component this is reset (to 0), and increased after each major incremental addition to your project.

#### What is an incremental change in ++C?

An incremental change is the addition of a definition, without that breaking previous code, for example, adding an overload that will overtake a previous one:

```c++
bool func(short a) {
    // ...
}
// Added function:
void func(int a) {
    // ...
}
```

here, if someone called `func(10)`, previously, `bool func(short)` would get called, but now, the shorter cast route is `void func(int)`, which leads to a breaking change, because 1. there might be behavioural differences between the two functions and 2. the return types of the two functions differ, so the following code:

```c++
import std;

void main() {
    printf("%d", func(10));
}
```

Will break, because `func(int)` doesn't return a value.

Regardless, if the introduced overload doesn't break compatibility and doesn't introduce a behavioural change, the change is considered incremental.

### Revision

The revision segemnt of a version is a 32-bit unsigned integer. It represents either a build version or a bug fix index. Regardless, this segments represents a change that introduces no change to the public API or its behaviour (you could change the whole inner workings of a project in one revision, but as long as it introduces no breaking changes, that is fine).

When checking for compatibility between versions, the revision segment is completely igonred, so if the requirement is 5.45.8, 5.45.15 and 5.45.0 are compatible.

#### When to use revision changes?

There are three ways to use this segments:

1. Set it to 0 and don't use it
2. Use it as a build index (increase it on each build)
3. Use it as a bug fix index (increase it on each bug fixed)