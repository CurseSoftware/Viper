#include <driver.h>

namespace toolchain = viper::toolchain;

auto main(int argc, char** argv) -> int
{
    toolchain::driver::Driver program_driver {};
    program_driver.run(argc, argv);

    return 0;
}
