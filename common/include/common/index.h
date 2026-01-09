#ifndef VIPER_COMMON_INDEX_H
#define VIPER_COMMON_INDEX_H

#include <compare>
#include <cstdint>
namespace viper
{
    struct AnyIdBase
    {
        static constexpr int32_t NoneIndex = -1;

        AnyIdBase() = delete;
        constexpr explicit AnyIdBase(int index) : index{ index } {}

        constexpr auto hasValue() const -> bool { return index != NoneIndex; }

        int32_t index;
    };
    
    template <typename ID>
    struct IdBase : public AnyIdBase
    {
        using AnyIdBase::AnyIdBase;

        static const ID& none;

        friend constexpr auto operator==(IdBase<ID> lhs, IdBase<ID> rhs) -> bool
        {
            return lhs.index == rhs.index;
        }
    };

    template <typename ID>
    constexpr const ID& IdBase<ID>::none = ID(NoneIndex);

    template <typename ID>
    struct IndexBase : public IdBase<ID>
    {
        using IdBase<ID>::IdBase;

        friend auto operator<=>(IndexBase<ID> lhs, IndexBase<ID> rhs) -> std::strong_ordering
        {
            return lhs.index <=> rhs.index;
        }
    };
} // namespace viper

#endif // VIPER_COMMON_INDEX_H
