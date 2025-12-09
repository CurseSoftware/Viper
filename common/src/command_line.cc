#include "command_line.h"
#include <iostream>
#include <stdexcept>

namespace viper::common::cli
{
    auto ArgumentParser::addCommand(const std::string& name, const std::string& description) -> Command&
    {
        auto cmd = Command(name, description);
        _commands.insert(std::make_pair(name, cmd));
        
        auto it = _commands.find(name);
        return it->second;
    }

    auto ArgumentParser::getUsageString() const noexcept -> std::string
    {
        std::string usage { "usage: " };
        
        std::size_t i = 0;
        for (const auto& [name, command] : _commands)
        {
            usage += "\n\t" + name + ": " + command.getUsageString();
        }

        return usage;
    }

    auto Command::getUsageString() const noexcept -> std::string
    {
        std::string usage {};

        for (const auto& arg : _arguments)
        {
            usage += arg->toString() + " ";
        }

        return usage;
    }

    auto ArgumentParser::parseCommands(int argc, char** argv) -> void
    {
        if (argc < 2 && !_commands.empty())
        {
            throw std::runtime_error("Invalid number of command line arguments");
        }

        std::cout << "Parsing commands...\n";
        for (int i = 1; i < argc; i++)
        {
            _input_arguments.emplace(argv[i]);
        }

        auto it = _commands.find(_input_arguments.front());
        if (it == _commands.end())
        {
            throw std::runtime_error("No commands found for argument: " + _input_arguments.front());
        }

        _input_arguments.pop();
        it->second.parseCommand(_input_arguments);
    }

    auto Command::parseCommand(std::queue<std::string>& arguments) -> void
    {
        while (!arguments.empty())
        {
            auto matched_opt = findMatch(arguments.front());
            if (!matched_opt)
            {
                throw std::runtime_error("No argument match found for \"" + arguments.front() + "\"");
            }

            std::cout << "Matched argument: " << arguments.front() << '\n';

            auto& matched = matched_opt.value();
            arguments.pop();

            matched->handle(arguments.front());
            arguments.pop();
        }
    }

    auto Command::findMatch(std::string_view input_argument) -> std::optional<std::shared_ptr<ArgumentBase>>
    {
        for (const auto& argument : _arguments)
        {
            if (argument->match(input_argument))
            {
                return argument;
            }
        }

        return std::nullopt;
    }

} // namespace viper::common::cli
