#ifndef VIPER_TOOLCHAIN_DIAGNOSTICS_DIAGNOSTIC_H
#define VIPER_TOOLCHAIN_DIAGNOSTICS_DIAGNOSTIC_H

#include "common/format.h"
#include <__format/format_functions.h>
#include <concepts>
#include <format>
#include <iostream>
#include <string>
#include <vector>
namespace viper::toolchain::diagnostics
{
    enum class Level
    {
        Note,
        Warning,
        Error
    };

    // Get the string version of the `Level` enum
    auto levelToString(Level level) noexcept -> std::string;

    class [[nodiscard]] Diagnostic
    {
        public:
            explicit Diagnostic(Level level, const std::vector<std::string> messages) noexcept
                : _level{ level }
                , _messages{ messages }
            {}

            auto level() const noexcept -> Level { return _level; }

            auto messages() const noexcept -> const std::vector<std::string>& { return _messages; }
        
        private:
            Level _level {};
            std::vector<std::string> _messages {};
    };

    // Concept to define an interface for
    // all valid diagnostics
    template <typename T>
    concept DiagnosticT = requires(const T& t)
    {
        { t.kind() } -> std::convertible_to<std::string>;
        { T::Fmt };
    };

    template <typename T, typename ... Args>
    class DiagnosticBase
    {
        public:
            explicit constexpr DiagnosticBase(Args ...args)
                : _formatted{ format::format(T::Fmt, std::forward<Args>(args)...) }
            {}

            auto print() -> void { std::cout << _formatted << '\n'; }

        protected:
            constexpr void required_diagnostic_interface() const requires DiagnosticT<T>
            {
                static_cast<const T*>(this)->kind();
            }
        
        private:
            Level _level;
            std::string _formatted;
    };

    template<DiagnosticT D, typename ...Args>
    auto make_diagnostic(Args&&... args) noexcept -> DiagnosticBase<D, Args...>
    {
        return DiagnosticBase<D, Args...>(args...);
    }
} // namespace viper::toolchain::diagnostics

#endif // VIPER_TOOLCHAIN_DIAGNOSTICS_DIAGNOSTIC_H
