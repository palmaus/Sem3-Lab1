#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "Menu.h"
#include "MutableSequence.h"

namespace cli {

    template <typename... Ts>
    class Session {
    public:
        Session(Menu<Ts...>& menu);

        ~Session();

        void run();

    private:
        void processCommand(const std::string& commandLine);
        void showHelp(const std::string& commandName) const;
        void showHelp() const;

        Menu<Ts...>& menu_;
        std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*> sequences_;
    };

} // namespace cli