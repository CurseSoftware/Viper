#ifndef VIPER_TOOLCHAIN_TESTING_MANAGER_H
#define VIPER_TOOLCHAIN_TESTING_MANAGER_H

#include "test.h"
#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
namespace viper::toolchain::testing
{
    struct TestRunResult
    {
        bool passed;
        std::chrono::milliseconds total_time;
        std::optional<std::string> message;
    };
    
    struct AggregateRunResult
    {
        std::chrono::milliseconds total_time;
        std::unordered_map<std::string, TestRunResult> test_results;
    };
    
    class [[nodiscard]] Manager
    {
        public:
            explicit Manager(std::ostream& out = std::cout) noexcept
                : _out{ out }
            {}

        public:
            struct TestRunInfo
            {
                TestRunInfo(std::string name, TestFunction& function)
                    : name{ std::move(name) }
                    , function{ function }
                {}
                std::string name;
                TestFunction function;
            };

        public:
            // Register a test to this manager
            auto registerTest(const std::string& test_name, TestFunction test_function) -> void;

            auto runAll() -> AggregateRunResult;

        private:
            std::ostream& _out;
            std::unordered_map<std::string, TestFunction> _tests {};
    };
} // namespace viper::toolchain::testing

#endif // VIPER_TOOLCHAIN_TESTING_MANAGER_H
