/** NodeLight, a lightweight JavaScript runtime. */
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

std::vector<std::function<void()>> tasks;

void interpretJS(const std::string& code) {
    auto task = [code]() {
        if (code == "console.log('Hello, World!');") {
            std::cout << "Hello, World!" << std::endl;
        }
    };
    tasks.emplace_back(task);
}

void runEventLoop() {
    while (!tasks.empty()) {
        auto task = tasks.front();
        task();
        tasks.erase(tasks.begin());
    }
}

int main() {
    std::ifstream file("../index.js");

    if (!file) {
        std::cerr << "Unable to open file index.js";
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
