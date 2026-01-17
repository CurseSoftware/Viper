#include "numeric.h"
#include "characters.h"
#include <algorithm>
#include <charconv>
#include <iostream>
#include <iterator>
#include <string>

namespace viper::toolchain::lex
{
    auto NumericLiteral::lex(std::string_view text, bool can_be_real) -> std::optional<NumericLiteral>
    {
        NumericLiteral literal {};
        
        if (text.empty() || !isNumeric(text[0]))
        {
            return std::nullopt;
        }

        bool seen_decimal { false };

        for (std::size_t i = 0; i < text.size(); i++)
        {
            char c = text[i];
            if (isNumeric(c) || c == '_')
            {
                continue;
            }

            if (can_be_real && c == '.' && i+1 != text.size() && isNumeric(text[i+1]) && !seen_decimal)
            {
                literal._decimal_offset = i;
                seen_decimal = true;
                continue;
            }
        }

        literal._text = text;

        // If there is not a decimal found
        // we set the position to the end
        if (!seen_decimal)
        {
            literal._decimal_offset = text.size();
        }

        return literal;
    }

    NumericLiteral::Parser::Parser(NumericLiteral& literal)
        : _literal{ literal }
        , _base{ Base::Decimal }
    {
        _int_section = literal.text().substr(0, literal.decimal_offset());
        
        if (_int_section.starts_with("0x"))
        {
            _base = Base::Hex;
        }
        else if (_int_section.starts_with("0b"))
        {
            _base = Base::Binary;
        }

        _fraction_section = literal.decimal_offset() == literal.text().size() 
            ? "" 
            : literal.text().substr(literal.decimal_offset() + 1);
    }

    auto NumericLiteral::Parser::check_ok() noexcept -> bool
    {
        return checkLeadingZero() && checkIntSection() && checkFractionSection();
    }

    auto NumericLiteral::Parser::checkLeadingZero() noexcept -> bool
    {
        if (_base == Base::Decimal && _int_section.length() > 1 && _int_section.starts_with('0'))
        {
            return false;
        }
        // TODO: perform diagnostic

        return true;
    }

    auto NumericLiteral::Parser::parseInt(std::string_view text) const noexcept -> uint64_t
    {
        std::array<char, 32> cleaned;
        uint64_t value { 0 };
        
        if (_clean_mantissa) [[unlikely]]
        {
            std::copy_if(text.begin(), text.end(), cleaned.begin(), 
                [](char c) {
                    return isNumeric(c);
                }
            );

            auto [ptr, ec] = std::from_chars(cleaned.begin(), cleaned.end(), value);
            return value;
        }
            
        auto [ptr, ec] = std::from_chars(text.begin(), text.end(), value);
        return value;
    }

    auto NumericLiteral::Parser::mantissa() const noexcept -> uint64_t
    {
        return parseInt(_int_section);
    }

    auto NumericLiteral::Parser::exponent() const noexcept -> uint64_t
    {
        if (_fraction_section.empty())
        {
            return 0;
        }

        return parseInt(_fraction_section);
    }

    auto NumericLiteral::findValue() noexcept -> Value
    {
        Parser parser(*this);

        if (!parser.check_ok())
        {
            return Error {};
        }

        if (parser.is_int())
        {
            return IntegerValue(parser.mantissa());
        }

        return RealValue {
            .base = _base,
            .mantissa = parser.mantissa(),
            .exponent = parser.exponent()
        };
    }

    auto NumericLiteral::Parser::checkIntSection() noexcept -> bool
    {
        auto int_result = checkDigits(_int_section, _base, true);
        _clean_mantissa |= int_result.has_separators;
        return int_result.is_ok;
    }

    auto NumericLiteral::Parser::checkFractionSection() noexcept -> bool
    {
        if (is_int())
        {
            return true;
        }

        // Binary cannot include a fractional part
        if (_base == Base::Binary)
        {
            // TODO: emit diagnostic
        }

        _clean_mantissa = true;

        return checkDigits(_fraction_section, _base, false).is_ok;
    }

    auto NumericLiteral::Parser::checkDigits(std::string_view text, Base base, bool allow_separators) noexcept -> CheckDigitResult
    {
        CheckDigitResult result { .is_ok = false };
        unsigned num_separators { 0 };
        
        const auto& valid_digits = [base]() {
            switch (base) 
            {
                case Base::Decimal:
                    return ValidDecimalDigits;
                case Base::Hex:
                    return ValidHexDigits;
                case Base::Binary:
                    return ValidBinaryDigits;
            }
        }();

        for (std::size_t i = 0; i < text.size(); i++)
        {
            char c = text[i];
            if (valid_digits[static_cast<unsigned char>(c)])
            {
                continue;
            }

            if (c == '_')
            {
                if (!allow_separators || i == 0 ||  text[i-1] == '_' || i + 1 == text.size())
                {
                    // TODO: emit a diagnostic
                    return { .is_ok = false };
                }
                
                num_separators++;
                continue;
            }

            // TODO: Emit diagnostic
            return { .is_ok = false };
        }

        if (num_separators == text.size())
        {
            return { .is_ok = false };
        }

        return { .is_ok = true, .has_separators = num_separators > 0 };
    }
} // namespace viper::toolchain::lex
