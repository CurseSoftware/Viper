#include "manager.h"
#include "common/format.h"
#include "test.h"
#include <chrono>

namespace viper::toolchain::testing
{
    auto Manager::registerTest(const std::string& test_name, TestFunction test_function) -> void
    {
        _tests[test_name] = test_function;
    }

    auto Manager::runAll() -> AggregateRunResult
    {
        AggregateRunResult results {};
        std::size_t tests_run { 0 };
        std::size_t total_tests { _tests.size() };
        std::size_t total_passed { 0 };
        std::size_t total_failed { 0 };

        auto total_start = std::chrono::high_resolution_clock::now();

        for (const auto& [name, test_function] : _tests)
        {
            auto test_start = std::chrono::high_resolution_clock::now();
            _out << format::format("[{}/{}] Running \"{}\": ", ++tests_run, total_tests, name);
            auto result = test_function();
            auto test_end = std::chrono::high_resolution_clock::now();

            auto run_time = std::chrono::duration_cast<std::chrono::milliseconds>(test_end - test_start);
            results.test_results[name] = {
                .passed = !result.has_value(),
                .total_time = run_time,
                .message = result
            };

            if (result.has_value())
            {
                total_failed++;
            } 
            else
            {
                total_passed++;
            }
            
            _out << format::format("{} in {} milliseconds\n", result ? "FAILED" : "PASSED", run_time.count());
        }

        auto total_end = std::chrono::high_resolution_clock::now();
        auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start);

        _out << format::format("Ran {} tests in {} milliseconds. {} Passed. {} Failed.\n", total_tests, total_duration.count(), total_passed, total_failed);

        for (const auto& [test_name, test_result] : results.test_results)
        {
            if (!test_result.passed)
            {
                _out << format::format("Test \"{}\" failed with message: {}\n", test_name, test_result.message.value());
            }
        }
        
        return results;
    }
} // namespace viper::toolchain::testing
