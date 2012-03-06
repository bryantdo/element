#include "Words.h"

Words::Words(Parameters* p_dParameters) {
    m_dWords = NULL;
    m_vWords = NULL;
    Words::loadWords(p_dParameters->getInputWordsFileName());
}

Words::Words(std::vector<std::string>* p_vWords) {
    m_dWords = NULL;
    m_vWords = NULL;
    m_vWords = new std::vector<std::string>(*p_vWords);
}

Words::~Words() {
    delete m_dWords;
    delete m_vWords;
}

bool Words::loadWords(std::string* p_sInputWordsFileName) {    
    m_dWords = new boost::unordered_map<std::string, bool>();
    m_vWords = new std::vector<std::string>();
    std::ifstream l_dWordsFileStream; l_dWordsFileStream.open((*p_sInputWordsFileName).c_str());
    std::string l_sLine;
    while(getline(l_dWordsFileStream, l_sLine)) {
        boost::trim(l_sLine);
        std::transform(l_sLine.begin(), l_sLine.end(), l_sLine.begin(), ::toupper);
        m_dWords->insert(std::make_pair(l_sLine, true));
        m_vWords->push_back(l_sLine);        
    }
    l_dWordsFileStream.close();
    return true;
}
