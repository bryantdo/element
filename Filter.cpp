#include "Filter.h"

Filter::Filter(int argc, char** argv) {
    m_dFilterParameters = new FilterParameters();
    if(argc < 2) { Filter::outputUsage(); return; }
    else {
        char* l_cParameter;
        for(int i = 1; i < argc; i++) {
            l_cParameter = argv[i];
            if(l_cParameter[0] == '-') {
                switch(l_cParameter[1]) {
                    case 'f': m_dFilterParameters->setFilterType((FilterParameters::FilterType)(atoi(argv[++i]))); break;
                    case 'a': m_dFilterParameters->setAbsoluteCuttoff(atof(argv[++i])); break;
                    case 'o': m_dFilterParameters->setOutputFileName(argv[++i]); break;
                    case 'c': m_dFilterParameters->setInputCountFileName(argv[++i]); break;
                    default: std::cout << "Error in " << argv[i] << " " << argv[++i] << std::endl;
                }
            }
        }
    }    
    this->run();
}

Filter::~Filter() {
    delete m_dFilterParameters;
}

bool Filter::run() {
    bool l_bSuccess = true;
    if(m_dFilterParameters->verifyParameters()) {
        Elementer* l_dElementer = new Elementer(m_dFilterParameters->getInputCountFileName());
        Filterer* l_dFilterer = new Filterer(l_dElementer);
        std::vector<std::string> l_vResults = l_dFilterer->getFilteredResultsAsStrings(
                *(m_dFilterParameters->getFilterType()), *(m_dFilterParameters->getSignificanceCuttoff()));
        if(l_vResults.size() != 0) {
            std::ofstream l_osOutputFile;
            l_osOutputFile.open(m_dFilterParameters->getOutputFileName().c_str(), std::ios::app | std::ios::out);
            for(unsigned int i = 0; i < l_vResults.size(); i++) { l_osOutputFile << l_vResults.at(i) << std::endl; }
            l_osOutputFile.close();
        }        
        delete l_dElementer;
        delete l_dFilterer;
    } else {
        this->outputUsage();
        l_bSuccess = false;   
    }
    return l_bSuccess;
}


void Filter::outputUsage() {
    std::cout << std::endl;
    std::cout << "Usage: element filter [-f 0,1,2,3] [-a double] [-o output-file] [-c count-file]" << std::endl;
    std::cout << "Options/Arguments:" << std::endl;
    std::cout << "      -f <0,1,2,3>    Specify which filtering method to apply. Optional, defaults to 1:" << std::endl;
    std::cout << "                        0: No filtering, use absolute cutoff" << std::endl;
    std::cout << "                        1: Benjamini-Hochberg" << std::endl;
    std::cout << "                        2: Bonferroni-Holm" << std::endl;
    std::cout << "                        3: Bonferroni" << std::endl;
    std::cout << "      -a <double>     Specify significance level. Optional, defaults to 0.05." << std::endl;
    std::cout << "      -c <file path>  File path to count file (REQD)." << std::endl;
    std::cout << "      -o <file path>  Output file name. Optional, defaults to \"count-file.filtered\"." << std::endl;
    std::cout << std::endl;
}
