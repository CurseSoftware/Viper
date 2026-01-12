#ifndef VIPER_COMMON_STREAMS_H
#define VIPER_COMMON_STREAMS_H

#include <ios>
#include <ostream>
#include <streambuf>
namespace viper
{
    //NOLINTNEXTLINE(
    class NullBuffer : public std::streambuf
    {
        public:
            NullBuffer() = default;

            auto underflow() -> std::streambuf::int_type override
            {
                return 0;
            }

            auto overflow(std::streambuf::int_type value) -> std::streambuf::int_type override
            {
                return traits_type::not_eof(value);
            }

            auto sync() -> int override
            {
                return 0;
            }

        private:
    };

    class NullOStream : private NullBuffer, public std::ostream
    {
        public:
            NullOStream() : std::ostream(this) {}
        private:
    };
} // namespace viper

#endif // VIPER_COMMON_STREAMS_H
