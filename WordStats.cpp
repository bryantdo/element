#include "WordStats.h"

WordStats::WordStats() {
    m_sWord     = new std::string;
    m_iCount    = new int();
    m_dMean     = new double();
    m_dStddev   = new double();
    m_dProb     = new double();
    m_iHitSeqs  = new int();
}

WordStats::WordStats(std::string p_sWord, int p_iCount, double p_dMean, double p_dStddev, double p_dProb, int p_iHitSeqs) {
    m_sWord     = new std::string(p_sWord);
    m_iCount    = new int(p_iCount);
    m_dMean     = new double(p_dMean);
    m_dStddev   = new double(p_dStddev);
    m_dProb     = new double(p_dProb);
    m_iHitSeqs  = new int(p_iHitSeqs);
}

WordStats::~WordStats() {
    delete m_sWord;
    delete m_iCount;
    delete m_dMean;
    delete m_dStddev;
    delete m_dProb;
    delete m_iHitSeqs;
}
