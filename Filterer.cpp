#include "Filterer.h"

Filterer::Filterer(Elementer* p_dElementer) {
    m_dUnfilteredResults                = p_dElementer->getWordStatsResults();
    m_dSortedUnfilteredStatsByLength    = new boost::unordered_map<unsigned int, std::vector<WordStatsObserved* >* >();
    this->initByWordSize();
}

Filterer::~Filterer() {
    delete m_dSortedUnfilteredStatsByLength;
}

bool Filterer::initByWordSize() {
    bool l_bSuccess = true;
    for(unsigned int i = 0; i < m_dUnfilteredResults->size(); i++) {
        unsigned int l_iWordLength = m_dUnfilteredResults->at(i)->getWord().size();
        if(m_dSortedUnfilteredStatsByLength->find(l_iWordLength) == m_dSortedUnfilteredStatsByLength->end()) {
            std::vector<WordStatsObserved* >* l_dTempVector = new std::vector<WordStatsObserved* >();
            l_dTempVector->push_back(m_dUnfilteredResults->at(i));
            m_dSortedUnfilteredStatsByLength->insert(std::make_pair(l_iWordLength, l_dTempVector));
        } else {
            m_dSortedUnfilteredStatsByLength->at(l_iWordLength)->push_back(m_dUnfilteredResults->at(i));
        }
    }    
    typedef boost::unordered_map<unsigned int, std::vector<WordStatsObserved* >* > map;
    BOOST_FOREACH(map::value_type i, *m_dSortedUnfilteredStatsByLength) {
        std::sort(i.second->begin(), i.second->end(), WordStatsObserved::sortByPValue);
    }
    return l_bSuccess;
}

std::vector<WordStatsObserved* > Filterer::getFilteredResults(
        FilterParameters::FilterType l_dFilterType, double p_dCuttoff) {
    std::vector<WordStatsObserved* > l_vFilteredResults;
    switch(l_dFilterType) {
        case FilterParameters::NONE:
            break;
        case FilterParameters::BENJAMINIHOCHBERG:
            l_vFilteredResults = this->getBenjaminiHochbergFiltered(p_dCuttoff);
            break;
        case FilterParameters::BONFERRONIHOLM:
            break;
        case FilterParameters::BONFERRONI:
            break;
        default:
            break;
    }
    return l_vFilteredResults;
}

std::vector<std::string> Filterer::getFilteredResultsAsStrings(
        FilterParameters::FilterType l_dFilterType, double p_dCuttoff) {
    std::vector<std::string> l_vFilteredResults;
    switch(l_dFilterType) {
        case FilterParameters::NONE:
            break;
        case FilterParameters::BENJAMINIHOCHBERG:
            l_vFilteredResults = this->getBenjaminiHochbergFilteredAsStrings(p_dCuttoff);
            break;
        case FilterParameters::BONFERRONIHOLM:
            break;
        case FilterParameters::BONFERRONI:
            break;
        default:
            break;
    }
    return l_vFilteredResults;
}

std::vector<WordStatsObserved* > Filterer::getBenjaminiHochbergFiltered(double p_dCuttoff) {
    std::vector<WordStatsObserved* > l_dBenHochFilteredResults;
    typedef boost::unordered_map<unsigned int, std::vector<WordStatsObserved* >* > map;
    BOOST_FOREACH(map::value_type i, *m_dSortedUnfilteredStatsByLength) {
        bool l_bDone = false;
        long unsigned int l_iDiff = 0, l_iIndex = 0, l_iNumElements = i.second->size();
        while(!l_bDone && l_iDiff < l_iNumElements) {
            l_iIndex = l_iNumElements - 1 - l_iDiff;
            double l_dCurrentPValue = i.second->at(l_iIndex)->getPValue();
            double l_iComparator = (double)(l_iNumElements - l_iDiff) * p_dCuttoff / l_iNumElements;
            if(l_dCurrentPValue < l_iComparator) { l_bDone = true; }
            l_iDiff++;
        }
        if(l_bDone) {
            std::vector<WordStatsObserved* >::iterator l_itPos = i.second->begin() + l_iIndex;
            l_dBenHochFilteredResults.insert(l_dBenHochFilteredResults.end(), i.second->begin(), l_itPos);
        }        
    }
    std::sort(l_dBenHochFilteredResults.begin(), l_dBenHochFilteredResults.end(), WordStatsObserved::sortByPValue);
    
    
    for(unsigned int i = 0; i < l_dBenHochFilteredResults.size(); i++) {
        for(unsigned int j = 0; j < l_dBenHochFilteredResults.size(); j++) {
            /*
            if(i != j) {
                std::string wordone = l_dBenHochFilteredResults.at(i)->getWord();
                std::string wordtwo = l_dBenHochFilteredResults.at(j)->getWord();
                std::cout << wordone << " " << wordtwo << " " << this->getLevenshtienDistance(wordone, wordtwo) << std::endl;
            }
            */
    		//if(i != j)
            this->getSmithWatermanDistance(l_dBenHochFilteredResults.at(i)->getWord(), l_dBenHochFilteredResults.at(j)->getWord());
        
        
        }
    }
    
    
    
    
    return l_dBenHochFilteredResults;
}

std::vector<std::string> Filterer::getBenjaminiHochbergFilteredAsStrings(double p_dCuttoff) {
    std::vector<std::string> l_dStringsResults;
    std::vector<WordStatsObserved* > l_dBenHochFilteredResults = this->getBenjaminiHochbergFiltered(p_dCuttoff);    
    for(unsigned int i = 0; i < l_dBenHochFilteredResults.size(); i++) {
        WordStatsObserved* l_dTempWordStatsObserved = l_dBenHochFilteredResults.at(i);
        std::stringstream l_dStringStream;
        l_dStringStream.precision(6);
        l_dStringStream << l_dTempWordStatsObserved->getWord() << "\t";
        l_dStringStream << l_dTempWordStatsObserved->getCount() << "\t";
        l_dStringStream << l_dTempWordStatsObserved->getMean() << "\t";
        l_dStringStream << l_dTempWordStatsObserved->getStddev() << "\t";
        l_dStringStream << l_dTempWordStatsObserved->getProb() << "\t";
        l_dStringStream << l_dTempWordStatsObserved->getHitSeqs() << "\t";
        l_dStringStream << l_dTempWordStatsObserved->getPValue();
        std::string l_sTempResult(l_dStringStream.str());
        l_dStringsResults.push_back(l_sTempResult);
    }    
    return l_dStringsResults;
}

int Filterer::getLevenshtienDistance(const std::string p_sSourceWord, const std::string p_sTargetWord) {
    unsigned int l_iLevenshtienDistance;
    const unsigned int l_iSourceLength = p_sSourceWord.length();
    const unsigned int l_iTargetLength = p_sTargetWord.length();    
    if(l_iSourceLength == 0) { l_iLevenshtienDistance = l_iTargetLength; } 
    else if(l_iTargetLength == 0) { l_iLevenshtienDistance = l_iSourceLength; }    
    else {
        typedef std::vector< std::vector<int> > Tmatrix;
        Tmatrix l_dMatrix(l_iSourceLength + 1);
        for(unsigned int i = 0; i <= l_iSourceLength; i++) { l_dMatrix[i].resize(l_iTargetLength + 1); }
        for(unsigned int i = 0; i <= l_iSourceLength; i++) { l_dMatrix[i][0] = i; }
        for(unsigned int j = 0; j <= l_iTargetLength; j++) { l_dMatrix[0][j] = j; }        
        for(unsigned int i = 1; i <= l_iSourceLength; i++) {
            const char s_i = p_sSourceWord[i-1];
            for(unsigned int j = 1; j <= l_iTargetLength; j++) {
                const char t_j = p_sTargetWord[j-1];
                int l_iCost;
                if(s_i == t_j) { l_iCost = 0; }
                else { l_iCost = 1; }
                const int l_iAbove      = l_dMatrix[i-1][  j];
                const int l_iLeft       = l_dMatrix[  i][j-1];
                const int l_iDiag       = l_dMatrix[i-1][j-1];
                int l_iCell             = std::min(l_iAbove+1, std::min(l_iLeft+1, l_iDiag+l_iCost));                
                if(i > 2 && j > 2) {
                    int l_iTrans = l_dMatrix[i-2][j-2]+1;
                    if(p_sSourceWord[i-2] != t_j) { l_iTrans++; }
                    if(s_i != p_sTargetWord[j-2]) { l_iTrans++; }
                    if(l_iCell < l_iTrans) { l_iCell = l_iTrans; }
                }                
                l_dMatrix[i][j] = l_iCell;
            }
        }
        l_iLevenshtienDistance = l_dMatrix[l_iSourceLength][l_iTargetLength];
    }
    return l_iLevenshtienDistance;
}

int Filterer::getSmithWatermanDistance(const std::string p_sSourceWord, const std::string p_sTargetWord) {
    using namespace seqan;
    typedef String<char>            TSequence;  // sequence type
    typedef Align<TSequence,ArrayGaps>  TAlign;     // align type

    
    TSequence seq1 = p_sSourceWord;
    TSequence seq2 = p_sTargetWord;
    TAlign align;
    resize(rows(align), 2);
    assignSource(row(align,0),seq1);
    assignSource(row(align,1),seq2);
    int score = globalAlignment(align,Score<int>(1,-1,-1,-1), Gotoh());
    ::std::cout << "Score = " << score << ::std::endl;
    ::std::cout << align;
    
    
}
