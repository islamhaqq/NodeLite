#ifndef NODELITE_H
#define NODELITE_H

#include <string>


void interpretJS(std::string& code, std::ifstream &file, std::string &line);
void runEventLoop(std::ostream& out);


#endif //NODELITE_H
