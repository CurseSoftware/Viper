#include "token.h"

#include <iostream>

namespace viper::base::token
{
    
    auto Token::print() -> void
    {
        std::cout << "Token: " << _text << std::endl;
    }

} // viper::base::token
