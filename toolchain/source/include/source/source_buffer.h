#ifndef VIPER_SOURCE_SOURCE_BUFFER_H
#define VIPER_SOURCE_SOURCE_BUFFER_H

#include "diagnostics/consumer.h"
#include "common/memory_buffer.h"
#include "common/utf-8.h"
#include <common/filesystem.h>
#include <optional>

namespace viper::toolchain::source
{
    class SourceBuffer
    {
        // Factory functions
        public:
            // Create the source buffer from a specified `viper::fs::FilePath`
            [[nodiscard]] static auto fromFilePath(const fs::FilePath& path, std::weak_ptr<diagnostics::Consumer> consumer) noexcept -> std::optional<SourceBuffer>;
            
            // Create the source buffer from a specified `viper::fs::File`
            // TOOD: create a `FileRef` util so that we can get a non-owning reference to this file rather than 
            // passing this non-const reference
            [[nodiscard]] static auto fromFile(fs::File& source_file) noexcept -> std::optional<SourceBuffer>;
        
        // Special Members
        public:
            // Non-copyable
            SourceBuffer(const SourceBuffer&) = delete;
            auto operator=(const SourceBuffer&) = delete;

            // Movable
            SourceBuffer(SourceBuffer&&) noexcept;
            auto operator=(SourceBuffer&&) noexcept -> SourceBuffer&;

        // API
        public:
            auto printAsAscii() const noexcept -> bool;
            
        // Constructor
        private:
            [[nodiscard]] explicit SourceBuffer(fs::FilePath file_path, std::unique_ptr<memory::MemoryBuffer> memory) noexcept 
                : _file_path{ std::move(file_path) }
                , _memory{ std::move(memory) }
            {}

        private:
            fs::FilePath _file_path {};

            std::unique_ptr<memory::MemoryBuffer> _memory { nullptr };
    };
} // namespace viper::toolchain::source

#endif // VIPER_SOURCE_SOURCE_BUFFER_H
