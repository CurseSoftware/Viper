#ifndef VIPER_TOOLCHAIN_DIAGNOSTICS_FILE_EMITTER_H
#define VIPER_TOOLCHAIN_DIAGNOSTICS_FILE_EMITTER_H

#include "emitter.h"
namespace viper::toolchain::diagnostics
{
    class FileEmitter : public Emitter<std::string>
    {
        public:
            using Emitter::Emitter;
    };
} // namespace viper::toolchain::diagnostics

#endif // VIPER_TOOLCHAIN_DIAGNOSTICS_FILE_EMITTER_H
