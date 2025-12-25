#ifndef VIPER_COMMON_MEMORY_BUFFER_H
#define VIPER_COMMON_MEMORY_BUFFER_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <cstring>
#include <optional>
#include <span>

namespace viper::memory
{
    class MemoryBuffer
    {
        // Info
        public:
            using StorageType = std::uint8_t;
            using SizeType = std::size_t;

            static_assert(sizeof(StorageType) == 1, "The binary storage type for a memory buffer must be 1 byte");
        
        // Factory
        public:
            // Create a memory buffer from the input data.
            // This makes a copy of this data and holds it here
            [[nodiscard]] static auto from(StorageType* data, SizeType length) noexcept -> std::optional<std::unique_ptr<MemoryBuffer>>;

            // Overload for creating memory buffer from vector data directory
            [[nodiscard]] static auto from(std::vector<StorageType>&&) noexcept -> std::optional<std::unique_ptr<MemoryBuffer>>;

        // Special Members
        public:
            // Copy delete
            MemoryBuffer(const MemoryBuffer&) = delete;
            auto operator=(const MemoryBuffer&) = delete;

            // Move
            MemoryBuffer(MemoryBuffer&& other) noexcept;
            auto operator=(MemoryBuffer&& other) noexcept -> MemoryBuffer&;

            // Destructor. Frees held memory.
            ~MemoryBuffer() noexcept;


        // API
        public:
            [[nodiscard]] auto size() const noexcept -> SizeType { return _length; }
            [[nodiscard]] auto data() noexcept -> StorageType* { return _data; }
            [[nodiscard]] auto data() const noexcept -> const StorageType* { return _data; }
            [[nodiscard]] auto begin() noexcept -> StorageType* { return _data; }
            [[nodiscard]] auto end() noexcept -> StorageType* { return _data + _length; }
            [[nodiscard]] auto begin() const noexcept -> const StorageType* { return _data; }
            [[nodiscard]] auto end() const noexcept -> const StorageType* { return _data + _length; }

        // Constructors
        private:
            // Constructor is not noexcept so that the factory functions can catch
            // potential allocation throws and return `std::nullopt`
            [[nodiscard]] explicit MemoryBuffer(StorageType* data, SizeType length);

        private:
            // Pointer to the data this is holding
            StorageType* _data { nullptr };

            // The length to the end of the data
            SizeType _length   {};
    };

} // namespace viper::memory

#endif // VIPER_COMMON_MEMORY_BUFFER_H
