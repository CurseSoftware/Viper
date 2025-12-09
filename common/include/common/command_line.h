#ifndef VIPER_COMMON_COMMAND_LINE_H
#define VIPER_COMMON_COMMAND_LINE_H

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>
#include <vector>
#include <queue>

namespace viper::common::cli
{
    // Forward declarations
    class Command;

    class ArgumentBase
    {
        public:
            [[nodiscard]] explicit ArgumentBase() = default;

            virtual ~ArgumentBase() = default;

            virtual auto handled() -> bool = 0;
            virtual auto name() -> const std::string& = 0;
            virtual auto alternative() -> const std::string& = 0;
            virtual auto toString() -> std::string = 0;

            auto match(std::string_view to_match) -> bool
            {
                return to_match == name() || to_match == alternative();
            }
    };

    class ArgumentParser
    {
        public:
            // Constructors
            [[nodiscard]] explicit ArgumentParser() = default;
            ArgumentParser(const ArgumentParser&) = default;
            ArgumentParser(ArgumentParser&&) = default;
            ~ArgumentParser() = default;

            auto operator=(const ArgumentParser&) -> ArgumentParser& = default;
            auto operator=(ArgumentParser&&) -> ArgumentParser& = default;

            /////////////////////////////
            //         Mutators        //
            /////////////////////////////
            
            // Add a command for the parser to parse
            [[nodiscard]] auto addCommand(const std::string& name, const std::string& description = "") -> Command&; 

            // Parse the commands
            auto parseCommands(int argc, char** argv) -> void;

            [[nodiscard]] auto getUsageString() const noexcept -> std::string;

        private:
            std::unordered_map<std::string, Command> _commands {};
    };

    template <typename T>
    class Argument : ArgumentBase
    {
        public:
            // Factory functions
            [[nodiscard]] static auto makeOptional(
                const std::string& name
                , const std::string& alt
                , const std::string& description
                , T default_value
            ) noexcept -> ArgumentBase*
            {
                constexpr bool REQUIRED = false; 
                
                return new Argument<T>(
                    name,
                    alt,
                    description,
                    REQUIRED,
                    default_value
                );
            }
            
            [[nodiscard]] static auto makeRequired(
                const std::string& name
                , const std::string& alt
                , const std::string& description
            ) noexcept -> ArgumentBase*
            {
                constexpr bool REQUIRED = true;
                return new Argument<T>(
                    name,
                    alt,
                    description,
                    REQUIRED
                );
            }

            [[nodiscard]] auto handled() noexcept -> bool override { return _handled; }
            [[nodiscard]] auto name() noexcept -> const std::string& override { return _name; }
            [[nodiscard]] auto alternative() noexcept -> const std::string& override { return _name; }
            [[nodiscard]] auto toString() noexcept -> std::string override { return "[" + _name + "]"; }
       
        private:
            [[nodiscard]] Argument(
                std::string_view name
                , std::string_view alt
                , std::string_view description
                , bool required
            ) noexcept
                : _name{ name }
                , _alt{ alt }
                , _description{ description }
                , _required{ required }
            {}
            
            [[nodiscard]] Argument(
                std::string_view name
                , std::string_view alt
                , std::string_view description
                , bool required
                , T default_value
            ) noexcept
                : _name{ name }
                , _alt{ alt }
                , _description{ description }
                , _required{ required }
                , _value{ default_value }
            {}

            std::string _name {};
            std::string _alt {};
            std::string _description {};
            bool _required { true };
            bool _handled{ false };
            std::optional<T> _value { std::nullopt };
    };

    template <typename T>
    class ArgumentPromise
    {
        public:
            [[nodiscard]] explicit ArgumentPromise(ArgumentBase* arg)
                : _argument{ arg }
            {}

        private:
            ArgumentBase* _argument;
    };

    class Command
    {
        public:
            [[nodiscard]] explicit Command(std::string name, std::string description) noexcept
                : _name{ name }
                , _description{ description }
            {}
            Command(const Command&) = default;
            Command(Command&&) = default;
            ~Command() = default;
            
            auto operator=(const Command&) -> Command& = default;
            auto operator=(Command&&) -> Command& = default;

            /////////////////////////////
            //         Mutators        //
            /////////////////////////////
            
            // Add an optional argument
            template <typename T>
            auto addOptional(
                const std::string& name
                , const std::string& alt
                , const std::string& description
                , T default_value
            ) -> ArgumentPromise<T>
            {
                auto command = Argument<T>::makeOptional(
                    name,
                    alt,
                    description,
                    default_value
                );
                _commands.push_back(command);
            }
            
            template <typename T>
            auto addRequired(
                const std::string& name
                , const std::string& alt
                , const std::string& description
            ) -> ArgumentPromise<T>
            {
                auto command = Argument<T>::makeRequired(
                    name,
                    alt,
                    description
                );
                
                _commands.push_back(command);
                return ArgumentPromise<T>(command);
            }

            [[nodiscard]] auto getUsageString() const noexcept -> std::string;

        
        private:
            std::string _name {};
            std::string _description {};
            std::vector<ArgumentBase*> _commands {};
    };

} // namespace viper::common::cli

#endif // VIPER_COMMON_COMMAND_LINE_H
