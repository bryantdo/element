#ifndef ELEMENTER_H
#define	ELEMENTER_H

#include <iostream>
#include <vector>
#include <sstream>
#include <boost/math/distributions/binomial.hpp>
#include "Words.h"
#include "WordStats.h"
#include "Sequences.h"
#include "Counter.h"
#include "Partitions.h"

class Elementer {
public:
    Elementer(Words*, std::vector<Sequences* >*, std::vector<Counter* >*, int*);
    virtual ~Elementer();
    WordStatsObserved* getWordStatResult(std::string);
    std::vector<WordStatsObserved* >* getWordStatsResults();
    std::vector<std::string>* getStringResults();
private:
    bool calculateProbabilities();
    bool*                                                       m_bVerbose;
    std::vector<std::string>*                                   m_vWords;
    boost::unordered_map<std::string, bool>*                    m_dWords;
    std::vector<Sequences* >*                                   m_vSequences;
    int*                                                        m_iNumThreads;
    Partitions*                                                 m_dPartitions;
    std::vector<Counter* >*                                     m_vSequencesCounters;
    Counter*                                                    m_dQueryCounter;
    std::vector<Counter* >*                                     m_vBackgroundCounters;
    int*                                                        m_iMixtureSize;
    std::vector<WordStatsObserved* >*                           m_vResults;
    boost::unordered_map<std::string, WordStatsObserved* >*     m_dResultsHash;
    std::vector<std::string>*                                   m_vResultsAsStrings;    
};

#endif	/* ELEMENTER_H */

