#include "Parameters.h"

Parameters::Parameters() {
    m_sInputWordsFileName       = new std::string("");
    m_sInputSequencesFileName   = new std::string("");
    m_sOutputFileName           = new std::string("");
    m_iNumThreads               = new int(1);
}

Parameters::~Parameters() {
    delete m_sInputWordsFileName;
    delete m_sInputSequencesFileName;
    delete m_sOutputFileName;
    delete m_iNumThreads;    
}

bool Parameters::verifyParameters() {
    bool l_bParametersGood = true;
    if(*m_sInputWordsFileName == "" || *m_sInputSequencesFileName == "") { l_bParametersGood = false; }
    else if(*m_sOutputFileName == "") { *m_sOutputFileName = *m_sInputSequencesFileName + ".bground"; }
    return l_bParametersGood;    
}