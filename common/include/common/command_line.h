#ifndef VIPER_COMMON_COMMAND_LINE_H
#define VIPER_COMMON_COMMAND_LINE_H

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>
#include <vector>

namespace viper::common
{
    // Forward declarations
    class ArgumentParser;

    class Argument
    {
        public:
            Argument() noexcept = default;
            Argument(const Argument&) = default;
            Argument(Argument&&) = default;
            ~Argument() = default;

            auto operator=(const Argument&) -> Argument& = default;
            auto operator=(Argument&&) -> Argument& = default;

            // Stringify the arguments
            virtual auto toString() const noexcept -> std::string = 0;

            // Get the name 
            virtual auto name() const noexcept -> const std::string& = 0;

            // Set the read value of the argument when parsing
            virtual auto setValue(const std::string&) noexcept -> void = 0;

            // Get the value read when parsing
            virtual auto getValue() const noexcept -> const std::string& = 0;
    };

    class PositionalArgument : public Argument 
    {
        public:
            using Handler = std::function<void(const PositionalArgument&)>;

            PositionalArgument(
                std::string_view name
                , std::string_view description
                , Handler&& handler
            ) noexcept
                : _name{ name }
                , _description { description }
                , _handler{ handler }
            {}
            
            auto name() const noexcept -> const std::string& override { return _name; }

            auto toString() const noexcept -> std::string override;
        
        private:
            std::string _name {};
            std::string _description {};
            Handler _handler;
    };

    class ChoiceArgument : public Argument
    {
        public:
            using Handler = std::function<void(const ChoiceArgument&)>;

            [[nodiscard]] explicit ChoiceArgument(
                std::string_view name
                , std::string_view description
                , const std::vector<std::shared_ptr<Argument>>& choices
                , Handler&& handler
            ) noexcept
                : _name{ name }
                , _description{ description }
                , _choices{ choices }
                , _handler{ handler }
            {}

            [[nodiscard]] auto name() const noexcept -> const std::string& override { return _name; }

            [[nodiscard]] auto toString() const noexcept -> std::string override;

        private:
            std::string _name {};
            std::string _description {};
            std::vector<std::shared_ptr<Argument>> _choices {};
            Handler _handler;
    };
    
    class ArgumentParser
    {
        public:
            [[nodiscard]] ArgumentParser() = default;
            ArgumentParser(const ArgumentParser&) = default;
            ArgumentParser(ArgumentParser&&) = default;
            ~ArgumentParser() = default;

            auto operator=(const ArgumentParser&) -> ArgumentParser& = default;
            auto operator=(ArgumentParser&&) -> ArgumentParser& = default;

            [[nodiscard]] auto addSubparser(std::string name) -> ArgumentParser&;

            [[nodiscard]] auto getArgumentString() const -> std::string;

            auto registerArg(std::shared_ptr<Argument> arg) -> void;

            auto parse() -> void;
        
        private:
            std::unordered_map<std::string, ArgumentParser> _subparsers {};

            std::unordered_map<std::string, std::shared_ptr<Argument>> _arguments {};
    };
} // namespace viper::common

#endif // VIPER_COMMON_COMMAND_LINE_H
