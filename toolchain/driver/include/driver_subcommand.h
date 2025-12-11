#ifndef VIPER_TOOLCHAIN_DRIVER_DRIVER_SUBCOMMAND_H
#define VIPER_TOOLCHAIN_DRIVER_DRIVER_SUBCOMMAND_H

#include <algorithm>
#include <common/command_line.h>
#include <memory>
#include <string>
#include <string_view>


namespace viper::toolchain::driver
{
    namespace cli = common::cli;
    
    // This is an API interface for  the various 
    // commands that the driver supports
    class DriverSubcommand
    {
        public:
            [[nodiscard]] explicit DriverSubcommand() = default;

            virtual ~DriverSubcommand() = default;
            
            // Run the command
            virtual auto run() noexcept -> void = 0;

            // Get the name of the command
            virtual auto name() const noexcept -> const std::string& = 0;

            // Build the command
            virtual auto build(cli::Command&) noexcept -> void = 0;
    };
} // namespace viper::toolchain::driver

#endif // VIPER_TOOLCHAIN_DRIVER_DRIVER_SUBCOMMAND_H
