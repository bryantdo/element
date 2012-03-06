#ifndef COUNTER_H
#define	COUNTER_H

#include <vector>
#include <omp.h>
#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <seqan/find.h>
#include "Words.h"
#include "WordStats.h"
#include "Sequences.h"

class Counter {
public:
    Counter();
    Counter(std::string*);
    Counter(Words*, Sequences*, int*);
    virtual ~Counter();
    bool addWordStats(std::string, WordStats*);
    bool addWordStats(std::string, int, double, double, double, int);
    WordStats* getWordStatResult(std::string);
    std::vector<WordStats* >* getWordStatsResults();
    std::vector<std::string>* getStringResults();
private:
    bool loadCountsFromFile(std::string*);
    bool countWordsHorspool();
    bool compileWordCountResults(std::vector<boost::unordered_map<std::string, int> >);
    void outputCompletionStatus(std::string, double, bool);
    bool*                                               m_bVerbose;
    std::vector<std::string>*                           m_vWords;
    boost::unordered_map<std::string, bool>*            m_dWords;
    std::vector<std::string>*                           m_vSequences;
    int*                                                m_iNumThreads;
    std::vector<WordStats* >*                           m_vResults;
    boost::unordered_map<std::string, WordStats* >*     m_dResultsHash;
    std::vector<std::string>*                           m_vResultsAsStrings;
};

#endif	/* COUNTER_H */
