#ifndef VIPER_TOOLCHAIN_DIAGNOSTICS_KINDS_H
#define VIPER_TOOLCHAIN_DIAGNOSTICS_KINDS_H
#include "diagnostic.h"
#include <__format/format_functions.h>
#include <string>

namespace viper::toolchain::diagnostics
{
    // Diagnostic for when an invalid token is encountered while lexing
    class InvalidTokenDiagnostic : public DiagnosticBase<InvalidTokenDiagnostic, int, std::string>
    {
        public:
            auto kind() const -> std::string { return "InvalidToken"; }

            static constexpr std::format_string<int, std::string> Fmt { "Invalid Token: {} {}" };
        private:
    };
} // namespace viper::toolchain::diagnostics

#endif // VIPER_TOOLCHAIN_DIAGNOSTICS_KINDS_H
