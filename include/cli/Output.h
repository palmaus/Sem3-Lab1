#pragma once

#include <iostream>
#include <string>

namespace cli {
    namespace output {

        // Выводит сообщение об успешном выполнении
        void success(const std::string& message);

        // Выводит сообщение об ошибке
        void error(const std::string& message);

        // Выводит сообщение об ошибке с указанием причины
        void error(const std::string& message, const std::string& reason);

    } // namespace output
} // namespace cli