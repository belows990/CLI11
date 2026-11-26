// CLI11 Issue #984: demonstrate Solution 1 with app.failure_message().
// c++ -std=c++11 -Iinclude issue984-demo.cpp -o issue984-demo
#include "CLI/CLI.hpp"

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
    CLI::App app{"Issue 984 structured-error demonstration"};
    std::vector<int> numbers;
    app.add_option("--numbers", numbers, "Three numbers")->expected(3);

    app.failure_message([](const CLI::App *current_app, const CLI::Error &error) {
        // The callback receives Error&, so check whether this is an ArgumentMismatch.
        const auto *mismatch = dynamic_cast<const CLI::ArgumentMismatch *>(&error);
        if(mismatch != nullptr) {
            const auto &details = mismatch->get_details();
            if(details.reason == CLI::ArgumentMismatch::Reason::AtLeast &&
               details.has_count && details.has_received) {
                return std::string("Please provide at least ") + std::to_string(details.count) +
                       " numbers for " + details.option_name + ". You provided only " +
                       std::to_string(details.received) + ".\n";
            }
        }
        // Preserve normal behavior for unrelated errors.
        return CLI::FailureMessage::simple(current_app, error);
    });

    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError &error) {
        // This invokes the registered failure_message() callback and prints to stderr.
        return app.exit(error);
    }

    std::cout << "Successfully parsed " << numbers.size() << " numbers.\n";
    return 0;
}

