#include "diagnostic.h"
#include "kinds.h"
    
namespace viper::toolchain::diagnostics
{
    auto levelToString(Level level) noexcept -> std::string
    {
        switch (level)
        {
            case Level::Note:
                return "Note";
            case Level::Warning:
                return "Warning";
            case Level::Error:
                return "Error";
        }
    }
} // namespace viper::toolchain::diagnostics
