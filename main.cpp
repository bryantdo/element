#include "main.h"

#include "Partitions.h"

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.2"
#endif
#ifndef CONTACT
#define CONTACT "Doug Bryant <dbryant@danforthcenter.org>"
#endif

int main(int argc, char** argv) {
    
    /*
    Partitions* l_dPartitions = new Partitions();
    std::vector<std::vector<int> >* l_dTemp = l_dPartitions->getPartitions(9,3);
    for(int i = 0; i < l_dTemp->size(); i++) {
        for(int j = 0; j < l_dTemp->at(i).size(); j++) {
            std::cout << l_dTemp->at(i).at(j) << " ";
        }
        std::cout << std::endl;
    }
    */
    
    
    if(argc < 2) { outputUsage(argv); }
    else {
        if(strcmp(argv[1], "bground") == 0) { Count* count = new Count(argc-1, argv+1); }
        else if(strcmp(argv[1], "count") == 0) { Element* element = new Element(argc-1, argv+1); }
        else { std::cout << "[main] unrecognized command " << argv[1] << std::endl; }
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
    return false;
}