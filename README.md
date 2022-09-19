
# ++C (ppc) Lang

Developed by TopchetoEU

Stage of project: WIP (work in progress)

## What is it (++C in a nutshell)?

This is a low-level, highly-customizable language, semi-OOP, partially-reflected, interpreted and compiled language. Now, lets decipher what each of these means:

### Low-level

This language runs very close to the bare-metal machine, maybe with a single layer separating them (the core library). This means that you can write some pretty low-level stuff in this language, like drivers, operating systems and embedded programs.

### Highly-customizable

This (I think) is not applicable for most languages: most are stuck with the same primitive types, same core library. In this language, you have close to none primitive types, there are only pointers, arrays, integers and floats, that's it. This allows this language to be whatever you want it to be: maybe you just want the bare-bones statically-linked library for embedded programming, or you want the full-blown dynamically linked, garbage-collected, fully-reflected library? We've got you covered.

### OOP (Object-oriented)

This language started with the idea of being a regular OOP language, but as the time went, I've strayed away from the typical OOP languages - no matter what is done, they feel just incomplete to me. This language, instead of being a traditional OOP language, instead is a bunch of makeup on top of traditional procedural languages. What is the impact of this? You still have classes, interface, member methods (member functions), properties, etc. The major difference is that there isn't any inheritance. You might ask 'why?'. Well, inheritance would've made this language's architecture leaps and bounds more complicated, which I didn't want to do. On the other hand, ++C has by far the most advanced interfaces (contracts), more advanced than any language I've seen (so far). In this language, contracts are very special, since you can dynamically create them, make custom cast operators from and to them, etc.

### Partially-reflected

In this language, by default, you don't get much reflection. But, there's a compiler option that allows the inclusion of reflection metadata (by default its disabled so that no info about the inner workings of executables are leaked, especially because of proprietary software). Still, when that's enabled, you get full self-reflection. Of course, no self-reflection will be allowed for non-exported types (although it is planned to be possible).

### Interpreted and compiled

Now, don't get the idea that this language is interpreted like Python or JavaScript. No. Actually, this language is being compiled down to intermediate language. You have the option to use this as your executable, bundled with a ++C interpreter (recommended when distributing applications). Another option is to compile it even further, down to machine code. This is done when you have to work on a lower level (drivers, OS, embedded, etc.). This option provides a significant performance boost, as well.

## What sets this apart from C, C# and C++ (and Java)

++C is a very unique language with its extreme versatility: it can be whatever you want it to be. It combines all the good things about the languages I like: C, C++, C# (and Java's architecture, the language itself is nothing to write home about).

By far my most favourite language is C#, which actually does a wonderful job of saving me a lot of code milage. Still, good luck with coding in C# outside Windows (it's possible, but I'd rather put my two hands in a toaster instead of doing that).

C++ is my best bet for cross-platform compatibility, but we all know C++: its infamous with its "bloatness". I have taken some ideas from C++, for example the template system of ++C would be much more limited if it weren't for C++. ++C is intended to be the "modern C++, what C++ should've been".

At last, lets talk about the elephant in the room: Java. I have a love-hate relationship with Java in that I absolutely love to hate it. Java is a very behind-the-times language: it doesn't even have properties, lambda expressions are a complete mess and don't even get me started with generics. Still, I quite like the idea of compiling the code once and running it on every single device that could ever exists. That's why ++C is compiled to an intermediate language. That intermediate then can be either 1. ran with an interpreter or 2. compiled.

## This project will have a last version

This language has a really strict set of features, which, when implemented, this project will be considered "completed". Still, I will not abandon this project. If anyone logs any bugs or suggests any new features after that point (that won't take too long to implement), I will be keen on putting out a new version. To be clear, this project will have a finished state at some point.

## Documentation

Check out the [documentation](./doc/index.md) of this project, I've tried to make it as thorough as possible. Still, if something is unclear, please make sure to log it as an issue.

## How to contribute?

Any help is entirely voluntary, yet very much appreciated. If you see any bugs, please don't hesitate to report them (as issue logs) and if you feel like you could add to this project's worth, please do make a pull request.

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
