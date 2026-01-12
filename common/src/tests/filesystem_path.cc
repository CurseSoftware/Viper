#include "filesystem.h"
#include "format.h"
#include "tests/filesystem_tests.h"
#include <filesystem>
#include <format>
#include <string>

namespace viper
{
    auto fileFromPath() -> std::optional<std::string>
    {
        const std::string path = "tests/simple.viper";
        auto file_opt = fs::File::fromPath(path);

        if (!file_opt)
        {
            return format::format("No file created from path \"{}\"", path);
        };

        return {};
    }

    auto fileToString() -> std::optional<std::string>
    {
        const std::string path = "tests/simple.viper";
        auto file_opt = fs::File::fromPath(path);
        
        if (!file_opt)
        {
            return format::format("No file created from path \"{}\"", path);
        }

        auto string_contents = file_opt.value().readContentToString();
        if (string_contents.empty())
        {
            return "No content for created file";
        }

        if (string_contents.size() != std::filesystem::file_size(path))
        {
            return format::format("Read size {} not same as actual size {}", string_contents.size(), std::filesystem::file_size(path));
        }

        return {};
    }

    auto fileToByteVector() -> std::optional<std::string>
    {
        const std::string path = "tests/simple.viper";
        auto file_opt = fs::File::fromPath(path);
        
        if (!file_opt)
        {
            return format::format("No file created from path \"{}\"", path);
        }

        auto vec_contents = file_opt.value().readContentToBytes();
        if (vec_contents.empty())
        {
            return format::format("Read size {} not same as actual size {}", vec_contents.size(), std::filesystem::file_size(path));
        }

        if (vec_contents.size() != std::filesystem::file_size(path))
        {
            return format::format("Read size {} not same as actual size {}", vec_contents.size(), std::filesystem::file_size(path));
        }

        return {};
    }
} // namespace viper
