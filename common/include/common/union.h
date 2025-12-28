#ifndef VIPER_COMMON_UNION_H
#define VIPER_COMMON_UNION_H

#include <type_traits>
namespace viper
{
    // This is a class to use `std::variant` or Rust enum-like
    // data
    class Union
    {
        public:
        private:
    };

    namespace detail
    {
        template <typename T, typename ...Pack>
        struct PackIndex;

        template <typename T, typename ...Ts>
        struct PackIndex<T, T, Ts...> : std::integral_constant<std::size_t, 0>
        {};

        template <typename T, typename U, typename ...Ts>
        struct PackIndex<T, U, Ts...> : std::integral_constant<std::size_t, 1 + PackIndex<T, Ts...>::value>
        {};

        template <typename T, typename ...Rest>
        union RecursiveUnion
        {
            T _t;
            RecursiveUnion<Rest...> rest;

            template <typename U>
            RecursiveUnion( std::integral_constant<std::size_t, 0> index, U&& u)
                : _t{ std::forward<U>(u) }
            { (void)index; }
        };

//        template <std::size_t I, typename U>
//        RecursiveUnion(std::integral_constant<std::size_t, I>, U&& u)
//            : rest{ std::integral_constant<std::size_t, I-1>{}, std::forward<U>(u) }
//        {
//        }
    } // namespace detail
} // namespace viper

#endif // VIPER_COMMON_UNION_H
