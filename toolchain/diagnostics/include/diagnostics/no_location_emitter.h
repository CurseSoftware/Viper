#ifndef VIPER_TOOLCHAIN_DIAGNOSTICS_NO_LOCATION_EMITTER_H
#define VIPER_TOOLCHAIN_DIAGNOSTICS_NO_LOCATION_EMITTER_H

#include "diagnostics/location.h"
#include "emitter.h"

namespace viper::toolchain::diagnostics
{
    class NoLocationEmitter : public Emitter<void*>
    {
        public:
            using Emitter::Emitter;

            template <typename... Args>
            auto emit(const DiagnosticBase<Args...>& diagnostic_base) noexcept -> void
            {
                Emitter::emit(nullptr, diagnostic_base);
            }

            auto convertLocation(void* discarded_location) const noexcept -> ConvertedLocation override
            {
                (void) discarded_location;
                
                return ConvertedLocation {
                    .location = Location(
                        "",
                        "",
                        0,
                        0,
                        1
                    )
                };
            }
        private:
    };
} // namespace viper::toolchain::diagnostics

#endif // VIPER_TOOLCHAIN_DIAGNOSTICS_NO_LOCATION_EMITTER_H
