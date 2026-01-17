#ifndef VIPER_TOOLCHAIN_BASE_STRING_LITERAL_STORE_H
#define VIPER_TOOLCHAIN_BASE_STRING_LITERAL_STORE_H

#include "common/containers/store.h"
namespace viper::toolchain::base
{
    struct StringLiteralIndex : public containers::Index<StringLiteralIndex>
    {
        using Index::Index;
        static const StringLiteralIndex None;
    };

    struct IdentifierId : public containers::Id<IdentifierId>
    {
        using Id::Id;
        static const IdentifierId None;
    };

    struct IntId : public containers::Id<IntId>
    {
        using Id::Id;
        static const IntId None;
    };

    struct RealId : public containers::Id<IntId>
    {
        using Id::Id;
        static const RealId None;
    };
} // namespace viper::toolchain::base

#endif // VIPER_TOOLCHAIN_BASE_STRING_LITERAL_STORE_H
