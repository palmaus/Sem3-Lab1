cmake_minimum_required(VERSION 3.10)
project(Sem3-Lab1)

set(CMAKE_CXX_STANDARD 17)

# Добавляем include директории

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
        src/cli/commands/CreateSequenceCommand.cpp
        src/cli/commands/PrintSequenceCommand.cpp
        src/cli/commands/GetSequenceElementCommand.cpp
        src/cli/commands/TryGetSequenceElementCommand.cpp
        src/cli/commands/GetSubsequenceCommand.cpp
        src/cli/commands/GetLengthCommand.cpp
        src/cli/commands/AppendSequenceCommand.cpp
        src/cli/commands/PrependSequenceCommand.cpp
        src/cli/commands/InsertSequenceCommand.cpp
        src/cli/commands/ConcatSequenceCommand.cpp
        test/exec/CliTests.cpp
        test/exec/TestRunner.cpp
        test/exec/InternalTests.cpp
        benchmarks/BenchmarkRunner.cpp
)

