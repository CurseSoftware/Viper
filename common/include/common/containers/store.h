#ifndef VIPER_COMMON_CONTAINERS_STORE_H
#define VIPER_COMMON_CONTAINERS_STORE_H

#include <array>
#include <bit>
#include <compare>
#include <concepts>
#include <cstdint>
#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace viper::containers
{
    struct AnyId
    {
        static constexpr std::int32_t NoneIndex { -1 };

        AnyId() = delete;
        
        constexpr explicit AnyId(int32_t index) : index{ index } {}

        constexpr auto hasValue() const noexcept -> bool { return index != -1; }

        int32_t index;
    };

    template <typename T>
    struct Id : public AnyId
    {
        using AnyId::AnyId;

        static const T& None;

        friend constexpr auto operator==(Id<T> lhs, Id<T> rhs) -> bool
        {
            return lhs.index == rhs.index;
        }
    };

    template <typename T>
    constexpr const T& Id<T>::None = Id(NoneIndex);

    template <typename T>
    struct Index : public Id<T>
    {
        using Id<T>::Id;
        
        friend auto operator<=>(Index<T> lhs, Index<T> rhs) noexcept -> std::strong_ordering
        {
            return lhs.index <=> rhs.index;
        }
    };

    template<typename IdType, typename ValueType>
    class [[nodiscard]] ValueStore
    {
        public:
            explicit ValueStore() noexcept = default;

            using Value = std::remove_cvref_t<ValueType>;
            using ValueRef = std::conditional_t<std::same_as<std::string_view, ValueType>, std::string_view, Value&>;
            using ConstRef = std::conditional_t<std::same_as<std::string_view, ValueType>, std::string_view, const Value&>;

        // API
        public:
            auto add(ValueType value) -> IdType
            {
                IdType id = IdType(static_cast<int32_t>(_values.size()));
                _values.push_back(value);

                return id;
            }

            auto get(IdType id) -> std::optional<ValueRef>
            {
                [[likely]] if (id.index < _values.size())
                {
                    return _values[id.index];
                }
            }

            auto get(IdType id) const -> std::optional<ConstRef>
            {
                [[likely]] if (id.index < _values.size())
                {
                    return _values[id.index];
                }
            }

            auto size() const noexcept -> std::size_t { return _values.size(); }

        // Iterator
        public:
            using StorageT = std::vector<ValueType>;
            using Iterator = StorageT::iterator;
            using ConstIterator = StorageT::const_iterator;

            auto begin() -> Iterator { return _values.begin(); }
            auto end() -> Iterator { return _values.end(); }

            auto begin() const -> ConstIterator { return _values.begin(); }
            auto end() const -> ConstIterator { return _values.end(); }
            auto cbegin() const -> ConstIterator { return _values.cbegin(); }
            auto cend() const -> ConstIterator { return _values.cend(); }
        
        private:
            StorageT _values {};
    };

} // namespace viper::containers

#endif // VIPER_COMMON_CONTAINERS_STORE_H
