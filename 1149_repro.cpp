// Reproduce issue #1149:
#include "CLI/CLI.hpp"
#include <string>

int main(int argc, char **argv) {
    CLI::App app("Issue 1149 reproduction");

    std::string package;

    auto *install = app.add_subcommand("install");
    install->add_option("package", package);

    app.add_subcommand("remove");
    app.add_subcommand("upgrade");

    // Enable your new fix inside CLI11.
    app.failure_message(CLI::FailureMessage::suggest);

    CLI11_PARSE(app, argc, argv);

    return 0;
}
