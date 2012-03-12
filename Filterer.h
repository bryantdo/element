#ifndef FILTERER_H
#define	FILTERER_H

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <seqan/align.h>
#include "Parameters.h"
#include "Elementer.h"

class Filterer {
public:
    Filterer(Elementer*);
    virtual ~Filterer();
    std::vector<WordStatsObserved* > getFilteredResults(FilterParameters::FilterType, double);
    std::vector<std::string> getFilteredResultsAsStrings(FilterParameters::FilterType, double);
private:
    bool initByWordSize();
    std::vector<WordStatsObserved* > getBenjaminiHochbergFiltered(double);
    std::vector<std::string> getBenjaminiHochbergFilteredAsStrings(double);
    int getLevenshtienDistance(const std::string, const std::string);
    int getSmithWatermanDistance(const std::string, const std::string);
    std::vector<WordStatsObserved* >*                                           m_dUnfilteredResults;
    boost::unordered_map<unsigned int, std::vector<WordStatsObserved* >* >*     m_dSortedUnfilteredStatsByLength;    
};

#endif	/* FILTERER_H */
