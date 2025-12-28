#ifndef VIPER_TOOLCHAIN_DRIVER_DRIVER_H
#define VIPER_TOOLCHAIN_DRIVER_DRIVER_H

#include "diagnostics/emitter.h"
#include "driver_subcommand.h"
#include <common/command_line.h>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>

namespace viper::toolchain::driver
{
    class Driver
    {
        // Constructors
        public:
            // Constructor for Driver can *technically* throw std::bad_alloc,
            // but as this class should generally only be instantiated once
            // and at the beginning of execution, this should basically never happen
            [[nodiscard]] explicit Driver();
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

            // Diagnose an invalid command from the command line
            auto diagnoseInvalidCommand(const std::string&) noexcept -> void;

            // Print the usage string
            auto emitUsage(std::ostream& out = std::cout) const noexcept -> void;

        // Member fields
        private:
            cli::CommandLine _command_line {};

            // The held commands for the driver
            std::unordered_map<std::string, std::unique_ptr<DriverSubcommand>> _commands;

            std::shared_ptr<diagnostics::Consumer> _consumer;
            
            diagnostics::Emitter<const char*> _emitter;
    };

} // namespace viper::toolchain::driver

#endif // VIPER_TOOLCHAIN_DRIVER_DRIVER_H
