#ifndef VIPER_TOOLCHAIN_BASE_INT_H
#define VIPER_TOOLCHAIN_BASE_INT_H

#include "base/canonical_values.h"
#include "base/string_literal_store.h"
#include <cstdint>
namespace viper::toolchain::base
{
    class IntStore
    {
        // Special members
        public:
            [[nodiscard]] explicit IntStore() = default;

            // Not copyable
            IntStore(const IntStore&) = delete;
            auto operator=(const IntStore&) -> IntStore& = delete;
            
            // Movable
            IntStore(IntStore&&) = default;
            auto operator=(IntStore&&) -> IntStore& = default;

        // API
        public:
            [[nodiscard]] auto add(int64_t value) -> IntId
            {
                return _store.insert(value);
            }
            
            using StorageT = std::vector<int64_t>;
            using Iterator = StorageT::iterator;
            using ConstIterator = StorageT::const_iterator;
            auto size() -> std::size_t { return _store.size(); }

            auto begin() -> Iterator { return _store.begin(); }
            auto end() -> Iterator { return _store.end(); }

            auto begin() const -> ConstIterator { return _store.begin(); }
            auto end() const -> ConstIterator { return _store.end(); }
            auto cbegin() const -> ConstIterator { return _store.cbegin(); }
            auto cend() const -> ConstIterator { return _store.cend(); }

        private:
            CanonicalStore<IntId, int64_t> _store;
    };
} // namespace viper::toolchain::base

#endif // VIPER_TOOLCHAIN_BASE_INT_H
