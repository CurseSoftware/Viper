#include "command_line.h"

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
        std::string usage {};
        
        for (const auto& [name, command] : _commands)
        {
            usage += name + ": " + command.getUsageString() + "\n";
        }

        return usage;
    }

    auto Command::getUsageString() const noexcept -> std::string
    {
        std::string usage {};

        for (auto* const arg : _commands)
        {
            usage += "[" + arg->name() + "]";
        }

        return usage;
    }
} // namespace viper::common::cli
