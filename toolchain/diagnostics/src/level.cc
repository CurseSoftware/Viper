#include "diagnostic.h"
#include "kinds.h"
    
namespace viper::toolchain::diagnostics
{
    auto levelToString(Level level) noexcept -> std::string
    {
        switch (level)
        {
            case Level::Note:
                return "note";
            case Level::Warning:
                return "warning";
            case Level::Error:
                return "error";
        }
    }
} // namespace viper::toolchain::diagnostics
