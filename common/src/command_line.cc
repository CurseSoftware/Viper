#include "command_line.h"
#include <memory>

namespace viper::common
{
    // Since this is for printing, we can get away with some 
    // useful recursion here
    // NOLINTNEXTLINE(misc-no-recursion)
    auto ArgumentParser::getArgumentString() const -> std::string
    {
        std::string str {};

        for (const auto& [name, parser] : _subparsers)
        {
            str += name + ": " + parser.getArgumentString();
        }

        for (const auto& [name, argument] : _arguments)
        {
            str += argument->toString();
        }

        return str;
    }

    auto ArgumentParser::addSubparser(std::string name) -> ArgumentParser&
    {
        auto [it, second] = _subparsers.emplace(name, ArgumentParser());

        return it->second;
    }

    auto ArgumentParser::registerArg(std::shared_ptr<Argument> argument) -> void {
        _arguments.insert(std::make_pair(argument->name(), argument));
    }

    auto ArgumentParser::parse() -> void
    {
        // TODO
    }

    auto PositionalArgument::toString() const noexcept -> std::string
    {
        return _name + ": " + _description;
    }
    
    auto ChoiceArgument::toString() const noexcept -> std::string
    {
        std::string str {};

        std::size_t i = 0;
        for (const auto& choice : _choices)
        {
            str += choice->toString() + " | ";
            if (i < _choices.size() + 1)
            {
                str += " | ";
            }
            
            i++;
        }

        return str;
    }
} // namespace viper::common
