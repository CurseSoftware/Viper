# Viper Lang

## Building the toolchain
Building the toolchain from source requires CMake.

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
