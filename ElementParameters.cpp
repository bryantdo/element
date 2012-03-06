#include "Parameters.h"

ElementParameters::ElementParameters() {
    m_sInputWordsFileName       = new std::string("");
    m_sInputSequencesFileName   = new std::string("");
    m_sOutputFileName           = new std::string("");
    m_iNumThreads               = new int(1);
    m_vSequencesFileNames       = new std::vector<std::string>();
    m_vBackgroundStatsFileNames = new std::vector<std::string>();
};

ElementParameters::~ElementParameters() {
    delete m_sInputWordsFileName;
    delete m_sInputSequencesFileName;
    delete m_sOutputFileName;
    delete m_iNumThreads;
    delete m_vSequencesFileNames;
    delete m_vBackgroundStatsFileNames;
}

bool ElementParameters::verifyParameters() {
    bool l_bParametersGood = true;
    if(*m_sInputWordsFileName == "" || m_vSequencesFileNames->size() < 1 || m_vBackgroundStatsFileNames->size() < 1) { 
        l_bParametersGood = false; 
    } else if(*m_sOutputFileName == "") { *m_sOutputFileName = *m_sInputSequencesFileName + ".out"; }
    return l_bParametersGood;
}

void ElementParameters::setBackgroundStatsFileNames(std::vector<std::string> p_vBGroundStatsFileNames) { 
        m_vBackgroundStatsFileNames = new std::vector<std::string>(p_vBGroundStatsFileNames);
}

std::vector<std::string>* ElementParameters::getBackgroundStatsFileNames() { return m_vBackgroundStatsFileNames; }

std::vector<std::string>* ElementParameters::getSequencesFileNames() { return m_vSequencesFileNames; }

void ElementParameters::setInputFileNames(std::vector<std::string> p_vInputFileNames) {
    if(p_vInputFileNames.size() %2 == 0) {
        for(int i = 0; i < p_vInputFileNames.size(); i++) {
            m_vSequencesFileNames->push_back(p_vInputFileNames.at(i));
            m_vBackgroundStatsFileNames->push_back(p_vInputFileNames.at(++i));
        }
    }
}

