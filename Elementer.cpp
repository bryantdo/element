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
    for(int i = 0; i < p_vSequences->size(); i++) {
        std::vector<std::string>* l_vTempSeqs = p_vSequences->at(i)->getSequencesVector();
        l_vInputSeqs.insert(l_vInputSeqs.end(), l_vTempSeqs->begin(), l_vTempSeqs->end());
        /*
        Counter* l_dTempCounter = new Counter(p_dWords, p_vSequences->at(i), p_iNumThreads);
        m_vSequencesCounters->push_back(l_dTempCounter);
        */
    }
    Sequences l_dQuerySequences(&l_vInputSeqs);
    m_dQueryCounter             = new Counter(p_dWords, &l_dQuerySequences, p_iNumThreads);
    
    this->calculateProbabilities();
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

bool Elementer::calculateProbabilities() {
    using boost::math::binomial;
    Partitions* l_dPartitions = new Partitions();
    int l_iTotalNumQuerySeqs = 0; 
    for(int i = 0; i < m_vSequences->size(); i++) {
        l_iTotalNumQuerySeqs += m_vSequences->at(i)->getSequencesVector()->size();
    }    
    omp_set_num_threads(*m_iNumThreads);
    #define CHUNKSIZE 1
    int th_id;
    #pragma omp parallel private(th_id)
    {
    #pragma omp for schedule(dynamic, CHUNKSIZE)
    for(int i = 0; i < m_vWords->size(); i++) {
        int l_iObserved    = m_dQueryCounter->getWordStatResult(m_vWords->at(i))->getHitSeqs();
        double l_dPValue   = 0.;        
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
                for(int k = 0; k < l_vPartitionings->size(); k++) {
                    long double l_dProduct = 1.;
                    bool l_bValidPartitioning = true;
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
                    if(l_bValidPartitioning) { l_dPValue += (double)l_dProduct; }
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
    for(int i = 0; i < m_vWords->size(); i++) {
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
