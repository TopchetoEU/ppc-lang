# Type architecture

**NOTE: This is dated and the first iteration of the type system.**

This document describes how the ++C types work. The ++C's type system is made to be as simple as possible so the compiler can be simpler, hence faster. That's wh this is (I think) a type system unlike any other

## Members
Each type may contain only fields. A field has all the common modifier that each definition has (public, private, static, etc), as well as a `readonly` modifier, type and a name. Fields must have a static type.

Note that nonsealed methods are basically readonly fields and each time they are overrided the constructor assigns a new value to them

### Methods
Methods are instance functions, which basically are either: delegates that take as a first argument a reference to the instance or a static function that takes a first argument reference to the instance. The first are real methods and the seconds are instance functions, or as I like to call them - 'demimethods'. For the most part, we use demimethods, unless when a method is marked abstract or virtual. Then, we use real methods - delegate fields. So in a sence, the following class:
```cs
public abstract class Shape {
    public abstract float Perimeter { get; }
    public abstract float Area { get; }

    public override sealed string ToString() {
        return $"Perimeter: {Perimeter}, Area: {Area}";
    }
}
```
Would actually be
```cs
public class Shape {
    public delegate float PerimeterGetter(Shape shape);
    public delegate float AreaGetter(Shape shape);

    public readonly PerimeterGetter Perimeter_Get = null;
    public readonly AreaGetter Area_Get = null;

    public static string ToString(Shape shape) {
        return $"Perimeter: {Perimeter}, Area: {Area}";
    }
}
```

### Events
Events are a whole another level of mess, that for this edition of ++C we won't get into. Otherwise, the compiler would refer to a user-defined class `$_event`, which would be used for event handling. For now, compilers are allowed to scream at the programmer for using events

## Type classification

### Kinds of types
There are again two categories by which we classify types:
- By value / by reference (struct or class)
- Is it callable
Since both are pretty much stored as booleans, we can have normal classes that implement a callee, as well as structures implementing a callee and event interfaces that implement a callee. Because of that, ++C delegates work like Java's functional interfaces and delegates just cover up that mess. Still, all pointers are callable and the return type depends on the type that the pointer points to. Each pointer may be called with any arguments. This is how jumps are executed

### Simple, complex and dynamic types

This concept is introduced to allow the programmer to get the most out of the type architecture of ++C. In some contextes we can't use references or pointers, or sometimes we outright need a constant value, so that's why we made two categories in which to classify types:
- By contained references:
    - Simple - the type must be a by-value structure that is not a pointer and contain no fields that are complex
    - Complex - all types that aren't simple
- By their size:
    - Static - their size is known compile-time
    - Dynamic - their size is unknown, for example, the `void` type. For now, no other dynamic types exist

There is also a jargon, called a 'primitive' type. That basically means that the type is a language feature, like `$int_...`, `void`, etc...

Combinations of all 4 categories can be seen: simple dynamic types are types that contain no references, yet we don't know the size of.

All of the above-mentioned types may be used as generic parameter constraints (`simple`, `complex` and `dynamic`). By default, all types have a constraint `complex dynamic`, which covers all types. It is important to understand that simple types can be used in contexts where dynamic types are required

#### What about C# dynamic types

Since C#'s architecture is leaps and bounds more complex than ++C's one, we can't have dynamic types without a rework of the compiler from the ground up. Future plans for libraries being able to inject code into the compiler are a feature which may be implemented in the very far future, but for now, it is impossible to have C#-like dynamic objects. For now, the way is to use the `object` and `any` types or to use a library that supports full reflection.

## The purge

I like calling the point in which the user-friendly syntax-sugary code to what the compiler modules after the abstract tree constructor consider 'ledgible'. No enums, methods, constants or interfaces are safe from THE PURGE. Jokes aside, it is an important step in order to make the compiler a much simpler engine overall. The purge can be compared to the following: imagine a cup of mixed sand and sugar. The purge is pouring water over the whole thing - all the 'sugar of syntax' is being destroyed and only the sand remains.

## Arrays

In ++C, arrays are probably the most complicated feature. They are a language feature (primitive types), so you can rely on their existence. Still, due to the philosophy of ++C, these classes provide just enough API so that a core library can be as flexible as possible. There are two kinds of arrays: static and dynamic.

### Static arrays

A static array is an array with a type-fixed size (the array's size is a generic constant). They act like C and C++ arrays and can be used so that the compiler activates some of its optimizations. Static arrays are simple primitive static types (have a fixed size, are a language feature and don't include any pointers) The static array can be represented by the following code:
```cs
public sealed struct StaticArray<T, int size> where T: static {
    public sealed T this(size_t i) { get; set; }
    public sealed T* operator implicit() => ...;
}
```
Since there can be a static array of static arrays, we've decided that the static arrays won't be multidimensional. That means that in order to make a multidimensional array you need to do the following:
```cs
int[][] arr = new int[10][10];
```
Still, there is the syntactic sugar of `new T[n,m]`, which will basically get replaced by the above syntax.

The only limitation of these arrays is that their size can be only static, which is what we're going to fix with dynamic arrays.

### Dynamic arrays
The dynamic arrays are pretty much like C#'s arrays - they don't have a compile-time fixed size, which means that it is mandatory to allocate their memory runtime. Dynamic arrays are initialized as dynamic only when a non-constant size is used. The other major difference is that dynamic arrays can be multidimensional. This time, the core library must provide a `$_dynarr` type, which follows the following structure:
```cs
public sealed class $_dynarr<T><size_t dims = 1> where T: static {
    public T this(params size_t[dims] indexes) { get; set; }
    public DynamicArray(params size_t[dims] indexes) => ...;
}
```
The compiler is going to call the initialized when needed and is going to use the `this[]` indexer to fill the array with data. The dynamic initializer is only called when an array with a non-constant size is initialized. The compiler will try to call the `$_dynarr` initializer as little as possible.

## Enums
As it has been already discussed, this compiler's aim is to have as simplistic architecture as possible. Because of that, a lot of generalization must happen behind the scenes, and that doesn't leave any room for the enum type. Still, you'd be hard-pressed to find any language without an enum construct, so ++C has one, too. Still, an enum in the compiled code is basically a struct that contains just one field (of the specified type), a ToString and casters. An enum may contain constants of any simple static type (by default it is `int`, but can be specified by specifying what type the enum 'extends'). In ++C enums are strikingly similar to the ones in Java, although in Java they are somewhat more flexible.

The following code:
```cs
struct Vec2I {
    public int X;
    public int Y;
}

enum MyEnum: Vec2I {
    Up = new Vec2I(0, -1),
    Down = new Vec2I(0, 1),
    Left = new Vec2I(-1, 0),
    Right = new Vec2I(1, 0),
    Zero = new Vec2I(0, 0)
}
```
would be equivalent to:
```cs
struct Vec2I { ... }
class MyEnum {
    public const Vec2I Up = new Vec2I(0, -1);
    public const Vec2I Down = new Vec2I(0, 1);
    public const Vec2I Left = new Vec2I(-1, 0);
    public const Vec2I Right = new Vec2I(1, 0);
    public const Vec2I Zero = new Vec2I(0, 0);

    private Vec2I val;

    public Vec2I operator implicit() => val;
    public static MyEnum operator explicit(Vec2I vec) => new MyEnum(vec);

    private MyEnum(Vec2I vec) {
        val = vec;
    }

    public override string ToString() {
        switch (val) {
            case Up: return "Up";
            case Down: return "Down";
            case Left: return "Left";
            case Right: return "Right";
            case Zero: return "Zero";
            default: return val.ToString();
        }
    };
}
```

If having this `ToString` method autogenerated is not wanted, it can be turned off in the compiler.
Unlike its C# counterpart, in ++C you can define properties and methods in enums, but they can't be virtual, nor abstract.

## Interfaces

Basically classes but all methods abstract and shit

