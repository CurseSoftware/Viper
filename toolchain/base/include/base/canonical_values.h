#ifndef VIPER_TOOLCHAIN_BASE_CANONICAL_VALUES_H
#define VIPER_TOOLCHAIN_BASE_CANONICAL_VALUES_H

#include "common/containers/store.h"
#include <cstdint>
#include <functional>
#include <set>
#include <type_traits>
#include <unordered_map>
namespace viper::toolchain::base
{
    template <typename IdType, typename ValueType>
    class CanonicalStore
    {
        public:
            using Id = std::remove_cv_t<IdType>;
            using Value = std::remove_cv_t<ValueType>;
            using ValueRef = std::conditional_t<std::same_as<std::string_view, ValueType>, std::string_view, Value&>;
            using ConstRef = std::conditional_t<std::same_as<std::string_view, ValueType>, std::string_view, const Value&>;

        // API
        public:
            auto insert(Value value) -> IdType
            {
                auto it = _key_store.find(value);
                if (it != _key_store.end())
                {
                    return IdType(it->second);
                }

                auto key = static_cast<int32_t>(_values.size());
                _values.push_back(value);
                _key_store.emplace(_values.back(), key);
                return IdType(key);
            }

            auto lookup(Id id) -> std::optional<ValueRef>
            {
                [[likely]] if (id.index < _values.size())
                {
                    return _values.at(id.index);
                }
                
                return std::nullopt;
            }

            auto lookup(Id id) const -> std::optional<ConstRef>
            {
                [[likely]] if (id.index < _values.size())
                {
                    return _values.at(id.index);
                }
                
                return std::nullopt;
            }

        // Iterator
        public:
            using StorageT = std::vector<ValueType>;
            using Iterator = StorageT::iterator;
            using ConstIterator = StorageT::const_iterator;

            auto size() -> std::size_t { return _values.size(); }

            auto begin() -> Iterator { return _values.begin(); }
            auto end() -> Iterator { return _values.end(); }

            auto begin() const -> ConstIterator { return _values.begin(); }
            auto end() const -> ConstIterator { return _values.end(); }
            auto cbegin() const -> ConstIterator { return _values.cbegin(); }
            auto cend() const -> ConstIterator { return _values.cend(); }
        
        private:
            std::unordered_map<Value, int32_t> _key_store {};
            std::vector<Value> _values {};

    };
} // namespace viper::toolchain::base

#endif // VIPER_TOOLCHAIN_BASE_CANONICAL_VALUES_H
