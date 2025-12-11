#ifndef VIPER_TOOLCHAIN_DRIVER_DRIVER_H
#define VIPER_TOOLCHAIN_DRIVER_DRIVER_H

#include "driver_subcommand.h"
#include <common/command_line.h>
#include <memory>
#include <unordered_map>

namespace viper::toolchain::driver
{
    class Driver
    {
        // Constructors
        public:
            [[nodiscard]] explicit Driver() noexcept;
            Driver(const Driver&) = default;
            Driver(Driver&&) = default;
            ~Driver() = default;

            auto operator=(const Driver&) -> Driver& = default;
            auto operator=(Driver&&) -> Driver& = default;

        // Methods
        public:
            auto run(int argc, char** argv) -> void;

        // Methods
        private:
            // Add the subcommands that the driver supports
            auto addSubcommands() noexcept -> void;
            
            // Build the subcommands from the command line utility
            auto buildSubcommands() noexcept -> void;

        // Member fields
        private:
            cli::CommandLine _command_line {};

            // The held commands for the driver
            std::unordered_map<std::string, std::unique_ptr<DriverSubcommand>> _commands;
    };

} // namespace viper::toolchain::driver

#endif // VIPER_TOOLCHAIN_DRIVER_DRIVER_H
