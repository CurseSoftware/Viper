#ifndef VIPER_TOOLCHAIN_DIAGNOSTICS_EMITTER_H
#define VIPER_TOOLCHAIN_DIAGNOSTICS_EMITTER_H

#include "consumer.h"
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

                private:
                    friend class Emitter<LocationType>;
                    explicit Builder() = default;
            };

        public:
            template <typename... Args>
            auto build() noexcept -> Builder&;

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
