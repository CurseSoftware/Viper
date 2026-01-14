#include "common/format.h"
#include "consumer.h"
#include "diagnostic.h"
#include <iostream>

namespace viper::toolchain::diagnostics
{
    auto StreamConsumer::handleDiagnostic(Diagnostic diagnostic) noexcept -> void
    {
        for (const auto& message : diagnostic.messages())
        {

            _stream << format::format("{}: ", levelToString(diagnostic.level()));
            _stream << message.message << '\n';
            if (!message.location.file_name().empty())
            {
                _stream << "In file: " << message.location.file_name() << " at position " << message.location.column_number() << '\n';
            }

            if (!message.location.line().empty())
            {
                _stream << message.location.line() << '\n';
            }
        }
    }
} // namespace viper::toolchain::diagnostics
