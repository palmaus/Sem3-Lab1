#pragma once

#include "Command.h"

namespace cli {

    template <typename... Ts>
    class GetSequenceElementCommand : public Command<Ts...> {
    public:
        GetSequenceElementCommand();

        void execute(const std::vector<std::string>& args,
                     std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) override;

        std::string getDescription() const override;

    private:
        void getByIndex(const std::string& seqName, int index,
                        const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const;
        void getFirst(const std::string& seqName,
                        const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const;
        void getLast(const std::string& seqName,
                       const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const;
    };

} // namespace cli