#ifndef VIPER_TOOLCHAIN_LEX_NUMERIC_H
#define VIPER_TOOLCHAIN_LEX_NUMERIC_H

#include "diagnostics/emitter.h"
#include "lexer.h"
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>
#include <variant>
namespace viper::toolchain::lex
{
    class NumericLiteral
    {
        public:
            enum class Base { Decimal, Hex, Binary };

            struct IntegerValue
            {
                uint64_t value;
            };

            struct RealValue
            {
                Base base;
                uint64_t mantissa;
                uint64_t exponent;
            };

            struct Error {};

            using Value = std::variant<IntegerValue, RealValue, Error>;

            static auto lex(std::string_view text, bool can_be_real, SourcePointerEmitter& emitter) -> std::optional<NumericLiteral>;

            // Get the value for 
            [[nodiscard]] auto value() const noexcept -> Value;

            // Get the raw text for this number
            [[nodiscard]] auto text() const noexcept -> std::string_view { return _text; }

            [[nodiscard]] auto decimal_offset() const noexcept -> int16_t { return _decimal_offset; }

            [[nodiscard]] auto findValue() noexcept -> Value;
        
        private:
            explicit NumericLiteral(SourcePointerEmitter& emitter) noexcept
                : _emitter{ emitter }
            {}

            class [[nodiscard]] Parser
            {
                public:
                    struct CheckDigitResult
                    {
                        bool is_ok;
                        bool has_separators;
                    };
                    explicit Parser(NumericLiteral& literal, SourcePointerEmitter& emitter);
                    
                    auto check_ok() noexcept -> bool;
                    auto checkIntSection() noexcept -> bool;
                    auto checkFractionSection() noexcept -> bool;
                    auto checkLeadingZero() noexcept -> bool;
                    auto checkDigits(std::string_view text, Base base, bool allow_separators) noexcept -> CheckDigitResult;

                    auto is_int() const noexcept -> bool { return _literal.decimal_offset() == _literal.text().size(); }

                    auto exponent() const noexcept -> uint64_t;
                
                    auto mantissa() const noexcept -> uint64_t;
                

                // Private methods
                private:

                    auto parseInt(std::string_view text) const noexcept -> uint64_t;

                // Private members
                private:
                    Base _base;
                    NumericLiteral& _literal;
                    SourcePointerEmitter& _emitter;
                    std::string_view _int_section;
                    std::string_view _fraction_section;
                    bool _clean_mantissa { false };

                    static constexpr std::array<bool, 256> ValidDecimalDigits = [] {
                        std::array<bool, 256> table {};
                        for (char c : "0123456789")
                        {
                            table[static_cast<unsigned char>(c)] = true;
                        }
                        return table;
                    }();

                    static constexpr std::array<bool, 256> ValidHexDigits = [] {
                        std::array<bool, 256> table {};
                        for (char c : "0123456789ABCDEF")
                        {
                            table[static_cast<unsigned char>(c)] = true;
                        }
                        return table;
                    }();

                    static constexpr std::array<bool, 256> ValidBinaryDigits = [] {
                        std::array<bool, 256> table {};
                        for (char c : "01")
                        {
                            table[static_cast<unsigned char>(c)] = true;
                        }
                        return table;
                    }();
            };

        private:
            std::string_view _text;
            int16_t _decimal_offset;
            Base _base { Base::Decimal };
            bool _clean_mantissa { false };
            SourcePointerEmitter& _emitter;
    };
} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_NUMERIC_H
