#include "emit.h"


namespace viper
{
    auto emit(std::ostream& out, std::string_view data) -> void
    {
        out << data;
    }
} // namespace viper
