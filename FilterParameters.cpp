#include "Parameters.h"

FilterParameters::FilterParameters() {
    m_sInputWordsFileName       = new std::string("");
    m_sInputSequencesFileName   = new std::string("");
    m_iNumThreads               = new int(1);
    m_sInputCountFileName       = new std::string("");
    m_iFilterType               = new FilterType(BENJAMINIHOCHBERG);
    m_dSignificanceCuttoff      = new double(-1.);
    m_sOutputFileName           = new std::string("");
}

FilterParameters::~FilterParameters() {
    delete m_sInputWordsFileName;
    delete m_sInputSequencesFileName;
    delete m_sOutputFileName;
    delete m_iNumThreads;
    delete m_sInputCountFileName;
    delete m_iFilterType;
    delete m_dSignificanceCuttoff;
}

bool FilterParameters::verifyParameters() {
    bool l_bParametersGood = true;
    if(*m_sInputCountFileName == "") { l_bParametersGood = false; }
    else {
        if(*m_dSignificanceCuttoff == -1) { *m_dSignificanceCuttoff = 0.05; }
        if(*m_sOutputFileName == "") { *m_sOutputFileName = *m_sInputCountFileName + ".filtered"; }
    }
    return l_bParametersGood;
}