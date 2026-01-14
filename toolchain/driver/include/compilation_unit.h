#ifndef VIPER_TOOLCHAIN_DRIVER_COMPILATION_UNIT_H
#define VIPER_TOOLCHAIN_DRIVER_COMPILATION_UNIT_H

#include "diagnostics/consumer.h"
#include "lex/tokenized_buffer.h"
#include "source/source_buffer.h"
#include <optional>
#include <string>
#include <string_view>

namespace viper::toolchain::driver
{
    class CompilationUnit
    {
        // Methods
        public:
            // Tokenize the compilation unit
            auto tokenize() noexcept -> void;

        // Special Members
        public:
            [[nodiscard]] explicit CompilationUnit(
                    std::string_view input_file_name
                    , std::weak_ptr<diagnostics::Consumer> diagnostics_consumer
            ) : _input_file_name{ input_file_name }
              , _diagnostics_consumer{ std::move(diagnostics_consumer) }
            {
                if (_diagnostics_consumer.expired())
                {
                    std::cout << "expired\n";
                }
            }

            // No copy
            CompilationUnit(const CompilationUnit&) = delete;
            auto operator=(const CompilationUnit&) -> CompilationUnit& = delete;
            
            // Movable
            CompilationUnit(CompilationUnit&&) = default;
            auto operator=(CompilationUnit&&) -> CompilationUnit& = default;

        public:
            [[nodiscard]] auto tokens() const noexcept -> const lex::TokenizedBuffer& { return _tokens.value(); }

        // Private methods
        private:
            auto getSourceBuffer(std::string_view filename) const noexcept -> std::optional<source::SourceBuffer>;
        
        // Member fields
        private:
            // The name given to this 
            std::string _package_name {};

            std::string _input_file_name {};
            
            // The internal name used by the toolchain
            std::string _internal_name {};

            std::weak_ptr<diagnostics::Consumer> _diagnostics_consumer;

            /// Initialized as steps are processed ///
            // The source buffer for this compilation unit
            std::optional<source::SourceBuffer> _source { std::nullopt };

            // Tokens for this compilation unit
            std::optional<lex::TokenizedBuffer> _tokens { std::nullopt };

            // TODO: add compiler options

            // Whether the process is still successful
            bool _success { true };
    };
} // namespace viper::toolchain::driver

#endif // VIPER_TOOLCHAIN_DRIVER_COMPILATION_UNIT_H
