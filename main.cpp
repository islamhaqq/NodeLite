/** NodeLight, a lightweight JavaScript runtime. */
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <regex>

std::vector<std::function<void()>> tasks;

void interpretJS(std::string& code) {
    std::regex consoleLogRegex(R"(console\.log\('(.+)'\);)");
    std::regex functionDefinitionRegex(R"(function\s+(\w+)\(\)\s*\{\s*console\.log\('(.+)'\);\s*\})");
    std::regex functionCallRegex(R"((\w+)\(\);)");

    std::smatch matches;
    // Check for function definition
    if (std::regex_search(code, matches, functionDefinitionRegex)) {
        if (matches.size() == 3) { // We expect 3 matches: the whole pattern, the function name, and the log message
            std::string funcName = matches[1];
            std::string message = matches[2];

            // Create a task for the function definition
            auto task = [funcName, message]() {
                if (funcName == "displayMessage") { // We only support displayMessage for now
                    std::cout << message << std::endl;
                }
            };
            tasks.emplace_back(task);
        }
    }

    // Check for function calls
    while (std::regex_search(code, matches, functionCallRegex)) {
        if (matches.size() > 1) {
            std::string funcName = matches[1];

            for (auto& task : tasks) {
                task();
            }
        }
        code = matches.suffix().str(); // Continue searching from the end of the last match
    }
}

void runEventLoop() {
    while (!tasks.empty()) {
        auto task = tasks.front();
        task();
        tasks.erase(tasks.begin());
    }
}

int main(const int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path-to-js-file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    std::string jsCode;
    std::string line;
    while (getline(file, line)) {
        jsCode += line;
    }
    file.close();

    interpretJS(jsCode);

    runEventLoop();

    return 0;
}
