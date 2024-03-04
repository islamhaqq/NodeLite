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

std::pair<bool, std::string> runTest(const TestCase &testCase) {
    std::filesystem::path exePath = std::filesystem::current_path() / "../bin/nodelite.exe";
    std::filesystem::path jsFilePath = std::filesystem::absolute(testCase.jsFile);
    std::string command = exePath.string() + " " + jsFilePath.string() + " output.txt";
    std::system(command.c_str());

    std::string expectedOutput = readFileContents(testCase.expectedOutputFile);
    std::string actualOutput = readFileContents("output.txt");

    if (expectedOutput != actualOutput) {
        std::string errorMsg = "Expected:\n" + expectedOutput + "\nActual:\n" + actualOutput + "\n";
        return {false, errorMsg};
    }

    return {true, ""};
}

int main() {
    std::vector<TestCase> testCases;
    std::filesystem::path testDir = "../tests";

    for (const auto& entry : std::filesystem::directory_iterator(testDir)) {
        if (entry.path().extension() == ".js") {
            std::filesystem::path expectedOutputFile = entry.path();
            expectedOutputFile.replace_extension(".txt");

            if (std::filesystem::exists(expectedOutputFile)) {
                testCases.push_back(TestCase{entry.path().string(), expectedOutputFile.string()});
            }
        }
    }

    int passedTests = 0;
    for (const auto& testCase : testCases) {
        auto [testPassed, errorMessage] = runTest(testCase);
        if (testPassed) {
            std::cout << testCase.jsFile << " PASSED" << std::endl;
            ++passedTests;
        } else {
            std::cout << testCase.jsFile << " FAILED\n" << errorMessage << std::endl;
        }
    }

    std::cout << "Summary: " << passedTests << "/" << testCases.size() << " tests passed." << std::endl;

    return 0;
}
