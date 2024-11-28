#include <BenchmarkRunner.h>
#include <iostream>
#include <string>
#include <memory>

#include "TestRunner.h"
#include "InternalTests.h"
#include "SharedPtr.h"
#include "UniquePtr.h"
#include "gui/mainwindow.h"
#include <QApplication>


// Функция для запуска всех тестов
void runAllTests() {
    TestRunner runner;

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
            ptr = nullptr;
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

                benchmarkRunner.runBenchmarks(iterations);

                for (const auto& pair : benchmarkRunner.getResults()) {
                        outfile << pair.first << "," << iterations << "," << pair.second << "\n";
                }
        }

        outfile.close();
}

int main(int argc, char *argv[]) {
    bool runTests = false;

    if (runTests) {
        runAllTests();
        benchmark(10000, 1000000, 10000); // от 10k до 1млн с шагом 10k
        return 0;
    } else {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
}
