#include "Element.h"

Element::Element(int argc, char** argv) {
    if(argc < 4) { Element::outputUsage(); return; }
    else {
        m_dElementParameters        = new ElementParameters();
        m_dWords                    = NULL;
        m_dSequences                = NULL;
        m_vSequences                = new std::vector<Sequences* >();
        m_vBGroundCounters          = new std::vector<Counter* >();
        std::vector<std::string> l_vInputFiles;
        char* l_cParameter;
        for(int i = 1; i < argc; i++) {
            l_cParameter = argv[i];
            if(l_cParameter[0] == '-') {
                switch(l_cParameter[1]) {
                    case 't': m_dElementParameters->setNumThreads(atoi(argv[++i])); break;
                    case 's': m_dElementParameters->setInputSequencesFileName(argv[++i]); break;
                    case 'w': m_dElementParameters->setInputWordsFileName(argv[++i]); break;
                    default: std::cout << "Error in " << argv[i] << " " << argv[++i] << std::endl;
                }
            } else {
                std::string l_sTempString(argv[i]);
                l_vInputFiles.push_back(l_sTempString);
            }
        }        
        m_dElementParameters->setInputFileNames(l_vInputFiles);
    }
    this->run();
}

Element::~Element() {
    delete m_dElementParameters;
    delete m_dWords;
    delete m_vSequences;
    delete m_vBGroundCounters;
}

bool Element::run() {
    bool l_bSuccess = true;
    if(m_dElementParameters->verifyParameters()) {
        m_dWords = new Words(m_dElementParameters);
        std::vector<std::string>* l_dSequencesFileNames = m_dElementParameters->getSequencesFileNames();
        for(int i = 0; i < l_dSequencesFileNames->size(); i++) {
            Sequences* l_dTempSequences = new Sequences(&(l_dSequencesFileNames->at(i)));
            m_vSequences->push_back(l_dTempSequences);
        }        
        std::vector<std::string>* l_dBackgroundStatsFileNames = m_dElementParameters->getBackgroundStatsFileNames();
        for(int i = 0; i < l_dBackgroundStatsFileNames->size(); i++) {
            Counter* l_dTempCounter = new Counter(&(l_dBackgroundStatsFileNames->at(i)));            
            m_vBGroundCounters->push_back(l_dTempCounter);
        }
        
        Elementer* l_dElementer = 
                new Elementer(m_dWords, m_vSequences, m_vBGroundCounters, m_dElementParameters->getNumThreads());
        
        std::vector<std::string>* l_vResults = l_dElementer->getStringResults();
        if(l_vResults->size() > 0) {
            std::ofstream l_osOutputFile; l_osOutputFile.open(
                    m_dElementParameters->getOutputFileName().c_str(), std::ios::app | std::ios::out);
            for(int i = 0; i < l_vResults->size(); i++) { l_osOutputFile << l_vResults->at(i) << std::endl; }
            l_osOutputFile.close();
        }
        delete l_dElementer;
    } else {
        this->outputUsage();
        l_bSuccess = false;
    }
    return l_bSuccess;
}

void Element::outputUsage() {
    std::cout << std::endl;
    std::cout << "Usage: ewf element [-t integer] [-w words-file] [sequences-file bground-file ...]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options/Arguments: " << std::endl;
    std::cout << "   -t <integer>        Number of threads to use. Optional, defaults to 1." << std::endl;
    std::cout << "   -w <file path>      Input words file, one word per line. (REQD)" << std::endl;
    std::cout << "   sequences-file      Input promoter sequences file, FASTA format." << std::endl;
    std::cout << "     bground-file ...  Background statistics files as created by count." << std::endl;
    std::cout << "                       At least one pair of sequence-file/bground-file REQD." << std::endl;
    std::cout << std::endl;
}
