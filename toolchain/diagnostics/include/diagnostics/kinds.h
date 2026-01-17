#ifndef VIPER_TOOLCHAIN_DIAGNOSTICS_KINDS_H
#define VIPER_TOOLCHAIN_DIAGNOSTICS_KINDS_H
#include "diagnostic.h"
#include <__format/format_functions.h>
#include <string>

namespace viper::toolchain::diagnostics
{

// This macro defines a kind of diagnostic that can be used 
// with the make_diagnostic function.
#define VIPER_DIAGNOSTIC_KIND(name, format, ...) \
    class name##Diagnostic : public DiagnosticBase<name##Diagnostic, __VA_ARGS__>\
    { \
        public: \
            auto kind() const -> std::string { return #name; } \
            static constexpr std::format_string<__VA_ARGS__> Fmt { #format }; \
        private: \
    };

// ----------------------------- //
//       DRIVER DIAGNOSTICS      //
// ----------------------------- //
VIPER_DIAGNOSTIC_KIND(InvalidCommand, "Invalid command: {}", std::string)
VIPER_DIAGNOSTIC_KIND(InvalidCommandArgument, "Invalid argument for command '{}': {}", std::string, std::string)
VIPER_DIAGNOSTIC_KIND(FileNotFound, "File not found: {}", std::string)

// --------------------------- //
//      LEXER DIAGNOSTICS      //
// --------------------------- //
VIPER_DIAGNOSTIC_KIND(InvalidCharacters, "Found invalid characters while parsing: {}", std::string)
VIPER_DIAGNOSTIC_KIND(IntBeginsWithZero, "Integer literal begins with {}", char)
VIPER_DIAGNOSTIC_KIND(InvalidNumericCharacter, "Invalid character found for numeric literal: 0", const char&)
VIPER_DIAGNOSTIC_KIND(InvalidNumericSeparatorPosition, "Invalid position for separator in numeric literal", char)

} // namespace viper::toolchain::diagnostics

#endif // VIPER_TOOLCHAIN_DIAGNOSTICS_KINDS_H
