#ifndef VIPER_TOOLCHAIN_DIAGNOSTICS_EMITTER_H
#define VIPER_TOOLCHAIN_DIAGNOSTICS_EMITTER_H

#include "consumer.h"
#include "diagnostic.h"
#include <memory>

namespace viper::toolchain::diagnostics
{
    template <typename LocationType>
    class Emitter
    {
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

                        _emitter->_consumer.lock()->handleDiagnostic(std::move(_diagnostic));
                    }

                    template <typename ...Args>
                    auto emit() && -> void
                    {
                        static_assert(false, "Use emitter.build().note().emit() instead of emitter.build().emit()");
                    }

                    template <typename ...Args>
                    auto addMessage(const DiagnosticBase<Args...>& diagnostic_base) -> void
                    {
                        if (!_emitter)
                        {
                            return;
                        }

                        // _diagnostic.messages().emplace_back({.level = Level::Note, .message = diagnostic_base.formatted() });
                        _diagnostic.emplaceMessage(Level::Note, diagnostic_base.formatted());
                    }

                    template <typename ...Args>
                    auto note(const DiagnosticBase<Args...>& diagnostic_base) -> Builder&
                    {
                        addMessage(diagnostic_base);
                        return *this;
                    }

                protected:
                    template <typename ...Args>
                    explicit Builder(Emitter* emitter, const DiagnosticBase<Args...> diagnostic_base)
                        : _emitter{ emitter }
                        , _diagnostic(diagnostic_base.level())
                    {
                        addMessage(diagnostic_base);
                    }

                private:
                    friend class Emitter<LocationType>;
                    explicit Builder() = default;
                    Emitter* _emitter { nullptr };
                    Diagnostic _diagnostic;
            };

        public:
            template <typename... Args>
            auto build(const DiagnosticBase<Args...> diagnostic_base) noexcept -> Builder
            {
                return Builder(this, diagnostic_base);
            }

            template <typename... Args>
            auto emit(const DiagnosticBase<Args...> diagnostic_base) noexcept -> void
            {
                auto builder = Builder(this, diagnostic_base);
                builder.emit();
            }


        public:
            [[nodiscard]] explicit Emitter(std::shared_ptr<Consumer> consumer)
                : _consumer{ consumer }
            {}
        
        private:
            friend class Builder;

        private:
            std::weak_ptr<Consumer> _consumer;
    };
} // namespace viper::toolchain::diagnostics


#endif // VIPER_TOOLCHAIN_DIAGNOSTICS_EMITTER_H
