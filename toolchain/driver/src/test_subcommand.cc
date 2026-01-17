#include "test_subcommand.h"
#include "common/tests/filesystem_tests.h"
#include "common/tests/memory_buffer_tests.h"
#include "lex/tests/lex_tests.h"
#include <cstddef>
#include <string>
namespace viper::toolchain::driver
{
    auto TestCommand::build(cli::Command& command) noexcept -> void
    {
        _output_arg = command.addOptional<std::string>("output", "Output stream for testing. Defaults to std::cout if not specified", NoOutputStreamSpecified);
    }

    auto TestCommand::run() noexcept -> void
    {
        // TODO: support sending this to file

        createMemoryBufferTests();
        createFilesystemTests();
        createLexTests();

        _manager.runAll();
    }

    auto TestCommand::createMemoryBufferTests() noexcept -> void
    {
        _manager.registerTest("Memory buffer simple allocation", bufferRawAllocateTestLarge);
        _manager.registerTest("Memory buffer large allocation", bufferRawAllocateTestLarge);
        _manager.registerTest("Memory buffer basic string view", bufferStringViewTest);
        _manager.registerTest("Memory buffer large string view", bufferLargeStringViewTest);
        _manager.registerTest("Memory buffer vector allocation", bufferVecAllocateTest);
        _manager.registerTest("Memory buffer large vector allocation", bufferVecAllocateTestLarge);
        _manager.registerTest("Memory buffer span cast", bufferSpanTest);
    }
    
    auto TestCommand::createFilesystemTests() noexcept -> void
    {
        _manager.registerTest("Filesystem file from path", fileFromPath);
        _manager.registerTest("Filesystem file to string", fileToString);
        _manager.registerTest("Filesystem file to vector", fileToByteVector);
    }
    
    auto TestCommand::createLexTests() noexcept -> void
    {
        _manager.registerTest("Lex Keywords", lex::lexKeywordTest);
        _manager.registerTest("Lex symbols", lex::lexSymbolTest);
        _manager.registerTest("Lex identifiers", lex::lexIdentifiers);
        _manager.registerTest("Lex numbers", lex::lexNumbers);
    }

} // namespace viper::toolchain::driver
