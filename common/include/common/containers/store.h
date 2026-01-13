#ifndef VIPER_COMMON_CONTAINERS_STORE_H
#define VIPER_COMMON_CONTAINERS_STORE_H

#include <array>
#include <bit>
#include <compare>
#include <cstdint>
#include <string_view>
#include <utility>

namespace viper::containers
{
    struct AnyId
    {
        static constexpr std::int32_t NoneIndex { -1 };

        AnyId() = delete;
        
        constexpr explicit AnyId(std::int32_t index) : index{ index } {}

        constexpr auto hasValue() const noexcept -> bool { return index != -1; }

        std::int32_t index;
    };

    template <typename T>
    struct Id : public AnyId
    {
        using AnyId::AnyId;

        static const T None;

        friend constexpr auto operator==(Id<T> lhs, Id<T> rhs) -> bool
        {
            return lhs.index == rhs.index;
        }
    };

    template <typename T>
    constexpr const T Id<T>::None = Id(NoneIndex);

    template <typename T>
    struct Index : public Id<T>
    {
        friend auto operator<=>(Index<T> lhs, Index<T> rhs) noexcept -> std::strong_ordering
        {
            return lhs.index <=> rhs.index;
        }
    };

    template<typename Id, typename Value>
    class ValueStore
    {
        public:
        private:
    };

} // namespace viper::containers

#endif // VIPER_COMMON_CONTAINERS_STORE_H
