#ifndef VIPER_TOOLCHAIN_BASE_TOKEN_H
#define VIPER_TOOLCHAIN_BASE_TOKEN_H

#include <cstddef>
#include <string_view>
#include <string>

namespace viper::base::token
{
    class Token
    {
        public:
            Token(std::string_view text)
                : _text{text}
            {}

            auto print() -> void;

        private:
            std::string _text {};
    };
} // viper::base::token

#endif // VIPER_TOOLCHAIN_BASE_TOKEN_H
