#include "Counter.h"

Counter::Counter() {
    m_bVerbose          = new bool(true);
    m_vWords            = new std::vector<std::string>();
    m_vResults          = new std::vector<WordStats* >();
    m_vResultsAsStrings = new std::vector<std::string>();
    m_dResultsHash      = new boost::unordered_map<std::string, WordStats* >();    
}

Counter::Counter(std::string* p_sInputFileName) {
    m_bVerbose          = new bool(true);
    m_vWords            = new std::vector<std::string>();
    m_vResults          = new std::vector<WordStats* >();
    m_vResultsAsStrings = new std::vector<std::string>();
    m_dResultsHash      = new boost::unordered_map<std::string, WordStats* >();
    this->loadCountsFromFile(p_sInputFileName);
}

Counter::Counter(Words* p_dWords, Sequences* p_dSequences, int* p_iNumThreads) {
    m_bVerbose          = new bool(true);
    m_dWords            = p_dWords->getWordsHash();
    m_vWords            = p_dWords->getWordsVector();
    m_vSequences        = p_dSequences->getSequencesVector();
    m_iNumThreads       = p_iNumThreads;
    m_vResults          = new std::vector<WordStats* >();
    m_vResultsAsStrings = new std::vector<std::string>();
    m_dResultsHash      = new boost::unordered_map<std::string, WordStats* >();
    this->countWordsHorspool();
}

Counter::~Counter() {
    delete m_bVerbose;    
    delete m_vResults;
    delete m_vResultsAsStrings;
    delete m_dResultsHash;    
}

bool Counter::loadCountsFromFile(std::string* p_sInputFileName) {
    bool l_bReturnStatus = true;
    std::ifstream l_dInputFileStream; l_dInputFileStream.open((*p_sInputFileName).c_str());
    std::string l_sLine;
    while(getline(l_dInputFileStream, l_sLine)) {
        boost::trim(l_sLine);
        std::vector<std::string> l_vTokens;
        boost::split(l_vTokens, l_sLine, boost::is_any_of("\t"), boost::token_compress_off);
        
        std::string     l_sWord(l_vTokens.at(0));
        int             l_iCount;
        double          l_dMean;
        double          l_dStddev;
        double          l_dProb;
        int             l_iHitSeqs;
        
        try { l_iCount = boost::lexical_cast<int>(l_vTokens.at(1)); }
        catch(const std::exception&) { l_iCount = -1; }
        try { l_dMean = boost::lexical_cast<double>(l_vTokens.at(2)); }
        catch(const std::exception&) { l_dMean = -1; }
        try { l_dStddev = boost::lexical_cast<double>(l_vTokens.at(3)); }
        catch(const std::exception&) { l_dStddev = -1; }
        try { l_dProb = boost::lexical_cast<double>(l_vTokens.at(4)); }
        catch(const std::exception&) { l_dProb = -1; }
        try { l_iHitSeqs = boost::lexical_cast<int>(l_vTokens.at(5)); }
        catch(const std::exception&) { l_iHitSeqs = -1; }
        
        if(l_sWord.size() > 0 && l_iCount != -1 && l_dMean != -1 && l_dStddev != -1 && l_dProb != -1 
                && l_iHitSeqs != -1) {
            this->addWordStats(l_sWord, l_iCount, l_dMean, l_dStddev, l_dProb, l_iHitSeqs);
        } else { l_bReturnStatus = false; }
    }
    l_dInputFileStream.close();
    return l_bReturnStatus;
}

bool Counter::countWordsHorspool() {
    boost::unordered_map<std::string, std::vector<int> > l_dCompiledResults;
    for(int i = 0; i < m_vWords->size(); i++) {
        std::vector<int> l_dTempVector;
        l_dCompiledResults.insert(std::make_pair(m_vWords->at(i), l_dTempVector));
    }    
    
    omp_set_num_threads(*m_iNumThreads);
    #define CHUNKSIZE 1
    int th_id;
    #pragma omp parallel private(th_id)
    {
    #pragma omp for schedule(dynamic, CHUNKSIZE)
    for(int i = 0; i < m_vSequences->size(); i++) {        
        for(int j = 0; j < m_vWords->size(); j++) {
            seqan::Finder<std::string> l_dFinderForward(m_vSequences->at(i));
            int l_iFoundCount = 0;
            seqan::Pattern<std::string, seqan::Horspool> l_dForwardWord(m_vWords->at(j));            
            while(seqan::find(l_dFinderForward, l_dForwardWord)) { l_iFoundCount++; }
            std::string l_sReverseWord = __utility::findReverseComplement(m_vWords->at(j));
            if(l_sReverseWord != m_vWords->at(j)) {
                seqan::Finder<std::string> l_dFinderReverse(m_vSequences->at(i));
                seqan::Pattern<std::string, seqan::Horspool> l_dReverseWord(l_sReverseWord);
                while(seqan::find(l_dFinderReverse, l_dReverseWord)) { l_iFoundCount++; }
            }
            if(l_iFoundCount > 0) {
                #pragma omp critical
                { l_dCompiledResults.at(m_vWords->at(j)).push_back(l_iFoundCount); }
            }
        }
        
        if(*m_bVerbose && i % (*m_iNumThreads + 1) == 0) {
            #pragma omp critical
            {
            double l_dFractionComplete = ((double)i / (double)m_vSequences->size()) * 100;
            outputCompletionStatus("\rPercent input sequences analyzed:", l_dFractionComplete, false);
            }
        }        
    }
    }
    if(*m_bVerbose) { outputCompletionStatus("\rPercent input sequences analyzed:", 100.00, true); }
    
    boost::unordered_map<std::string, WordStats*> l_dWordStatsHash;
    int l_iNumSeqs = m_vSequences->size();
    #pragma omp parallel private(th_id)
    {
    #pragma omp for schedule(dynamic, CHUNKSIZE)    
    for(int i = 0; i < m_vWords->size(); i++) {        
        std::string l_sWord = m_vWords->at(i);        
        int l_iSum = 0, l_iNumHits = l_dCompiledResults.at(l_sWord).size();        
        double l_dMean = 0.0, l_dStddev = 0.0, l_dProb = 0.0;
        for(int j = 0; j < l_iNumHits; j++) { l_iSum += l_dCompiledResults.at(l_sWord).at(j); }
        for(int j = 0; j < l_iNumSeqs - l_iNumHits; j++) { l_dCompiledResults.at(l_sWord).push_back(0); }            
        l_dMean = double(l_iSum) / double(l_iNumSeqs);
        for(int j = 0; j < l_iNumSeqs; j++) {
            l_dStddev += pow((l_dMean - double(l_dCompiledResults.at(l_sWord).at(j))), 2);
        }
        l_dStddev = sqrt(l_dStddev / double(l_iNumSeqs));        
        l_dProb = double(l_iNumHits) / double(l_iNumSeqs);
        WordStats* l_dTempWordStats = new WordStats(l_sWord, l_iSum, l_dMean, l_dStddev, l_dProb, l_iNumHits);
        #pragma omp critical
        {
        l_dWordStatsHash.insert(std::make_pair(l_sWord, l_dTempWordStats));
        }
        
        if(*m_bVerbose && i % (*m_iNumThreads + 1) == 0) {
            #pragma omp critical
            {
            double l_dFractionComplete = ((double)i / (double)m_vWords->size()) * 100;
            outputCompletionStatus("\rPercent data aggregated:", l_dFractionComplete, false);
            }
        }
    }
    }
    if(*m_bVerbose) { outputCompletionStatus("\rPercent data aggregated:", 100.00, true); }
    
    for(int i = 0; i < m_vWords->size(); i++) {
        m_vResults->push_back(l_dWordStatsHash.at(m_vWords->at(i)));
        m_dResultsHash->insert(std::make_pair(m_vWords->at(i), l_dWordStatsHash.at(m_vWords->at(i))));
    }
    
    for(int i = 0; i < m_vResults->size(); i++) {
        std::stringstream l_dStringStream;
        l_dStringStream << m_vResults->at(i)->getWord() << "\t";
        l_dStringStream << m_vResults->at(i)->getCount() << "\t";
        l_dStringStream << m_vResults->at(i)->getMean() << "\t";
        l_dStringStream << m_vResults->at(i)->getStddev() << "\t";
        l_dStringStream << m_vResults->at(i)->getProb() << "\t";
        l_dStringStream << m_vResults->at(i)->getHitSeqs();
        std::string l_sTempResult(l_dStringStream.str());
        m_vResultsAsStrings->push_back(l_sTempResult);
    }
    
    return true;
}

bool Counter::compileWordCountResults(std::vector<boost::unordered_map<std::string, int> > p_dResultsList) {
    typedef boost::unordered_map<std::string, int > l_tStringIntMap;
    boost::unordered_map<std::string, std::vector<int> > l_dCompiledResults;
    std::string l_sWord = "";
    for(int i = 0; i < p_dResultsList.size(); i++) {
        BOOST_FOREACH(l_tStringIntMap::value_type j, p_dResultsList.at(i)) {
            if(l_dCompiledResults.find(j.first) == l_dCompiledResults.end()) {
                std::vector<int> l_dTempCounts;
                l_dTempCounts.push_back(j.second);
                l_dCompiledResults.insert(std::make_pair(j.first, l_dTempCounts));
            } else {
                l_dCompiledResults.at(j.first).push_back(j.second);
            }            
        }
    }
    return true;
}

void Counter::outputCompletionStatus(std::string p_sTag, double p_dPercentDone, bool p_bDone) {    
    std::cout << boost::str(boost::format("%s %3.2f") %p_sTag %p_dPercentDone);
    if(p_bDone) { std::cout << std::endl; }
    fflush(stdout);
}

WordStats* Counter::getWordStatResult(std::string p_sWord) {
    WordStats* l_dReturnWordStats = NULL;
    if(m_dResultsHash->find(p_sWord) != m_dResultsHash->end()) { l_dReturnWordStats = m_dResultsHash->at(p_sWord); }    
    return l_dReturnWordStats;
}

std::vector<WordStats* >* Counter::getWordStatsResults() {
    if(m_vResults == NULL) { this->countWordsHorspool(); }
    return m_vResults;
}

std::vector<std::string>* Counter::getStringResults() {
    if(m_vResults == NULL) { this->countWordsHorspool(); }
    return m_vResultsAsStrings;
}

bool Counter::addWordStats(std::string p_sWord, WordStats* p_dWordStats) {
    bool l_bReturnStatus = true;
    if(m_dResultsHash->find(p_sWord) == m_dResultsHash->end()) {
        m_vWords->push_back(p_sWord);
        m_vResults->push_back(p_dWordStats);
        m_dResultsHash->insert(std::make_pair(p_sWord, p_dWordStats));
    } else {
        l_bReturnStatus = false;
    }
    return l_bReturnStatus;
}

bool Counter::addWordStats(std::string p_sWord, int p_iCount, double p_dMean, double p_dStddev, double p_dProb, 
        int p_iHitSeqs) {
    bool l_bReturnStatus = true;
    if(m_dResultsHash->find(p_sWord) == m_dResultsHash->end()) {
        WordStats* l_dTempWordStats = new WordStats(p_sWord, p_iCount, p_dMean, p_dStddev, p_dProb, p_iHitSeqs);
        m_vWords->push_back(p_sWord);
        m_vResults->push_back(l_dTempWordStats);       
        m_dResultsHash->insert(std::make_pair(p_sWord, l_dTempWordStats));
    } else {
        l_bReturnStatus = false;
    }
    return l_bReturnStatus;
}
