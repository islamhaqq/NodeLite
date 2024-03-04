/** NodeLite, a lightweight JavaScript runtime. */

#include <iostream>
#include <fstream>

#include "NodeLite.h"


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
        }
        outStream = &outFile;
    }

    interpretJS(jsCode, file, line);

    runEventLoop(*outStream);

    return 0;
}
