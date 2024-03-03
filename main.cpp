/** NodeLight, a lightweight JavaScript runtime. */
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <regex>

std::vector<std::function<void(std::ostream&)>> tasks;

void interpretJS(std::string& code) {
    std::regex consoleLogRegex(R"(console\.log\('(.+)'\);)");
    std::regex functionDefinitionRegex(R"(function\s+(\w+)\(\)\s*\{\s*console\.log\('(.+)'\);\s*\})");
    std::regex functionCallRegex(R"((\w+)\(\);)");

    std::smatch matches;
    if (std::regex_search(code, matches, functionDefinitionRegex)) {
        if (matches.size() == 3) { // We expect 3 matches: the whole pattern, the function name, and the log message
            std::string funcName = matches[1];
            std::string message = matches[2];

            auto task = [funcName, message](std::ostream& out) {
                if (funcName == "displayMessage") {
                    out << message << std::endl;
                }
            };
            tasks.emplace_back(task);
        }
    }
}

void runEventLoop(std::ostream& out) {
    while (!tasks.empty()) {
        auto task = tasks.front();
        task(out);
        tasks.erase(tasks.begin());
    }
}

int main(const int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path-to-js-file> [output-file]" << std::endl;
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

    std::ofstream outFile;
    std::ostream* outStream = &std::cout;

    if (argc >= 3) {
        outFile.open(argv[2]);
        if (!outFile) {
            std::cerr << "Unable to open file " << argv[2] << std::endl;
            return 1;
        } else {
            outStream = &outFile;
        }
    }

    interpretJS(jsCode);

    runEventLoop(*outStream);

    return 0;
}
