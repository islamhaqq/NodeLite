/** NodeLight, a lightweight JavaScript runtime. */
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <regex>

std::vector<std::function<void()>> tasks;

void interpretJS(std::string& code) {
    // Regex to find function calls and definitions
    std::regex funcDefRegex(R"(function\s+(\w+)\(\)\s*\{\s*console\.log\('(.+)'\);\s*\})");
    std::regex funcCallRegex(R"((\w+)\(\);)");

    std::smatch matches;
    // Check for function definition
    if (std::regex_search(code, matches, funcDefRegex)) {
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
    while (std::regex_search(code, matches, funcCallRegex)) {
        if (matches.size() > 1) {
            std::string funcName = matches[1];

            // If a function is called, add its task to the queue (if defined)
            for (auto& task : tasks) {
                // We need a way to identify tasks; for simplicity, we assume they're unique
                // This is a limitation: in real scenarios, we'd need a better management of functions and scopes
                task(); // Execute the task associated with the function call
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
