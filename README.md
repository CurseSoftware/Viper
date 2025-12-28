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
