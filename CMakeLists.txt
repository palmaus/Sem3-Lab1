cmake_minimum_required(VERSION 3.10)
project(Sem3-Lab1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(benchmarks/)
include_directories(include/project)
include_directories(src/gui)
include_directories(test/include)

add_executable(Sem3-Lab1
        src/main.cpp
        test/exec/TestRunner.cpp
        test/exec/InternalTests.cpp
        benchmarks/BenchmarkRunner.cpp
        src/gui/mainwindow.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)
target_link_libraries(Sem3-Lab1 Qt6::Core Qt6::Widgets Qt6::Gui )
