#ifndef VIPER_COMMON_FILESYSTEM_H
#define VIPER_COMMON_FILESYSTEM_H

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <variant>
#include <vector>

namespace viper::fs
{

    enum class FileType
    {
        StandardFile
        , Directory
        , SymLink
    };

    enum class FileAccessFlags
    {
        Exists = 0,
        Read,
        Write,
        ReadWrite,
        Exeucutable
    };

    enum class FileStatus
    {
        Open,
        Closed
    };

    using FileId = std::uint32_t;
    using FilePermissions = std::int32_t;
    using FilePath = std::filesystem::path;
    using FileName = std::string;

    enum FileOpenResult
    {
        Success,
        Failed
    };

    class File
    {
        // Special Members
        public:
            File(const File&) = delete;
            auto operator=(const File&) -> File& = delete;
            
            File(File&&) noexcept = default;
            auto operator=(File&&) noexcept -> File& = default;
            ~File() noexcept;

        // Factory
        public:
            // Create a file from a specified path
            [[nodiscard]] static auto fromPath(const FilePath&) noexcept -> std::optional<File>;

            // Read the contents of this file to a buffer of `std::byte`
            [[nodiscard]] auto readContentToBytes() noexcept -> std::vector<std::uint8_t>;

            // Read the contents of this file to a buffer of `std::string`
            [[nodiscard]] auto readContentToString() noexcept -> std::string;
        
        public:
            // Close the file
            auto close() noexcept -> void;

            // Check whether the file is still valid
            [[nodiscard]] auto isValid() const noexcept -> bool;

            // Get the file path for this file
            [[nodiscard]] auto path() -> const FilePath& { return _path; }
            
        // Constructor & Private Methods
        private:
            [[nodiscard]] explicit File() noexcept = default;

            // Open the file stream from the specified path
            auto openStreamFromPath(const FilePath&) noexcept -> FileOpenResult;

        // Member fields
        private:
            FilePath _path {};
            std::fstream _file_stream {};
    };
} // namespace viper::fs

#endif // VIPER_COMMON_FILESYSTEM_H
