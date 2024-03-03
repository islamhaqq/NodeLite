#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <filesystem>

struct TestCase {
    std::string jsFile;
    std::string expectedOutputFile;
};

std::string readFileContents(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

bool runTest(const TestCase& testCase) {
    std::filesystem::path exePath = std::filesystem::current_path() / "../bin/nodelite.exe";
    std::filesystem::path jsFilePath = std::filesystem::absolute(testCase.jsFile);
    std::string command = exePath.string() + " " + jsFilePath.string() + " output.txt";
    std::system(command.c_str());

    std::string expectedOutput = readFileContents(testCase.expectedOutputFile);
    std::string actualOutput = readFileContents("output.txt");

    return expectedOutput == actualOutput;
}

int main() {
    // @TODO: Generate test cases automatically from a directory of test files
    std::vector<TestCase> testCases = {
        {"../tests/displayMessage.js", "../tests/displayMessage_expected.txt"}
    };

    int passedTests = 0;
    for (const auto& testCase : testCases) {
        if (runTest(testCase)) {
            std::cout << testCase.jsFile << " PASSED" << std::endl;
            ++passedTests;
        } else {
            std::cout << testCase.jsFile << " FAILED" << std::endl;
        }
    }

    std::cout << "Summary: " << passedTests << "/" << testCases.size() << " tests passed." << std::endl;

    return 0;
}
