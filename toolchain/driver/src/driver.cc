#include "driver.h"
#include "common/command_line.h"
#include "diagnostics/consumer.h"
#include "driver_subcommand.h"
#include "lex_subcommand.h"
#include "diagnostics/diagnostic.h"
#include "diagnostics/kinds.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <system_error>
#include <utility>

namespace viper::toolchain::driver
{
    // Constructor
    Driver::Driver()
        : _consumer{ std::make_shared<diagnostics::StreamConsumer>(std::cout) }
        , _emitter{ _consumer }
    {
        addSubcommands();
        buildSubcommands();
    }

    auto Driver::addSubcommands() noexcept -> void
    {
        _commands.emplace(
            LexCommand::CommandName
            , std::make_unique<LexCommand>()
        );

        // TODO: add the other commands as they are implemented
    }
    
    auto Driver::buildSubcommands() noexcept -> void
    {
        for (auto& [name, subcommand] : _commands)
        {
            subcommand->build(_command_line.addCommand(name));
        }
    }

    auto Driver::run(int argc, char** argv) -> void
    {
        try
        {
            auto parsed_command = _command_line.parseCommands(argc, argv);

            auto it = _commands.find(parsed_command.parsed_command_name);
            if(it == _commands.end())
            {
                throw std::runtime_error(parsed_command.parsed_command_name);
            }

            it->second->run();
        } catch (std::runtime_error& err)
        {
            diagnoseInvalidCommand(err.what());
            std::cout << _command_line.getUsageString() << "\n";
        }
    }

    auto Driver::diagnoseInvalidCommand(const std::string& command) noexcept -> void
    {
        auto diag = diagnostics::make_diagnostic<diagnostics::InvalidCommandDiagnostic>(diagnostics::Level::Error, std::string(command));
        _emitter.emit(diag);
    }
} // namespace viper::toolchain::driver
