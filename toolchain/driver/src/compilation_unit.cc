#include "compilation_unit.h"
#include "diagnostics/diagnostic.h"
#include "diagnostics/kinds.h"
#include "source/source_buffer.h"
#include <optional>

namespace viper::toolchain::driver
{
    auto CompilationUnit::tokenize() noexcept -> void
    {
        _source = getSourceBuffer(_input_file_name);
        if (!_source)
        {
            _success = false; 
            return;
        }
    }

    auto CompilationUnit::getSourceBuffer(std::string_view filename) const noexcept -> std::optional<source::SourceBuffer>
    {
        return source::SourceBuffer::fromFilePath(filename, _diagnostics_consumer);
    }
} // namespace viper::toolchain::driver
