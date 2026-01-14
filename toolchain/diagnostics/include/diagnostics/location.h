#ifndef VIPER_TOOLCHAIN_DIAGNOSTICS_LOCATION_H
#define VIPER_TOOLCHAIN_DIAGNOSTICS_LOCATION_H

#include <cstdint>
#include <string_view>
namespace viper::toolchain::diagnostics
{
    class [[nodiscard]] Location
    {
        public:
            explicit Location(
                std::string_view file_name
                , std::string_view line
                , int32_t line_number
                , int32_t col_number
                , int32_t length
            ) noexcept
                : _file_name{ file_name }
                , _line{ line }
                , _line_num{ line_number }
                , _col_num{ col_number }
                , _length{ length }
            {}

            auto file_name() const noexcept -> std::string_view { return _line; }
            auto line() const noexcept -> std::string_view { return _line; }
            auto line_number() const noexcept -> int32_t { return _line_num; }
            auto column_number() const noexcept -> int32_t { return _col_num; }
            auto length() const noexcept -> int32_t { return _length; }
        
        private:
            // Name of the file
            std::string_view _file_name;

            // Content of the line for this diagnostic
            std::string_view _line;

            // Line number of the diagnostic
            int32_t _line_num { -1 };

            // Column number of the diagnostic
            int32_t _col_num { -1 };

            // length of the diagnostic
            // This should always be at least 1 since there cannot be
            // a diagnostic with nothing
            int32_t _length { 1 };
    };
} // namespace viper::toolchain::diagnostics

#endif // VIPER_TOOLCHAIN_DIAGNOSTICS_LOCATION_H
