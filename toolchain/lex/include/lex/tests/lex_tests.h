#ifndef VIPER_TOOLCHAIN_LEX_TESTS_LEX_TESTS_H
#define VIPER_TOOLCHAIN_LEX_TESTS_LEX_TESTS_H

#include <optional>
#include <string>
namespace viper::toolchain::lex
{
    auto lexKeywordTest() -> std::optional<std::string>;

    auto lexSymbolTest() -> std::optional<std::string>;

    auto lexIdentifiers() -> std::optional<std::string>;
} // namespace viper::toolchain::lex

#endif // VIPER_TOOLCHAIN_LEX_TESTS_LEX_TESTS_H
