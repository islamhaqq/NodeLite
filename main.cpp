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
    const std::string jsCode = "console.log('Hello, World!');";

    interpretJS(jsCode);

    runEventLoop();

    return 0;
}
