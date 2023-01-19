
# ++C (ppc) Lang

Developed by TopchetoEU

Stage of project: WIP (work in progress)


## How is this language unique (what is ++C)?

Let's face it, today's programming landscape has an abundance of programming languages, and anyone would have a hard time explaining why adding one more language to this oversaturated market would make any sense, but bare with me. This language is unique in one way: metaprogramming. Metaprogramming is a programming paradigm that has never been shown in its full potential. I hope that with this language I'll be able to do so.

## Metaprogramming

In ++C, a lot of emphasis is put on metaprogramming. But first, what is metaprogramming? In short, it means writing code with code, or generating code via more code. This is extensively used in languages like C and C++, and all the UI editors you're using (like the WPF editor), yes, they too are metaprogramming. Although it is a powerful tool, metaprogramming has never been executed properly (in my opinion) - C has the `#define` macros, which is for all intents and purposes complete garbage, C++ has templates (and macros), but it would take a human being about 100 years to fully understand and master the templates (they're too complicated). So far, only Rust has managed to pull off a somewhat OK version of the metaprogramming. Yet, I want to extend on the metaprogramming, by completely integrating it into this language. This is done by adding a modding API to the compiler, which allows any library to extend ++C's syntax.

## This project will have a last version

This language has a really strict set of features, which, when implemented, this project will be considered "completed". Still, I will not abandon this project. If anyone logs any bugs or suggests any new features after that point (that won't take too long to implement), I will be keen on putting out a new version. To be clear, this project will have a finished state at some point.

## Documentation

Check out the [documentation](./doc/index.md) of this project, I've tried to make it as thorough as possible. Still, if something is unclear, please make sure to log it as an issue.

## How to contribute?

Any help is entirely voluntary, yet very much appreciated. If you see any bugs, please don't hesitate to report them (as issue logs) and if you feel like you could add to this project's worth, please do make a pull request. Just make sure to 

## How to compile?

### Linux

- Command: `make`
- Install command (after build): `make install`
- Uninstall command: `make uninstall`
- Output: `bin/++c-linux`
- Prerequisites: `gcc`, `make`

### Windows

- Command: `make`
- Install command (after build): `make install`
- Uninstall command: `make uninstall`
- Output: `bin/++c-windows.exe`
- Prerequisites: `gcc`, `make` (I use the GnuWin port)

### Mac OS X

For now, there's no official support, but the build scripts should in theory work with any unix-like OS. You can try to use the linux instructions
