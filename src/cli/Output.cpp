#include "Output.h"

namespace cli {
    namespace output {

        void success(const std::string& message) {
            std::cout << message << std::endl;
        }

        void error(const std::string& message) {
            std::cerr << message << std::endl;
        }

        void error(const std::string& message, const std::string& reason) {
            std::cerr << message << reason << std::endl;
        }

    } // namespace output
} // namespace cli