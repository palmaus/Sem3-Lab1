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

void benchmark(int startIterations, int endIterations, int step) {
        BenchmarkRunner benchmarkRunner;

        benchmarkRunner.registerBenchmark("RawPtr Creation", []() mutable { // mutable!
            int* ptr = new int(42);
            delete ptr;
            ptr = nullptr;  // Important!
        });

        benchmarkRunner.registerBenchmark("SharedPtr Creation", []() {
            SharedPtr<int> ptr(new int(42));
        });
        benchmarkRunner.registerBenchmark("Std_SharedPtr Creation", []() {
            std::shared_ptr<int> ptr(new int(42));
        });

        benchmarkRunner.registerBenchmark("UniquePtr Creation", []() {
            UniquePtr<int> ptr(new int(42));
        });
        benchmarkRunner.registerBenchmark("Std_UniquePtr Creation", []() {
            std::unique_ptr<int> ptr(new int(42));
        });


        std::ofstream outfile("../benchmarks/benchmark_results.csv");
        if (!outfile.is_open()) {
                std::cerr << "Unable to open file: ../benchmarks/benchmark_results.csv" << std::endl;
                return;
        }
        outfile << "Benchmark,Iterations,Time(ns)\n";

        for (int iterations = startIterations; iterations <= endIterations; iterations += step) {
                benchmarkRunner.clearResults();

                benchmarkRunner.runBenchmarks(iterations); // Вызываем обычный runBenchmarks

                for (const auto& pair : benchmarkRunner.getResults()) {
                        outfile << pair.first << "," << iterations << "," << pair.second << "\n";
                }
        }

        outfile.close();
}

int main() {
    bool runTests = true;

    if (runTests) {
        runAllTests();
        benchmark(10000, 1000000, 10000); // от 10k до 1млн с шагом 10k
        return 0;
    }
}