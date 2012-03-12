#include "WordStats.h"

WordStatsObserved::WordStatsObserved() {
    m_sWord     = new std::string;
    m_iCount    = new int();
    m_dMean     = new double();
    m_dStddev   = new double();
    m_dProb     = new double();
    m_iHitSeqs  = new int();
    m_dPValue   = new double();
}

WordStatsObserved::WordStatsObserved(std::string p_sWord, int p_iCount, double p_dMean, double p_dStddev, 
        double p_dProb, int p_iHitSeqs, double p_dPValue) {
    m_sWord     = new std::string(p_sWord);
    m_iCount    = new int(p_iCount);
    m_dMean     = new double(p_dMean);
    m_dStddev   = new double(p_dStddev);
    m_dProb     = new double(p_dProb);
    m_iHitSeqs  = new int(p_iHitSeqs);
    m_dPValue   = new double(p_dPValue);
}

WordStatsObserved::WordStatsObserved(WordStats* p_dWordStats, double p_dPValue) {
    m_sWord     = new std::string(p_dWordStats->getWord());
    m_iCount    = new int(p_dWordStats->getCount());
    m_dMean     = new double(p_dWordStats->getMean());
    m_dStddev   = new double(p_dWordStats->getStddev());
    m_dProb     = new double(p_dWordStats->getProb());
    m_iHitSeqs  = new int(p_dWordStats->getHitSeqs());
    m_dPValue  = new double(p_dPValue);
}

WordStatsObserved::~WordStatsObserved() {
    delete m_sWord;
    delete m_iCount;
    delete m_dMean;
    delete m_dStddev;
    delete m_dProb;
    delete m_iHitSeqs;
    delete m_dPValue;
}

bool WordStatsObserved::sortByPValue(WordStatsObserved* i, WordStatsObserved* j) { 
    return (i->getPValue() < j->getPValue());
}

void WordStatsObserved::setPValue(double p_dPValue) {
    m_dPValue = new double(p_dPValue);
}
