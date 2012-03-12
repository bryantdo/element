#include "Elementer.h"

Elementer::Elementer(Words* p_dWords, std::vector<Sequences* >* p_vSequences, 
        std::vector<Counter* >* p_vBackgroundCounters, int* p_iNumThreads) {
    m_bVerbose                  = new bool(true);
    m_dWords                    = p_dWords->getWordsHash();
    m_vWords                    = p_dWords->getWordsVector();
    m_iNumThreads               = p_iNumThreads;
    m_dPartitions               = new Partitions();
    m_vSequences                = p_vSequences;
    m_vBackgroundCounters       = p_vBackgroundCounters;
    m_iMixtureSize              = new int(m_vBackgroundCounters->size());
    m_vResults                  = new std::vector<WordStatsObserved* >();
    m_dResultsHash              = new boost::unordered_map<std::string, WordStatsObserved* >();
    m_vResultsAsStrings         = new std::vector<std::string>();
    m_vSequencesCounters        = new std::vector<Counter* >();
    
    std::vector<std::string> l_vInputSeqs;
    for(unsigned int i = 0; i < p_vSequences->size(); i++) {
        std::vector<std::string>* l_vTempSeqs = p_vSequences->at(i)->getSequencesVector();
        l_vInputSeqs.insert(l_vInputSeqs.end(), l_vTempSeqs->begin(), l_vTempSeqs->end());
    }
    Sequences l_dQuerySequences(&l_vInputSeqs);
    m_dQueryCounter             = new Counter(p_dWords, &l_dQuerySequences, p_iNumThreads);    
    this->calculateProbabilities();
}

Elementer::Elementer(std::string* p_sInputFileName) {
    m_bVerbose                  = new bool(true);
    m_dPartitions               = NULL;
    m_iMixtureSize              = NULL;
    m_dQueryCounter             = NULL;
    m_vSequencesCounters        = NULL;
    m_vWords                    = new std::vector<std::string>();
    m_vResults                  = new std::vector<WordStatsObserved* >();
    m_dResultsHash              = new boost::unordered_map<std::string, WordStatsObserved* >();
    m_vResultsAsStrings         = new std::vector<std::string>();
    this->loadProbsFromFile(p_sInputFileName);
}

Elementer::~Elementer() {
    delete m_bVerbose;
    delete m_dPartitions;
    delete m_iMixtureSize;
    delete m_dQueryCounter;
    delete m_vSequencesCounters;
    delete m_vResults;
    delete m_dResultsHash;
    delete m_vResultsAsStrings;
}

bool Elementer::loadProbsFromFile(std::string* p_sInputFileName) {
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
        double          l_dPValue;
        
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
        try { l_dPValue = boost::lexical_cast<double>(l_vTokens.at(6)); }
        catch(const std::exception&) { l_dPValue = -1; }
        
        if(l_sWord.size() > 0 && l_iCount != -1 && l_dMean != -1 && l_dStddev != -1 && l_dProb != -1 
                && l_iHitSeqs != -1 && l_dPValue != -1) {
            this->addWordStats(l_sWord, l_iCount, l_dMean, l_dStddev, l_dProb, l_iHitSeqs, l_dPValue);
        } else { l_bReturnStatus = false; }
    }
    l_dInputFileStream.close();
    return l_bReturnStatus;
}

bool Elementer::addWordStats(std::string p_sWord, int p_iCount, double p_dMean, double p_dStddev, 
        double p_dProb, int p_iHitSeqs, double p_dPValue) {
    bool l_bReturnStatus = true;
    if(m_dResultsHash->find(p_sWord) == m_dResultsHash->end()) {
        WordStatsObserved* l_dTempWordStatsObserved = 
                new WordStatsObserved(p_sWord, p_iCount, p_dMean, p_dStddev, p_dProb, p_iHitSeqs, p_dPValue);
        m_vWords->push_back(p_sWord);
        m_vResults->push_back(l_dTempWordStatsObserved);       
        m_dResultsHash->insert(std::make_pair(p_sWord, l_dTempWordStatsObserved));
    } else {
        l_bReturnStatus = false;
    }
    return l_bReturnStatus;
}

bool Elementer::calculateProbabilities() {
    using boost::math::binomial;
    Partitions* l_dPartitions = new Partitions();
    int l_iTotalNumQuerySeqs = 0; 
    for(unsigned int i = 0; i < m_vSequences->size(); i++) {
        l_iTotalNumQuerySeqs += m_vSequences->at(i)->getSequencesVector()->size();
    }    
    omp_set_num_threads(*m_iNumThreads);
    #define CHUNKSIZE 1
    int th_id;
    #pragma omp parallel private(th_id)
    {
    #pragma omp for schedule(dynamic, CHUNKSIZE)
    for(unsigned int i = 0; i < m_vWords->size(); i++) {
        int l_iObserved    = m_dQueryCounter->getWordStatResult(m_vWords->at(i))->getHitSeqs();
        long double l_dPValue   = 0.;        
        if(*m_iMixtureSize == 1) {
            Counter* l_dBackgroundCounter       = m_vBackgroundCounters->at(0);
            double l_iPrior                     = l_dBackgroundCounter->getWordStatResult(m_vWords->at(i))->getProb();
            binomial l_dBinomial(l_iTotalNumQuerySeqs, l_iPrior);
            int l_iObservedComplement = l_iObserved == 0 ? 0 : l_iObserved - 1;
            l_dPValue = cdf(complement(l_dBinomial, l_iObservedComplement));
        } else if(*m_iMixtureSize > 1) {
            for(int j = l_iObserved; j <= l_iTotalNumQuerySeqs; j++) {
                std::vector<std::vector<int> >* l_vPartitionings;                
                #pragma omp critical
                { l_vPartitionings = l_dPartitions->getPartitions(j, *m_iMixtureSize); }                
                for(unsigned int k = 0; k < l_vPartitionings->size(); k++) {
                    bool l_bValidPartitioning = true;
                    long double l_dProduct = 1.;
                    for(int l = 0; l < *m_iMixtureSize; l++) {
                        Counter* l_dBackgroundCounter = m_vBackgroundCounters->at(l);
                        double l_iPartialPrior = l_dBackgroundCounter->getWordStatResult(m_vWords->at(i))->getProb();
                        int l_iNumPartialQuerySeqs = *(m_vSequences->at(l)->getNumSeqs());                        
                        if(l_iNumPartialQuerySeqs >= l_vPartitionings->at(k).at(l) && l_bValidPartitioning) {
                            binomial l_dBinomial(l_iNumPartialQuerySeqs, l_iPartialPrior);
                            l_dProduct *= pdf(l_dBinomial, l_vPartitionings->at(k).at(l));
                        } else {
                            l_bValidPartitioning = false;
                        }
                    }
                    if(l_bValidPartitioning) { l_dPValue += l_dProduct; }
                }
            }
        }
        WordStats* l_dTempWordStats = m_dQueryCounter->getWordStatResult(m_vWords->at(i));
        if(l_dTempWordStats != NULL) {
            WordStatsObserved* l_dTempWordStatsObserved = new WordStatsObserved(l_dTempWordStats, l_dPValue);
            #pragma omp critical 
            { m_dResultsHash->insert(std::make_pair(m_vWords->at(i), l_dTempWordStatsObserved)); }
        }
    }
    }    
    for(unsigned int i = 0; i < m_vWords->size(); i++) {
        if(m_dResultsHash->find(m_vWords->at(i)) != m_dResultsHash->end()) {
            WordStatsObserved* l_dTempWordStatsObserved = m_dResultsHash->at(m_vWords->at(i));
            m_vResults->push_back(l_dTempWordStatsObserved);
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
            m_vResultsAsStrings->push_back(l_sTempResult);
        }
    }    
    delete l_dPartitions;
    return true;
}

WordStatsObserved* Elementer::getWordStatResult(std::string p_sWord) {
    WordStatsObserved* l_dReturnWordStats = NULL;
    if(m_dResultsHash->find(p_sWord) != m_dResultsHash->end()) { l_dReturnWordStats = m_dResultsHash->at(p_sWord); }    
    return l_dReturnWordStats;
}

std::vector<WordStatsObserved* >* Elementer::getWordStatsResults() {
    return m_vResults;
}

std::vector<std::string>* Elementer::getStringResults() {
    return m_vResultsAsStrings;
}
