#ifndef VIPER_TOOLCHAIN_BASE_SHARED_VALUES_H
#define VIPER_TOOLCHAIN_BASE_SHARED_VALUES_H

#include "base/canonical_values.h"
#include "base/string_literal_store.h"

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

            auto identifiers() noexcept -> IdentifierStore& { return _identifiers; }
            auto identifiers() const noexcept -> const IdentifierStore& { return _identifiers; }
            
            auto string_literals() noexcept -> StringLiteralStore& { return _string_literals; }
            auto string_literals() const noexcept -> const StringLiteralStore& { return _string_literals; }

        private:
            StringLiteralStore _string_literals;
            IdentifierStore _identifiers;
    };
} // namespace viper::toolchain::base

#endif // VIPER_TOOLCHAIN_BASE_SHARED_VALUES_H
