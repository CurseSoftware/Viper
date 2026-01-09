#ifndef VIPER_TOOLCHAIN_BASE_COMPILATION_UNIT_H
#define VIPER_TOOLCHAIN_BASE_COMPILATION_UNIT_H

#include <string>

namespace viper::toolchain::base
{
    class CompilationUnit
    {
        public:
            // Tokenize the compilation unit
            auto tokenize() noexcept -> void;
        private:
            // The name given to this 
            std::string _package_name {};
            
            // The internal name used by the toolchain
            std::string _internal_name {};


            // TODO: add compiler options
    };
} // namespace viper::toolchain::base

#endif // VIPER_TOOLCHAIN_BASE_COMPILATION_UNIT_H
