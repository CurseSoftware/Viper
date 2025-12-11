#ifndef VIPER_COMMON_COMMAND_LINE_H
#define VIPER_COMMON_COMMAND_LINE_H

#include <charconv>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <unordered_map>
#include <functional>
#include <vector>
#include <queue>

namespace viper::common::cli
{
    // Forward declarations
    class Command;

    using CommandReceivedCallback = std::function<void()>;

    class ArgumentBase
    {
        public:
            [[nodiscard]] explicit ArgumentBase() = default;

            virtual ~ArgumentBase() = default;

            virtual auto handle(const std::string&) -> void = 0;
            virtual auto handleAbsentOptional() -> void = 0;
            virtual auto handled() const noexcept -> bool = 0;
            virtual auto name() const noexcept -> const std::string& = 0;
            virtual auto alternative() const noexcept -> const std::string& = 0;
            virtual auto toString() const noexcept -> std::string = 0;
            virtual auto isOptional() const noexcept -> bool = 0;
            virtual auto isRequired() const noexcept -> bool = 0;

            auto match(std::string_view to_match) const noexcept -> bool
            {
                return to_match == name() || to_match == alternative();
            }
    };

    class CommandLine
    {
        // Constructors
        public:
            [[nodiscard]] explicit CommandLine() = default;
            CommandLine(const CommandLine&) = default;
            CommandLine(CommandLine&&) = default;
            ~CommandLine() = default;

            auto operator=(const CommandLine&) -> CommandLine& = default;
            auto operator=(CommandLine&&) -> CommandLine& = default;
            
        // Methods
        public:
            // Create a subcommand to be configured
            [[nodiscard]] auto addCommand(const std::string& name,  const std::string& description = "") -> Command&; 

            // Get the usage string for all the commands
            [[nodiscard]] auto getUsageString() const noexcept -> std::string;
            
            // Parse the commands
            auto parseCommands(int argc, char** argv) -> void;

            // Run the command that was received from the command line
            auto runReceivedCommand() const noexcept -> void;

        private:
            std::unordered_map<std::string, Command> _commands {};
            std::queue<std::string> _input_arguments {};
    };

    template <typename T>
    class Argument : public ArgumentBase
    {
        // Factory functions for creating arguments
        public:
            // Create an argument that may optionally appear.
            // This requires a defaulted value for the case
            // when it is not present.
            [[nodiscard]] static auto makeOptional(
                const std::string& name
                , const std::string& alt
                , const std::string& description
                , T default_value
            ) noexcept -> std::shared_ptr<Argument<T>>
            {
                constexpr bool REQUIRED = false; 
                return std::shared_ptr<Argument<T>>(
                    new Argument<T>(
                        name,
                        alt,
                        description,
                        REQUIRED,
                        default_value
                    )
                );
            }
            
            // Create an argument that is required to appear
            [[nodiscard]] static auto makeRequired(
                const std::string& name
                , const std::string& alt
                , const std::string& description
            ) noexcept -> std::shared_ptr<Argument<T>>
            {
                constexpr bool REQUIRED = true;
                return std::shared_ptr<Argument<T>>(
                    new Argument<T>(
                        name,
                        alt,
                        description,
                        REQUIRED
                    )
                );
            }

        // Getters
        public:
            // Whether or not this argument has been handled
            [[nodiscard]] auto handled() const noexcept -> bool override { return _handled; }

            // The name of this argument
            [[nodiscard]] auto name() const noexcept -> const std::string& override { return _name; }

            // The alternative (usually shortened) name of this argument
            [[nodiscard]] auto alternative() const noexcept -> const std::string& override { return _alt; }

            // Get the stringified version of this argument
            [[nodiscard]] auto toString() const noexcept -> std::string override 
            {
                std::string str = "[" + _name;
                if (_alt != "")
                {
                    str += " | " + _alt;
                }
                str += "]";
                return str;
            }

            // Return `true` if this argument is optional
            [[nodiscard]] auto isOptional() const noexcept -> bool override { return !_required; }

            // Return `true` if this argument is required
            [[nodiscard]] auto isRequired() const noexcept -> bool override { return _required; }

            // Get the parsed value from the argument.
            // This will throw if the argument is not
            // considered handled, even if it is a default value.
            //
            // This is because that value can change depending on
            // whatever was parsed.
            [[nodiscard]] auto get() const -> T;

        // Setters & Mutators
        public:
            // Used for setting `_handled` to `true` for optional arguments that were
            // not present when parsing. This allows the values to be read from optional
            // arguments after parsing when they were not present.
            auto handleAbsentOptional() -> void override
            {
                if (_required)
                {
                    throw std::runtime_error("Cannot set handling for non-optional argument");
                }

                _handled = true;
            }

            // Handle the given input text
            auto handle(const std::string& input_text) -> void override
            {
                if (_handled)
                {
                    throw std::runtime_error("Argument " + _name + " has already been handled.");
                }

                
                try {
                    _value = handleImpl(input_text);
                    _handled = true;
                } catch (std::runtime_error& err)
                {
                    throw err;
                }
            }

        private:
            auto handleImpl(const std::string& str) -> T
            {
                if constexpr (std::is_integral_v<T>)
                {
                    T out {};
                    auto result = std::from_chars(str.data(), str.data() + str.size(), out);
                    if (result.ec != std::errc{})
                    {
                        throw std::runtime_error("Failed to parse integral value from input: {" + str + "} for argument: [" + _name + "]");
                    }

                    return out;
                }

                else if constexpr (std::is_floating_point_v<T>)
                {
                    return static_cast<T>(std::stod(str));
                }

                else if constexpr (std::is_same_v<T, std::string>)
                {
                    return str;
                }

                else 
                {
                    static_assert(false, "Unsupported parsing type");
                }
            }

        // Private constructors
        private:
            // Private constructor for creating an argument
            [[nodiscard]] explicit Argument(
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
            
            // Private constructor for creating an argument with a defaulted value
            [[nodiscard]] explicit Argument(
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

        // Member fields
        private:
            // The name of the argument
            std::string _name {};

            // The alternative (usually shortened) form of the argument
            std::string _alt {};

            // The usage description of the argument
            std::string _description {};

            // Flag for whether this argument is required or not
            bool _required { true };

            // Flag for whether this argument has been handled or not
            bool _handled{ false };

            // The value read for this argument
            std::optional<T> _value { std::nullopt };
    };

    template <typename T>
    class ArgumentPromise
    {
        public:
            [[nodiscard]] explicit ArgumentPromise(std::shared_ptr<ArgumentBase> arg)
                : _argument{ arg }
            {}

            // Get the value from the promise
            auto get() const -> T;

        private:
            // The argument that this is a promise for
            std::weak_ptr<ArgumentBase> _argument;
    };

    class Command
    {
        // Constructors
        public:
            [[nodiscard]] explicit Command(std::string_view name, std::string_view description) noexcept
                : _name{ name }
                , _description{ description }
            {}
            Command(const Command&) = default;
            Command(Command&&) = default;
            ~Command() = default;
            
            auto operator=(const Command&) -> Command& = default;
            auto operator=(Command&&) -> Command& = default;

        // Getters
        public:
            // Get the name of this command
            auto name() -> const std::string& { return _name; }

            // Return whether or not the command has been parsed or not.
            [[nodiscard]] auto hasBeenParsed() -> bool;

            // Get the usage string for the command
            [[nodiscard]] auto getUsageString() const noexcept -> std::string;
        
        // Methods
        public:
            // Register an optional argument for this command.
            // This argument must supply a default value for 
            // the case when it is not present
            template <typename T>
            [[nodiscard]] auto addOptional(
                const std::string& name
                , const std::string& alt
                , const std::string& description
                , T default_value
            ) -> ArgumentPromise<T>
            {
                auto argument = Argument<T>::makeOptional(
                    formatArgumentName(name).value(),
                    alt,
                    description,
                    default_value
                );
                _arguments.push_back(argument);
            }
            
            // Register a required argument for this command
            template <typename T>
            [[nodiscard]] auto addOptional(
                const std::string& name
                , const std::string& description
                , T default_value
            ) -> ArgumentPromise<T>
            {
                auto argument = Argument<T>::makeOptional(
                    formatArgumentName(name).value(),
                    formatAltName(name).value(),
                    description,
                    default_value
                );
                
                _arguments.push_back(argument);
                return ArgumentPromise<T>(argument);
            }
            
            // Register a required argument for this command
            // when there is a specified alternative form specified
            template <typename T>
            [[nodiscard]] auto addRequired(
                const std::string& name
                , const std::string& alt
                , const std::string& description
            ) -> ArgumentPromise<T>
            {
                auto argument = Argument<T>::makeRequired(
                    formatArgumentName(name).value(),
                    alt,
                    description
                );
                
                _arguments.push_back(argument);
                return ArgumentPromise<T>(argument);
            }
            
            // Register a required argument for this command
            template <typename T>
            [[nodiscard]] auto addRequired(
                const std::string& name
                , const std::string& description
            ) -> ArgumentPromise<T>
            {
                auto argument = Argument<T>::makeRequired(
                    formatArgumentName(name).value(),
                    formatAltName(name).value(),
                    description
                );
                
                _arguments.push_back(argument);
                return ArgumentPromise<T>(argument);
            }

            // Parse the arguments for this command.
            // This does not include the name of the command itself
            auto parseCommand(std::queue<std::string>&) -> void;

        // Private methods
        private:
            // Find the match for the input argument in the registered arguments.
            // If there is no match, then it returns `std::nullopt`
            [[nodiscard]] auto findMatch(std::string_view) -> std::optional<std::shared_ptr<ArgumentBase>>;

            // This sets all optional arguments that were not
            // present to `handled` so that their values can be 
            // properly read from.
            auto handleRemainingOptionalArgs() -> void;

            [[nodiscard]] auto formatArgumentName(const std::string&) const noexcept -> std::optional<std::string>;
            
            [[nodiscard]] auto formatAltName(const std::string&) const noexcept -> std::optional<std::string>;
        
        private:
            // The name of the command
            std::string _name {};
            
            // The description of the command
            std::string _description {};
            
            // The list of arguments attached to this command
            std::vector<std::shared_ptr<ArgumentBase>> _arguments {};

            bool _been_parsed { false };
    };
} // namespace viper::common::cli

#endif // VIPER_COMMON_COMMAND_LINE_H
