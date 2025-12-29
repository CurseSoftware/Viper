# Experimental Compiler Toolchain & Language
Temporarily named Viper

## Purpose
We wish to create a compiler toolchain for an experimental systems programming language.
The main purpose of the project is to experiment with compiler toolchain technologies while also exploring potential useful language design ideas for the compiled language itself.

Some examples of the kinds of ideas being pursued are:
- How can we express the syntax of the compiled language so that it is easy to change and extend in the compiler while maintaining acceptable runtime performance.
    - Is it possible (and desirable) to decouple the parser from the tokenizer?
    - How effective is single pass tokenizing vs lazy tokenizing?
        - This can affect the parser (somewhat), so what are the tradeoffs here?

## Building the toolchain
Building the toolchain from source requires CMake.

This is currently being built on MacOS with C++ 20, but we have working builds on Ubuntu and WSL as well. 
Theoretically, this should compile on Windows, but it is untested, and that assumption has failed me too many times to be confident.

We recommend using the utility script for building/running any targets.
```bash
./go build --<debug | release> toolchain
```
This will build the toolchain in either release mode or with debug symbols.

### Running a built toolchain
Similarly to building, you can do:
```bash
./go run --<debug | release> toolchain
```
This will run the toolchain.

To make life easier, you can create a symlink to the built binary:
```bash
# Symlink the debug build
ln -s build_debug/toolchain/toolchain
# Or symlink the release build
ln -s build_release/toolchain/toolchain
```

Then rather than using the `go` utility script you can just
```bash
./viper ...
```

### Commands
Currently, the lexer is being developed, so this is the only available command.

You can run
```bash
./viper lex -f input.viper
```
or
```bash
./viper lex --file input.viper
```

## Concept Language
The language itself is not fleshed out, and as this is experimental it will greatly vary over time.

Here are base-level concepts for the language as of now:

Structures:
```c++
struct Point
{
    x: i32;
    y: i32;
}
```

Functions:
```c++
define my_function() -> Point
{
    return Point {
        x: 0,
        y: 0
    };
}
```

Entry point to program:
```c++
// Entry function
#[entry]
define main() -> i32
{
    return 0;
}
```

Modifiers
```c
#[printable, debug_printable]
struct Point
{
    x: i32;
    y: i32;
}

define test() -> void
{
    let p: Point = Point {
        x: 0,
        y: 0
    };

    p.print();
}

// This is a pure function
define my_function(p: Point&) -> void
{
    // Will not compile
    p.method_that_mutates();

    // Non-modifying, produces no side-effects. Compiles fine
    p.print();
}

// This is an impure function
@impure
define my_function_2(p: Point&) -> void
{
    // Marked impure, compiles fine now
    p.method_that_mutates();

    // Non-modifying, produces no side-effects. Compiles fine
    p.print();
}
```

Unions function similar to Rust tagged-unions/enums:
```c
union Token
{
    Keyword(std::string),
    Equal,
    Plus,
    Minus,
    Slash,
    ...
}
```

Other language concepts:
- Explicit mutability
    - All variables are immutable by default, and mutability is explicit in the syntax (via `mut` keyword)
- Trait-like system
    - This is not fleshed out, but the trait system in Rust is great at defining shared behaviors without inheritence. As this language is largely designed with data-driven models in mind, we want to know what we can take from that system to make data-driven work easy and ergonomic.
- Pure functions by default
    - We are experimenting with all procedures being pure by default. To make an impure function you use the explicit `@impure` directive (syntax subject to change).
    - The goal is to see what is achievable with this pattern, and is this useful at all.
    - Obviously, no pure function can call an impure function while guaranteeing that no side-effects occur, so we have a function-flagging problem here. We wish to see what the effects of this are.
