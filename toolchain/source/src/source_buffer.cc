#include "source_buffer.h"
#include "common/utf-8.h"
#include <common/filesystem.h>
#include <iostream>
#include <utility>

namespace viper::source
{
    auto SourceBuffer::fromFilePath(const fs::FilePath &path) noexcept -> std::optional<SourceBuffer>
    {
        auto file_opt = fs::File::fromPath(path);
        if (!file_opt)
        {
            return std::nullopt;
        }

        auto file = std::move(file_opt.value());
        return fromFile(file);
    }

    auto SourceBuffer::fromFile(fs::File& source_file) noexcept -> std::optional<SourceBuffer>
    {
        auto memory = memory::MemoryBuffer::from(source_file.readContentToBytes());
        if (!memory)
        {
            return std::nullopt;
        }

        return SourceBuffer(source_file.path(), std::move(memory.value()));
    }
            

    SourceBuffer::SourceBuffer(SourceBuffer&& other) noexcept
    {
        _file_path = std::exchange(other._file_path, {});
        _memory = std::exchange(other._memory, nullptr);
    }
            
    auto SourceBuffer::operator=(SourceBuffer&& other) noexcept -> SourceBuffer&
    {
        if (this != &other)
        {
            _file_path = std::exchange(other._file_path, {});
            _memory = std::exchange(other._memory, nullptr);
        }

        return *this;
    }

    auto SourceBuffer::printAsAscii() const noexcept -> bool
    {
        for (std::size_t offset = 0; offset < _memory->size(); offset++)
        {
            auto read_char = encoding::utf8::readExpect1ByteChar(*_memory, offset);
            if (!read_char)
            {
                std::cout << "Invalid ascii char read at offset [" << offset << "]\n";
                return false;
            }
            std::cout << static_cast<char>(read_char.value());
        }

        return true;
    }
} // namespace viper::source
