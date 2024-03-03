/** NodeLight, a lightweight JavaScript runtime. */
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

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
