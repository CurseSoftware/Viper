#ifndef VIPER_TOOLCHAIN_DIAGNOSTICS_CONSUMER_H
#define VIPER_TOOLCHAIN_DIAGNOSTICS_CONSUMER_H
#include "diagnostic.h"
#include <ostream>

namespace viper::toolchain::diagnostics
{
    // A consumer handles the diagnostics emitted from an `Emitter`
    class Consumer
    {
        public:
            virtual ~Consumer() = default;

            virtual auto handleDiagnostic(Diagnostic diagnostic) noexcept -> void = 0;
        private:
    };

    class StreamConsumer : public Consumer
    {
        public:
            [[nodiscard]] explicit StreamConsumer(std::ostream& stream)
                : _stream{ stream }
            {}

            auto handleDiagnostic(Diagnostic diagnostic) noexcept -> void override;
        
        private:
            std::ostream& _stream;
    };
} // namespace viper::toolchain::diagnostics

#endif // VIPER_TOOLCHAIN_DIAGNOSTICS_CONSUMER_H
