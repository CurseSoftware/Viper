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
    
    template <typename IdType>
    class IndexIterator
    {
        public:
            IndexIterator() = delete;
            explicit IndexIterator(IdType id) noexcept : _id{ id } {}

            friend auto operator==(const IndexIterator lhs, const IndexIterator& rhs) noexcept -> bool
            {
                return lhs._id == rhs._id;
            }

            friend auto operator<=>(const IndexIterator lhs, const IndexIterator& rhs) noexcept -> std::strong_ordering
            {
                return lhs._id <=> rhs._id;
            }

            auto operator*() const noexcept -> const IdType& { return _id; }

            friend auto operator-(const IndexIterator& lhs, const IndexIterator& rhs) noexcept -> int
            {
                return lhs._id.index - rhs._id.index;
            }

            auto operator+=(int n) noexcept -> IndexIterator&
            {
                _id.index += n;
                return *this;
            }

            auto operator-=(int n) noexcept -> IndexIterator&
            {
                _id.index -= n;
                return *this;
            }

            auto operator++() noexcept -> IndexIterator&
            {
                _id.index++;
                return *this;
            }

        private:
            IdType _id;
    };

    template<typename IdType, typename ValueType>
    class [[nodiscard]] Store
    {
        public:
            explicit Store() noexcept = default;

            using Value = std::remove_cvref_t<ValueType>;
            using ValueRef = std::conditional_t<std::same_as<std::string_view, ValueType>, std::string_view, Value&>;
            using ConstRef = std::conditional_t<std::same_as<std::string_view, ValueType>, std::string_view, const Value&>;

        // API
        public:
            auto reserve(int32_t size)
            {
                _values.reserve(size);
            }

            auto add(ValueType value) -> IdType
            {
                IdType id = IdType(static_cast<int32_t>(_values.size()));
                _values.push_back(value);

                return id;
            }

            template <typename ...Args>
            auto emplace(Args&& ...args)
            {
                IdType id = IdType(static_cast<int32_t>(_values.size()));
                _values.emplace_back(args...);
                return id;
            }

            auto get(IdType id) -> std::optional<ValueRef>
            {
                if (id.index < _values.size()) [[likely]]
                {
                    return _values[id.index];
                }

                return std::nullopt;
            }

            auto get(IdType id) const -> std::optional<ConstRef>
            {
                if (id.index < _values.size()) [[likely]]
                {
                    return _values[id.index];
                }

                return std::nullopt;
            }

            auto size() const noexcept -> std::size_t { return _values.size(); }

        public:

        // Iterator
        public:
            using StorageT = std::vector<ValueType>;
            using Iterator = StorageT::iterator;
            using ConstIterator = StorageT::const_iterator;
            
            class ValueRange
            {
                public:
                    explicit ValueRange(const Store& store) noexcept
                        : _store{ store }
                    {}
                    // auto begin() -> Iterator { return _store._values.begin(); }
                    // auto end() -> Iterator { return _store._values.end(); }

                    auto begin() const -> ConstIterator { return _store._values.begin(); }
                    auto end() const -> ConstIterator { return _store._values.end(); }
                    auto cbegin() const -> ConstIterator { return _store._values.cbegin(); }
                    auto cend() const -> ConstIterator { return _store._values.cend(); }

                private:
                    const Store& _store;
            };

            auto range() const noexcept -> ValueRange
            {
                return ValueRange(*this);
            }

            auto begin() -> IndexIterator<IdType> { return IndexIterator(IdType(0)); }
            auto end() -> IndexIterator<IdType> { return IndexIterator(IdType(_values.size() - 1)); }

            auto begin() const -> IndexIterator<IdType> { return IndexIterator(IdType(0)); }
            auto end() const -> IndexIterator<IdType> { return IndexIterator(IdType(_values.size() - 1)); }

        // Iterator over the indices
        public:
        
        private:
            StorageT _values {};
    };
} // namespace viper::containers

#endif // VIPER_COMMON_CONTAINERS_STORE_H
