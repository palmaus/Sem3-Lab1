#include "CliTests.h"
#include "TestRunner.h"
#include "Output.h"
#include "CreateSequenceCommand.h"
#include "PrintSequenceCommand.h"
#include "AppendSequenceCommand.h"
#include "PrependSequenceCommand.h"
#include "InsertSequenceCommand.h"
#include "GetLengthCommand.h"
#include "GetSubsequenceCommand.h"
#include "GetSequenceElementCommand.h"
#include "TryGetSequenceElementCommand.h"
#include "ConcatSequenceCommand.h"
#include "MutableListSequence.h"
#include <sstream>
#include <variant>
#include <string>
#include <algorithm>

namespace cli_tests {
    template<typename T>
    bool sequencesEqual(const MutableSequence<T> *seq1, const MutableSequence<T> *seq2) {
        if (seq1->getLength() != seq2->getLength()) {
            return false;
        }
        for (int i = 0; i < seq1->getLength(); ++i) {
            if (seq1->get(i) != seq2->get(i)) {
                return false;
            }
        }
        return true;
    }

    void testCreateSequenceCommand() {
        TestRunner runner;
        std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;

        // Тест создания ListSequence
        runner.expectOutput("testCreateSequenceCommand::List sequence creation", [&sequences]() {
            cli::CreateSequenceCommand<int, double, std::string> command;
            command.execute({"list1",  "hello", "world"}, sequences);
        }, "Sequence 'list1' created.\n");
        if (sequences.find("list1") == sequences.end()) {
            throw std::runtime_error("Sequence 'list1' not found.");
        }
        if (dynamic_cast<MutableListSequence<std::variant<int, double, std::string>>*>(sequences["list1"]) == nullptr) {
            throw std::runtime_error("Sequence 'list1' is not of type ListSequence.");
        }
        if (sequences["list1"]->getLength() != 2) {
            throw std::runtime_error("Sequence 'list1' has incorrect length.");
        }
    }

    void testPrintSequenceCommand() {
        TestRunner runner;
        std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;

        std::variant<int, double, std::string> list1Data[] = {"hello", "world"};
        sequences["list1"] = new MutableListSequence<std::variant<int, double, std::string>>(list1Data, 2);

        // Тест вывода ListSequence
        runner.expectOutput("testPrintSequenceCommand::List sequence output", [&sequences]() {
            cli::PrintSequenceCommand<int, double, std::string> command;
            command.execute({"list1"}, sequences);
        }, "hello world \n");

        // Тест вывода несуществующей последовательности
        runner.expectOutput("testPrintSequenceCommand::Nonexistent sequence output", [&sequences]() {
            cli::PrintSequenceCommand<int, double, std::string> command;
            command.execute({"nonexistent"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");

        // Очистка памяти после тестов
        for (auto& pair : sequences) {
            delete pair.second;
        }
    }

    void testAppendSequenceCommand() {
        TestRunner runner;

        // Тест для ListSequence
        {
            std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
            sequences["list1"] = new MutableListSequence<std::variant<int, double, std::string>>();
            runner.expectOutput("testAppendSequenceCommand::Append to list sequence", [&sequences]() {
                cli::AppendSequenceCommand<int, double, std::string> command;
                command.execute({"list1", "hello"}, sequences);
            }, "Element appended.\n");
            if (sequences["list1"]->getLength() != 1) {
                throw std::runtime_error("Sequence 'list1' has incorrect length after appending.");
            }
            if (std::get<std::string>(sequences["list1"]->get(0)) != "hello") {
                throw std::runtime_error("Sequence 'list1' has incorrect element after appending.");
            }
            delete sequences["list1"];
        }
    }

    void testPrependSequenceCommand() {
        TestRunner runner;

        // Тест для ListSequence
        {
            std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
            std::variant<int, double, std::string> list1Data[] = {"hello", "world"};
            sequences["list1"] = new MutableListSequence<std::variant<int, double, std::string>>(list1Data, 2);
            runner.expectOutput("testPrependSequenceCommand::Prepend to list sequence", [&sequences]() {
                cli::PrependSequenceCommand<int, double, std::string> command;
                command.execute({"list1", "start"}, sequences);
            }, "Element prepended.\n");
            if (sequences["list1"]->getLength() != 3) {
                throw std::runtime_error("Sequence 'list1' has incorrect length after prepending.");
            }
            if (std::get<std::string>(sequences["list1"]->get(0)) != "start") {
                throw std::runtime_error("Sequence 'list1' has incorrect element after prepending.");
            }
            delete sequences["list1"];
        }
    }

    void testInsertSequenceCommand() {
        TestRunner runner;

        // Тест для ListSequence
        {
            std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
            std::variant<int, double, std::string> list1Data[] = {"hello", "world"};
            sequences["list1"] = new MutableListSequence<std::variant<int, double, std::string>>(list1Data, 2);
            runner.expectOutput("testInsertSequenceCommand::Insert to list sequence", [&sequences]() {
                cli::InsertSequenceCommand<int, double, std::string> command;
                command.execute({"list1", "1", "beautiful"}, sequences);
            }, "Element inserted.\n");
            if (sequences["list1"]->getLength() != 3) {
                throw std::runtime_error("Sequence 'list1' has incorrect length after inserting.");
            }
            if (std::get<std::string>(sequences["list1"]->get(1)) != "beautiful") {
                throw std::runtime_error("Sequence 'list1' has incorrect element after inserting.");
            }
            delete sequences["list1"];
        }
    }

    void testGetLengthCommand() {
        TestRunner runner;

        // Тест для ListSequence
        {
            std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
            std::variant<int, double, std::string> list1Data[] = {"hello", "world"};
            sequences["list1"] = new MutableListSequence<std::variant<int, double, std::string>>(list1Data, 2);
            runner.expectOutput("testGetLengthCommand::Get length of list sequence", [&sequences]() {
                cli::GetLengthCommand<int, double, std::string> command;
                command.execute({"list1"}, sequences);
            }, "2\n");
            delete sequences["list1"];
        }
    }

    void testGetSubsequenceCommand() {
        TestRunner runner;

        // Тест для ListSequence
        {
            std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
            std::variant<int, double, std::string> list1Data[] = {"hello", "beautiful", "world"};
            sequences["list1"] = new MutableListSequence<std::variant<int, double, std::string>>(list1Data, 3);
            runner.expectOutput("testGetSubsequenceCommand::Get subsequence of list sequence", [&sequences]() {
                cli::GetSubsequenceCommand<int, double, std::string> command;
                command.execute({"list1", "0", "1"}, sequences);
            }, "hello beautiful \n");
            delete sequences["list1"];
        }
    }

    void testGetSequenceElementCommand() {
        TestRunner runner;

        // Тест для ListSequence
        {
            std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
            std::variant<int, double, std::string> list1Data[] = {"hello", "beautiful", "world"};
            sequences["list1"] = new MutableListSequence<std::variant<int, double, std::string>>(list1Data, 3);

            // Тест получения элемента по индексу
            runner.expectOutput("testGetSequenceElementCommand::Get list sequence element by index", [&sequences]() {
                cli::GetSequenceElementCommand<int, double, std::string> command;
                command.execute({"list1", "1"}, sequences);
            }, "beautiful\n");
            // Тест получения первого элемента
            runner.expectOutput("testGetSequenceElementCommand::Get first element of list sequence", [&sequences]() {
                cli::GetSequenceElementCommand<int, double, std::string> command;
                command.execute({"list1", "first"}, sequences);
            }, "hello\n");
            // Тест получения последнего элемента
            runner.expectOutput("testGetSequenceElementCommand::Get last element of list sequence", [&sequences]() {
                cli::GetSequenceElementCommand<int, double, std::string> command;
                command.execute({"list1", "last"}, sequences);
            }, "world\n");
            delete sequences["list1"];
        }
    }

    void testTryGetSequenceElementCommand() {
        TestRunner runner;

        // Тест для ListSequence
        {
            std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
            std::variant<int, double, std::string> list1Data[] = {"hello", "beautiful", "world"};
            sequences["list1"] = new MutableListSequence<std::variant<int, double, std::string>>(list1Data, 3);

            // Тест получения элемента по индексу
            runner.expectOutput("testTryGetSequenceElementCommand::Try get list sequence element by index", [&sequences]() {
                cli::TryGetSequenceElementCommand<int, double, std::string> command;
                command.execute({"list1", "1"}, sequences);
            }, "beautiful\n");
            // Тест получения первого элемента
            runner.expectOutput("testTryGetSequenceElementCommand::Try get first element of list sequence", [&sequences]() {
                cli::TryGetSequenceElementCommand<int, double, std::string> command;
                command.execute({"list1", "first"}, sequences);
            }, "hello\n");
            // Тест получения последнего элемента
            runner.expectOutput("testTryGetSequenceElementCommand::Try get last element of list sequence", [&sequences]() {
                cli::TryGetSequenceElementCommand<int, double, std::string> command;
                command.execute({"list1", "last"}, sequences);
            }, "world\n");
            // Тест получения элемента по неверному индексу
            runner.expectOutput("testTryGetSequenceElementCommand::Try get element by invalid index", [&sequences]() {
                cli::TryGetSequenceElementCommand<int, double, std::string> command;
                command.execute({"list1", "10"}, sequences);
            }, "None\n");
            delete sequences["list1"];
        }
    }

    void testConcatSequenceCommand() {
        TestRunner runner;

        // Тест для ListSequence
        {
            std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
            std::variant<int, double, std::string> list1Data[] = {"hello", "world"};
            sequences["list1"] = new MutableListSequence<std::variant<int, double, std::string>>(list1Data, 2);
            std::variant<int, double, std::string> list2Data[] = {"beautiful", "day"};
            sequences["list2"] = new MutableListSequence<std::variant<int, double, std::string>>(list2Data, 2);
            runner.expectOutput("testConcatSequenceCommand::Concat two list sequences", [&sequences]() {
                cli::ConcatSequenceCommand<int, double, std::string> command;
                command.execute({"list1", "list2", "new_list"}, sequences); // Убрали "concat"
            }, "Sequences concatenated into 'new_list'.\n");
            if (sequences["new_list"]->getLength() != 4) {
                throw std::runtime_error("Sequence 'new_list' has incorrect length after concatenation.");
            }
            std::variant<int, double, std::string> expectedListData[] = {"hello", "world", "beautiful", "day"};
            auto expectedList = new MutableListSequence<std::variant<int, double, std::string>>(expectedListData, 4);
            if (!sequencesEqual(sequences["new_list"], expectedList)) {
                throw std::runtime_error("Sequence 'new_list' has incorrect elements after concatenation.");
            }
            delete expectedList;
            delete sequences["list1"];
            delete sequences["list2"];
            delete sequences["new_list"];
        }
    }

    void testCreateSequenceWithSameName() {
        TestRunner runner;
        std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;

        // Попытка создать последовательность с существующим именем
        sequences["lst1"] = new MutableListSequence<std::variant<int, double, std::string>>();
        runner.expectOutput("testCreateSequenceWithSameName::Sequence with same name creation", [&sequences]() {
            cli::CreateSequenceCommand<int, double, std::string> command;
            command.execute({"lst1", "array"}, sequences);
        }, "Error: Sequence with this name already exists.\n");
        delete sequences["lst1"];
    }

    void testAppendSequenceInvalidName() {
        TestRunner runner;
        std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
        runner.expectOutput("testAppendSequenceInvalidName::Append to nonexistent sequence", [&sequences]() {
            cli::AppendSequenceCommand<int, double, std::string> command;
            command.execute({"nonexistent", "5"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");
    }

    void testPrependSequenceInvalidName() {
        TestRunner runner;
        std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
        runner.expectOutput("testPrependSequenceInvalidName::Prepend to nonexistent sequence", [&sequences]() {
            cli::PrependSequenceCommand<int, double, std::string> command;
            command.execute({"nonexistent", "5"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");
    }

    void testInsertSequenceInvalidName() {
        TestRunner runner;
        std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
        runner.expectOutput("testInsertSequenceInvalidName::Insert to nonexistent sequence", [&sequences]() {
            cli::InsertSequenceCommand<int, double, std::string> command;
            command.execute({"nonexistent", "1", "5"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");
    }

    void testGetLengthInvalidName() {
        TestRunner runner;
        std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
        runner.expectOutput("testGetLengthInvalidName::Get length of nonexistent sequence", [&sequences]() {
            cli::GetLengthCommand<int, double, std::string> command;
            command.execute({"nonexistent"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");
    }

    void testGetSubsequenceInvalidName() {
        TestRunner runner;
        std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
        runner.expectOutput("testGetSubsequenceInvalidName::Get subsequence of nonexistent sequence", [&sequences]() {
            cli::GetSubsequenceCommand<int, double, std::string> command;
            command.execute({"nonexistent", "1", "3"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");
    }

    void testGetSequenceElementInvalidName() {
        TestRunner runner;
        std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;

        runner.expectOutput("testGetSequenceElementInvalidName::Get element of nonexistent sequence by index", [&sequences]() {
            cli::GetSequenceElementCommand<int, double, std::string> command;
            command.execute({"nonexistent", "2"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");

        runner.expectOutput("testGetSequenceElementInvalidName::Get first element of nonexistent sequence", [&sequences]() {
            cli::GetSequenceElementCommand<int, double, std::string> command;
            command.execute({"nonexistent", "first"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");

        runner.expectOutput("testGetSequenceElementInvalidName::Get last element of nonexistent sequence", [&sequences]() {
            cli::GetSequenceElementCommand<int, double, std::string> command;
            command.execute({"nonexistent", "last"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");
    }

    void testTryGetSequenceElementInvalidName() {
        TestRunner runner;
        std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;

        runner.expectOutput("testTryGetSequenceElementInvalidName::Try get element of nonexistent sequence by index", [&sequences]() {
            cli::TryGetSequenceElementCommand<int, double, std::string> command;
            command.execute({"nonexistent", "2"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");

        runner.expectOutput("testTryGetSequenceElementInvalidName::Try get first element of nonexistent sequence", [&sequences]() {
            cli::TryGetSequenceElementCommand<int, double, std::string> command;
            command.execute({"nonexistent", "first"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");

        runner.expectOutput("testTryGetSequenceElementInvalidName::Try get last element of nonexistent sequence", [&sequences]() {
            cli::TryGetSequenceElementCommand<int, double, std::string> command;
            command.execute({"nonexistent", "last"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");
    }

        void testConcatSequenceInvalidName() {
        TestRunner runner;
        std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
        std::variant<int, double, std::string> arr1Data[] = {1, 2, 3};
        sequences["arr1"] = new MutableListSequence<std::variant<int, double, std::string>>(arr1Data, 3);

        // Несуществующая первая последовательность
        runner.expectOutput("testConcatSequenceInvalidName::Concat nonexistent and existing sequences", [&sequences]() {
            cli::ConcatSequenceCommand<int, double, std::string> command;
            command.execute({"nonexistent", "arr1", "new_sequence"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");

        // Несуществующая вторая последовательность
        runner.expectOutput("testConcatSequenceInvalidName::Concat existing and nonexistent sequences", [&sequences]() {
            cli::ConcatSequenceCommand<int, double, std::string> command;
            command.execute({"arr1", "nonexistent", "new_sequence"}, sequences);
        }, "Error: Sequence not found: nonexistent\n");

        delete sequences["arr1"];
    }

    void testInsertSequenceInvalidIndex() {
        TestRunner runner;

        // Тест для ListSequence
        {
            std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
            std::variant<int, double, std::string> list1Data[] = {"hello", "world"};
            sequences["list1"] = new MutableListSequence<std::variant<int, double, std::string>>(
                    list1Data, 2);
            runner.expectOutput("testInsertSequenceInvalidIndex::Insert to list sequence with invalid index", [&sequences]() {
                cli::InsertSequenceCommand<int, double, std::string> command;
                command.execute({"list1", "10", "beautiful"}, sequences);
            }, "Error: Index out of range.\n");
            runner.expectOutput("testInsertSequenceInvalidIndex::Insert to list sequence with invalid index", [&sequences]() {
                cli::InsertSequenceCommand<int, double, std::string> command;
                command.execute({"list1", "-1", "beautiful"}, sequences);
            }, "Error: Index out of range.\n");
            delete sequences["list1"];
        }
    }

    void testGetSubsequenceInvalidIndex() {
        TestRunner runner;

        // Тест для ListSequence
        {
            std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
            std::variant<int, double, std::string> list1Data[] = {"hello", "beautiful", "world"};
            sequences["list1"] = new MutableListSequence<std::variant<int, double, std::string>>(
                    list1Data, 3);

            runner.expectOutput("testGetSubsequenceInvalidIndex::Get subsequence of list sequence with invalid indexes", [&sequences]() {
                cli::GetSubsequenceCommand<int, double, std::string> command;
                command.execute({"list1", "10", "12"}, sequences);
            }, "Error: Invalid start or end index.\n");

            runner.expectOutput("testGetSubsequenceInvalidIndex::Get subsequence of list sequence with invalid indexes", [&sequences]() {
                cli::GetSubsequenceCommand<int, double, std::string> command;
                command.execute({"list1", "3", "1"}, sequences);
            }, "Error: Invalid start or end index.\n");

            runner.expectOutput("testGetSubsequenceInvalidIndex::Get subsequence of list sequence with invalid indexes", [&sequences]() {
                cli::GetSubsequenceCommand<int, double, std::string> command;
                command.execute({"list1", "-1", "2"}, sequences);
            }, "Error: Invalid start or end index.\n");
            delete sequences["list1"];
        }
    }

    void testGetSequenceElementInvalidIndex() {
        TestRunner runner;

        // Тест для ListSequence
        {
            std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
            std::variant<int, double, std::string> list1Data[] = {"hello", "beautiful", "world"};
            sequences["list1"] = new MutableListSequence<std::variant<int, double, std::string>>(
                    list1Data, 3);

            runner.expectOutput("testGetSequenceElementInvalidIndex::Get list sequence element with invalid index", [&sequences]() {
                cli::GetSequenceElementCommand<int, double, std::string> command;
                command.execute({"list1", "10"}, sequences);
            }, "Error: Index out of range.\n");

            runner.expectOutput("testGetSequenceElementInvalidIndex::Get list sequence element with invalid index", [&sequences]() {
                cli::GetSequenceElementCommand<int, double, std::string> command;
                command.execute({"list1", "-1"}, sequences);
            }, "Error: Index out of range.\n");
            delete sequences["list1"];
        }
    }

    void testTryGetSequenceElementInvalidIndex() {
        TestRunner runner;

        // Тест для ListSequence
        {
            std::unordered_map<std::string, MutableSequence<std::variant<int, double, std::string>>*> sequences;
            std::variant<int, double, std::string> list1Data[] = {"hello", "beautiful", "world"};
            sequences["list1"] = new MutableListSequence<std::variant<int, double, std::string>>(
                    list1Data, 3);

            runner.expectOutput("testTryGetSequenceElementInvalidIndex::Try get list sequence element with invalid index", [&sequences]() {
                cli::TryGetSequenceElementCommand<int, double, std::string> command;
                command.execute({"list1", "10"}, sequences);
            }, "None\n");

            runner.expectOutput("testTryGetSequenceElementInvalidIndex::Try get list sequence element with invalid index", [&sequences]() {
                cli::TryGetSequenceElementCommand<int, double, std::string> command;
                command.execute({"list1", "-1"}, sequences);
            }, "None\n");

            delete sequences["list1"];
        }
    }
}