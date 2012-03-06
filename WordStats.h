#ifndef WORDSTATS_H
#define	WORDSTATS_H

#include <string>

class WordStats {
public:
    WordStats();
    WordStats(std::string, int, double, double, double, int);
    virtual ~WordStats();
    void setWord(std::string p_sWord) { *m_sWord = p_sWord; }
    std::string getWord() { return *m_sWord; }
    void setCount(int p_iCount) { *m_iCount = p_iCount; }
    int getCount() { return *m_iCount; }
    void setMean(double p_dMean) { *m_dMean = p_dMean; }
    double getMean() { return *m_dMean; }
    void setStddev(double p_dStddev) { *m_dStddev = p_dStddev; }
    double getStddev() { return *m_dStddev; }    
    void setProb(double p_dProb) { *m_dProb = p_dProb; }
    double getProb() { return *m_dProb; }
    void setHitSeqs(int p_iHitSeqs) { *m_iHitSeqs = p_iHitSeqs; }
    int getHitSeqs() { return *m_iHitSeqs; }
protected:
    std::string*        m_sWord;
    int*                m_iCount;
    double*             m_dMean;
    double*             m_dStddev;
    double*             m_dProb;
    int*                m_iHitSeqs;
};

class WordStatsBackground : public WordStats {
public:
    WordStatsBackground();
    WordStatsBackground(std::string, int, double, double, double, int);
    ~WordStatsBackground();
private:
};

class WordStatsObserved : public WordStats {
public:
    WordStatsObserved();
    WordStatsObserved(std::string, int, double, double, double, int, double);
    WordStatsObserved(WordStats*, double);
    ~WordStatsObserved();
    void setPValue(double p_dPValue) ;
    double getPValue() { return *m_dPValue; }
private:
    double* m_dPValue;
};


#endif	/* WORDSTATS_H */
