#include <BenchmarkRunner.h>
#include <iostream>
#include <string>
#include <memory>

#include "Menu.h"
#include "Session.h"

#include "commands/CreateSequenceCommand.h"
#include "commands/PrintSequenceCommand.h"
#include "commands/GetSequenceElementCommand.h"
#include "commands/TryGetSequenceElementCommand.h"
#include "commands/GetSubsequenceCommand.h"
#include "commands/GetLengthCommand.h"
#include "commands/AppendSequenceCommand.h"
#include "commands/PrependSequenceCommand.h"
#include "commands/InsertSequenceCommand.h"
#include "commands/ConcatSequenceCommand.h"

#include "TestRunner.h"
#include "CliTests.h"
#include "InternalTests.h"
#include "MenuBuilder.h"
#include "SharedPtr.h"
#include "UniquePtr.h"

// Функция для запуска всех тестов
void runAllTests() {
    TestRunner runner;

    // Запуск тестов CLI
    runner.runTestGroup("CreateSequenceCommand Tests", {
            cli_tests::testCreateSequenceCommand,
            cli_tests::testCreateSequenceWithSameName
    });
    runner.runTestGroup("PrintSequenceCommand Tests", {
            cli_tests::testPrintSequenceCommand
    });
    runner.runTestGroup("AppendSequenceCommand Tests", {
            cli_tests::testAppendSequenceCommand,
            cli_tests::testAppendSequenceInvalidName
    });
    runner.runTestGroup("PrependSequenceCommand Tests", {
            cli_tests::testPrependSequenceCommand,
            cli_tests::testPrependSequenceInvalidName
    });
    runner.runTestGroup("InsertSequenceCommand Tests", {
            cli_tests::testInsertSequenceCommand,
            cli_tests::testInsertSequenceInvalidName,
            cli_tests::testInsertSequenceInvalidIndex
    });
    runner.runTestGroup("GetLengthCommand Tests", {
            cli_tests::testGetLengthCommand,
            cli_tests::testGetLengthInvalidName
    });
    runner.runTestGroup("GetSubsequenceCommand Tests", {
            cli_tests::testGetSubsequenceCommand,
            cli_tests::testGetSubsequenceInvalidName,
            cli_tests::testGetSubsequenceInvalidIndex
    });
    runner.runTestGroup("GetSequenceElementCommand Tests", {
            cli_tests::testGetSequenceElementCommand,
            cli_tests::testGetSequenceElementInvalidName,
            cli_tests::testGetSequenceElementInvalidIndex
    });
    runner.runTestGroup("TryGetSequenceElementCommand Tests", {
            cli_tests::testTryGetSequenceElementCommand,
            cli_tests::testTryGetSequenceElementInvalidName,
            cli_tests::testTryGetSequenceElementInvalidIndex
    });
    runner.runTestGroup("ConcatSequenceCommand Tests", {
            cli_tests::testConcatSequenceCommand,
            cli_tests::testConcatSequenceInvalidName
    });

    // Запуск тестов внутренней реализации
    runner.runTestGroup("SharedPtr Tests", {
                internal_tests::testSharedPtr
    });
    runner.runTestGroup("UniquePtr Tests", {
                internal_tests::testUniquePtr
    });
    runner.runTestGroup("WeakPtr Tests", {
                internal_tests::testWeakPtr
    });
    runner.runTestGroup("LinkedList Tests", {
            internal_tests::testLinkedList
    });
    runner.runTestGroup("MutableListSequence Tests", {
            internal_tests::testMutableListSequence
    });

    runner.printResults();
}

void benchmark(int iterations) {
    BenchmarkRunner benchmarkRunner;

    benchmarkRunner.registerBenchmark("RawPtr Creation", [iterations]() {
        int* ptr = new int(42);
        delete ptr;
    });

    benchmarkRunner.registerBenchmark("SharedPtr Creation", [iterations]() {
        SharedPtr<int> ptr(new int(42));
    });
    benchmarkRunner.registerBenchmark("Std_SharedPtr Creation", [iterations]() {
        std::shared_ptr<int> ptr(new int(42));
    });

    benchmarkRunner.registerBenchmark("UniquePtr Creation", [iterations]() {
        UniquePtr<int> ptr(new int(42));
    });
    benchmarkRunner.registerBenchmark("Std_UniquePtr Creation", [iterations]() {
        std::unique_ptr<int> ptr(new int(42));
    });


    benchmarkRunner.runBenchmarks(iterations);
    benchmarkRunner.printResults();
    benchmarkRunner.saveResultsToFile("../benchmarks/benchmark_results.csv");
}

// Функция для инициализации и запуска CLI
void runCLI() {
        cli::Menu<int, double, std::string> mainMenu =
            cli::MenuBuilder<int, double, std::string>("Main Menu")
                .addCommand("create", new cli::CreateSequenceCommand<int, double, std::string>())
                .addCommand("print", new cli::PrintSequenceCommand<int, double, std::string>())
                .addCommand("get", new cli::GetSequenceElementCommand<int, double, std::string>())
                .addCommand("tryGet", new cli::TryGetSequenceElementCommand<int, double, std::string>())
                .addCommand("getSubsequence", new cli::GetSubsequenceCommand<int, double, std::string>())
                .addCommand("length", new cli::GetLengthCommand<int, double, std::string>())
                .addCommand("append", new cli::AppendSequenceCommand<int, double, std::string>())
                .addCommand("prepend", new cli::PrependSequenceCommand<int, double, std::string>())
                .addCommand("insertAt", new cli::InsertSequenceCommand<int, double, std::string>())
                .addCommand("concat", new cli::ConcatSequenceCommand<int, double, std::string>())
        .build();

        // Запуск сессии CLI
        cli::Session<int, double, std::string> session(mainMenu);
        session.run();
}

int main() {
    bool runTests = true;

    if (runTests) {
        runAllTests();
        benchmark(1000000); // 1 млн итераций, результат усредняется
    } else {
        runCLI();
    }
    return 0;
}