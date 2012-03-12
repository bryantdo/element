#include "main.h"

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.2"
#endif
#ifndef CONTACT
#define CONTACT "Doug Bryant <dbryant@danforthcenter.org>"
#endif

int main(int argc, char** argv) {    
    if(argc < 2) { outputUsage(argv); }
    else {
        if(strcmp(argv[1], "bground") == 0) { Count* count = new Count(argc-1, argv+1); }
        else if(strcmp(argv[1], "count") == 0) { Element* element = new Element(argc-1, argv+1); }
        else if(strcmp(argv[1], "filter") == 0) { Filter* filter = new Filter(argc-1, argv+1); }
        else { std::cout << "[main] unrecognized command: " << argv[1] << std::endl; }
    }
    return 0;
    
}

static bool outputUsage(char** argv) {
    std::cout << "Program: element" << std::endl;
    std::cout << "Version: " << PACKAGE_VERSION << std::endl;
    std::cout << "Contact: " << CONTACT << std::endl;
    std::cout << std::endl;
    std::cout << "Usage: element [command] <options>" << std::endl;
    std::cout << "Command: bground" << std::endl;
    std::cout << "         count" << std::endl;
    std::cout << "         filter" << std::endl;
    return false;
}