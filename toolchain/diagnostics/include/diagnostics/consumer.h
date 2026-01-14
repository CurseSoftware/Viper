#ifndef VIPER_TOOLCHAIN_DIAGNOSTICS_CONSUMER_H
#define VIPER_TOOLCHAIN_DIAGNOSTICS_CONSUMER_H
#include "diagnostic.h"
#include <memory>
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

    class ErrorTrackingConsumer : public Consumer
    {
        public:
            [[nodiscard]] explicit ErrorTrackingConsumer(std::weak_ptr<Consumer> next_consumer)
                : _next_consumer{ std::move(next_consumer) }
            {}

            auto reset() noexcept -> void { _seen_error = false; }
            [[nodiscard]] auto seen_error() const noexcept -> bool { return _seen_error; }

            auto handleDiagnostic(Diagnostic diagnostic) noexcept -> void override
            {
                _seen_error = diagnostic.level() == Level::Error ? true : _seen_error;
                _next_consumer.lock()->handleDiagnostic(std::move(diagnostic));
            }
        
        private:
            std::weak_ptr<Consumer> _next_consumer;
            bool _seen_error { false };
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
