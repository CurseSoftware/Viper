#ifndef VIPER_TOOLCHAIN_BASE_SHARED_VALUES_H
#define VIPER_TOOLCHAIN_BASE_SHARED_VALUES_H

#include "base/canonical_values.h"
#include "base/int.h"
#include "base/real.h"
#include "base/string_literal_store.h"
#include "common/format.h"
#include <iostream>

namespace viper::toolchain::base
{
    
    class [[nodiscard]] SharedValues
    {
        public:
            explicit SharedValues() noexcept = default;
            
            // No copy
            SharedValues(const SharedValues&) = delete;
            auto operator=(const SharedValues&) -> SharedValues& = delete;
            
            // Movable
            SharedValues(SharedValues&&) = default;
            auto operator=(SharedValues&&) noexcept -> SharedValues& = default;

        public:
            using StringLiteralStore = containers::Store<StringLiteralIndex, std::string_view>;
            using IdentifierStore = CanonicalStore<IdentifierId, std::string_view>;
            using RealStore = containers::Store<RealId, Real>;

            auto identifiers() noexcept -> IdentifierStore& { return _identifiers; }
            auto identifiers() const noexcept -> const IdentifierStore& { return _identifiers; }
            
            auto string_literals() noexcept -> StringLiteralStore& { return _string_literals; }
            auto string_literals() const noexcept -> const StringLiteralStore& { return _string_literals; }
            
            auto integers() noexcept -> IntStore& { return _integers; }
            auto integers() const noexcept -> const IntStore& { return _integers; }
            
            auto reals() noexcept -> RealStore& { return _reals; }
            auto reals() const noexcept -> const RealStore& { return _reals; }


            auto dump() const noexcept -> void
            {
                std::cout << "Identifiers: \n";
                for (const auto& identifier : _identifiers)
                {
                    std::cout << format::format("\t{}\n", identifier);
                }

                std::cout << "Reals: \n";
                for (const auto& real : _reals)
                {
                    std::cout << format::format("\tMantissa: {}. Exponent: {}\n", real.mantissa, real.exponent);
                }

                std::cout << "Integers: \n";
                for (const auto& integer : _integers)
                {
                    std::cout << format::format("\t{}\n", integer);
                }
            }

        private:
            StringLiteralStore _string_literals;
            IdentifierStore _identifiers;
            IntStore _integers;
            RealStore _reals;
    };
} // namespace viper::toolchain::base

#endif // VIPER_TOOLCHAIN_BASE_SHARED_VALUES_H
