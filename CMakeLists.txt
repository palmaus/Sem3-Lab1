cmake_minimum_required(VERSION 3.10)
project(Sem3-Lab1)

set(CMAKE_CXX_STANDARD 17)

include_directories(benchmarks/)
include_directories(include/project)
include_directories(include/cli)
include_directories(include/cli/commands)
include_directories(test/include)

add_executable(Sem3-Lab1
        src/main.cpp
        src/cli/Session.cpp
        src/cli/Parser.cpp
        src/cli/Menu.cpp
        src/cli/Output.cpp
        test/exec/TestRunner.cpp
        test/exec/InternalTests.cpp
        benchmarks/BenchmarkRunner.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)
target_link_libraries(Sem3-Lab1 PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)
