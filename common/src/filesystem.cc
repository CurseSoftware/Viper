#include "filesystem.h"
#include <cstdint>
#include <iterator>
#include <optional>
#include <sstream>
#include <vector>

namespace viper::fs
{
    File::~File() noexcept
    {
        if (_file_stream.is_open())
        {
            close();
        }
    }

    auto File::fromPath(const FilePath & path) noexcept -> std::optional<File>
    {
        File file {};

        if (file.openStreamFromPath(path) != FileOpenResult::Success)
        {
            return {};
        }
        file._path = path;

        return std::move(file);
    }

    auto File::openStreamFromPath(const FilePath& path) noexcept ->FileOpenResult 
    {
        try
        {
            _file_stream = std::fstream(path);
        } catch (...)
        {
            // Usually a bad_alloc
            return FileOpenResult::Failed;
        }

        if (!_file_stream.is_open())
        {
            return FileOpenResult::Failed;
        }

        return FileOpenResult::Success;
    }

    auto File::close() noexcept -> void
    {
        if (_file_stream.is_open())
        {
            _file_stream.close();
        }
    }

    auto File::isValid() const noexcept -> bool
    {
        return !_file_stream.fail();
    }

    auto File::readContentToString() noexcept -> std::string
    {
        // std::string buffer { std::istreambuf_iterator<char>(_file_stream), std::istreambuf_iterator<char>() };
        _file_stream.seekg(0, std::ios::end);
        std::string buffer(_file_stream.tellg(), '\0');
        _file_stream.seekg(0);
        _file_stream.read(buffer.data(), buffer.size());
        return buffer;
    }

    auto File::readContentToBytes() noexcept -> std::vector<std::uint8_t>
    {
        // TODO: make sure that this is correct
        return std::vector<std::uint8_t>(
            std::istreambuf_iterator<char>(_file_stream),
            std::istreambuf_iterator<char>()
        );
    }
} // namespace viper::fs
