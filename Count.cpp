#include "Count.h"

Count::Count(int argc, char** argv) {
    m_dCountParams = new CountParameters();
    if(argc < 3) { Count::outputUsage(); return; }    
    else {
        m_dWords            = NULL;
        m_dSequences        = NULL;
        char* l_cParameter;
        for(int i = 1; i < argc; i++) {
            l_cParameter = argv[i];
            if(l_cParameter[0] == '-') {
                switch(l_cParameter[1]) {
                    case 'w': m_dCountParams->setInputWordsFileName(argv[++i]); break;
                    case 's': m_dCountParams->setInputSequencesFileName(argv[++i]); break;
                    case 'o': m_dCountParams->setOutputFileName(argv[++i]); break;
                    case 't': m_dCountParams->setNumThreads(atoi(argv[++i])); break;
                    default: std::cout << "Error in " << argv[i] << " " << argv[i+1] << std::endl;
                }
            }
        } 
    }
    this->run();
}

Count::~Count() { 
    delete m_dWords;
    delete m_dSequences;
    delete m_dCountParams; 
}

bool Count::run() {
    clock_t l_dStartTime = clock();
    bool l_bSuccess = true;
    if(m_dCountParams->verifyParameters()) {        
        m_dWords                = new Words(m_dCountParams);
        m_dSequences            = new Sequences(m_dCountParams);
        Counter* l_dCounter     = new Counter(m_dWords, m_dSequences, m_dCountParams->getNumThreads());
        std::vector<std::string>* l_vResults = l_dCounter->getStringResults();
        if(l_vResults->size() > 0) {
            std::ofstream l_osOutputFile; l_osOutputFile.open(
                    m_dCountParams->getOutputFileName().c_str(), std::ios::app | std::ios::out);
            for(unsigned int i = 0; i < l_vResults->size(); i++) { l_osOutputFile << l_vResults->at(i) << std::endl; }
            l_osOutputFile.close();
        }
        delete l_dCounter;
    } else {
        Count::outputUsage();
        l_bSuccess = false;
    }
    double l_dTimeDiff = (std::clock() - l_dStartTime) / (double)CLOCKS_PER_SEC / double(*(m_dCountParams->getNumThreads()));
    std::cout << "DONE. Analysis took " << l_dTimeDiff << " total wall seconds." << std::endl;
    return l_bSuccess;
}

void Count::outputUsage() {
    std::cout << std::endl;
    std::cout << "Usage: element count [-t threads] [-w words-file] [-o output-file] [-s sequences-file]" << std::endl;
    std::cout << "Options/Arguments:" << std::endl;
    std::cout << "      -t <integer>   Number of threads to use. Optional, defaults to 1." << std::endl;
    std::cout << "      -w <file path> Input words file, one word per line. (REQD)" << std::endl;
    std::cout << "      -o <file path> Output file name. Optional, defaults to \"sequences_file.bground\"" << std::endl;
    std::cout << "      -s <file path> Input sequences file, FASTA format. (REQD)" << std::endl;
    std::cout << std::endl;
}
