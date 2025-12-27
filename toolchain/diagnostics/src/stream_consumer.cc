#include "common/format.h"
#include "consumer.h"
#include "diagnostic.h"

namespace viper::toolchain::diagnostics
{
    auto StreamConsumer::handleDiagnostic(Diagnostic diagnostic) noexcept -> void
    {
        for (const auto& message : diagnostic.messages())
        {
            _stream << format::format("{}: ", levelToString(diagnostic.level()));
            _stream << message.message;

            _stream << '\n';
        }
    }
} // namespace viper::toolchain::diagnostics
