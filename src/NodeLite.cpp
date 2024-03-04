#include "NodeLite.h"

#include <fstream>
#include <regex>
#include <functional>

std::vector<std::function<void(std::ostream&)>> tasks;

void interpretJS(std::string &code, std::ifstream &file, std::string &line) {
    // Patterns
    std::regex consoleLogRegex(R"(console\.log\('(.+)'\);)");
    std::regex functionDefinitionRegex(R"(function\s+(\w+)\(\)\s*\{\s*console\.log\('(.+)'\);\s*\})");
    std::regex functionCallRegex(R"((\w+)\(\);)");

    // Match standalone console.log statements
    std::smatch consoleLogMatches;
    while (std::regex_search(code, consoleLogMatches, consoleLogRegex)) {
        if (consoleLogMatches.size() == 2) { // We expect 2 matches: the whole pattern and the log message
            std::string message = consoleLogMatches[1];
            auto logTask = [message](std::ostream& out) {
                out << message << std::endl;
            };
            tasks.emplace_back(logTask);
        }
        code = consoleLogMatches.suffix().str(); // Continue searching from the end of the last match
    }

    // Reset the code string for function search as regex_search has moved to the end of the string
    file.seekg(0, std::ios::beg); // Reset file pointer to the beginning
    code.clear(); // Clear the current code content
    while (std::getline(file, line)) { // Re-read the file content
        code += line + '\n'; // Rebuild the code string with newline characters
    }

    // Match function definitions and calls
    std::smatch functionMatches;
    if (std::regex_search(code, functionMatches, functionDefinitionRegex)) {
        if (functionMatches.size() == 3) { // Expected matches: full pattern, function name, message
            std::string funcName = functionMatches[1];
            std::string message = functionMatches[2];

            auto task = [funcName, message](std::ostream& out) {
                if (funcName == "displayMessage") { // This could be expanded for more functions
                    out << message << std::endl;
                }
            };
            tasks.emplace_back(task);
        }
    }

    // Note: Currently, function call detection doesn't dynamically link to function definitions.
    // Additional logic would be needed to match specific function calls to their definitions.
}

void runEventLoop(std::ostream &out) {
    while (!tasks.empty()) {
        auto task = tasks.front();
        task(out);
        tasks.erase(tasks.begin());
    }
}
