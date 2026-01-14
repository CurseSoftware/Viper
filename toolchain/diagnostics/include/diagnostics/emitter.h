#ifndef VIPER_TOOLCHAIN_DIAGNOSTICS_EMITTER_H
#define VIPER_TOOLCHAIN_DIAGNOSTICS_EMITTER_H

#include "consumer.h"
#include "diagnostic.h"
#include "location.h"
#include <memory>

namespace viper::toolchain::diagnostics
{
    // This is constructed by the `Emitter<T>::convertLocation(T t)` method defined by each
    // derived emitter
    // TODO: This will likely contain other information for things like sorting and potentially
    // other thing. That is why we don't just use a `Location` though that is all we have for now
    struct ConvertedLocation
    {
        Location location;
    };
    
    template <typename LocationType>
    class Emitter
    {
        // Builder class
        public:
            class [[nodiscard]] Builder
            {
                public:
                    Builder(Builder&&) noexcept = default;
                    auto operator=(Builder&&) noexcept -> Builder& = default;

                    Builder(const Builder&) = delete;
                    auto operator=(const Builder&) -> Builder& = delete;

                    template <typename ...Args>
                    auto emit() & -> void
                    {
                        if (!_emitter)
                        {
                            return;
                        }

                        if (_emitter->_consumer.expired())
                        {
                            // std::cout << "no consumer\n";
                        }
                        _emitter->_consumer.lock()->handleDiagnostic(std::move(_diagnostic));
                    }

                    template <typename ...Args>
                    auto emit() && -> void
                    {
                        static_assert(false, "Use emitter.build().note().emit() instead of emitter.build().emit()");
                    }

                    template <typename ...Args>
                    auto addMessage(LocationType location, const DiagnosticBase<Args...>& diagnostic_base) -> void
                    {
                        if (!_emitter)
                        {
                            return;
                        }

                        Message message = {
                            .level = Level::Note,
                            .location = _emitter->convertLocation(location).location,
                            .message = diagnostic_base.formatted()
                        };

                        _diagnostic.addMessage(message);

                        // _diagnostic.messages().emplace_back({.level = Level::Note, .message = diagnostic_base.formatted() });
                        // _diagnostic.emplaceMessage(Level::Note, _emitter->convertLocation(location), diagnostic_base.formatted());
                    }

                    template <typename ...Args>
                    auto note(const DiagnosticBase<Args...>& diagnostic_base) -> Builder&
                    {
                        addMessage(diagnostic_base);
                        return *this;
                    }

                protected:
                    template <typename ...Args>
                    explicit Builder(Emitter* emitter, LocationType location, const DiagnosticBase<Args...> diagnostic_base)
                        : _emitter{ emitter }
                        , _diagnostic(diagnostic_base.level())
                    {
                        addMessage(location, diagnostic_base);
                    }

                private:
                    friend class Emitter<LocationType>;
                    explicit Builder() = default;
                    Emitter* _emitter { nullptr };
                    Diagnostic _diagnostic;
            };

        // Public API
        public:
            template <typename... Args>
            auto build(const DiagnosticBase<Args...> diagnostic_base) noexcept -> Builder
            {
                return Builder(this, diagnostic_base);
            }

            template <typename... Args>
            auto emit(LocationType location, const DiagnosticBase<Args...>& diagnostic_base) noexcept -> void
            {
                auto builder = Builder(this, location, diagnostic_base);
                builder.emit();
                // std::cout << "here4\n";
            }

            // Convert the LocationType to a specific location
            virtual auto convertLocation(LocationType location) const noexcept -> ConvertedLocation = 0;


        // Special members
        public:
            [[nodiscard]] explicit Emitter(std::weak_ptr<Consumer> consumer)
                : _consumer{ std::move(consumer) }
            {}

            virtual ~Emitter() = default;
        
        private:
            friend class Builder;
            friend class NoLocationEmitter;

        private:
            std::weak_ptr<Consumer> _consumer;
    };
} // namespace viper::toolchain::diagnostics


#endif // VIPER_TOOLCHAIN_DIAGNOSTICS_EMITTER_H
