#ifndef PARAMETERS_H
#define	PARAMETERS_H

#include <string>
#include <vector>

class Parameters {
public:
    Parameters();
    Parameters(const Parameters& orig);
    virtual ~Parameters();
    void setInputWordsFileName(std::string p_sName) { *m_sInputWordsFileName = p_sName; }
    std::string* getInputWordsFileName() { return m_sInputWordsFileName; }
    void setInputSequencesFileName(std::string p_sName) { *m_sInputSequencesFileName = p_sName; }
    std::string* getInputSequencesFileName() { return m_sInputSequencesFileName; }
    void setOutputFileName(std::string p_sName) { *m_sOutputFileName = p_sName; }
    std::string getOutputFileName() { return *m_sOutputFileName; }
    void setNumThreads(int p_iNumThreads) { *m_iNumThreads = p_iNumThreads; }
    int* getNumThreads() { return m_iNumThreads; }
    virtual bool verifyParameters();
protected:
    std::string* m_sInputWordsFileName;
    std::string* m_sInputSequencesFileName;
    std::string* m_sOutputFileName;
    int*         m_iNumThreads;
};

class CountParameters : public Parameters {
public:
    CountParameters() {
        m_sInputWordsFileName       = new std::string("");
        m_sInputSequencesFileName   = new std::string("");
        m_sOutputFileName           = new std::string("");
        m_iNumThreads               = new int(1);
    };
    ~CountParameters(){
        delete m_sInputWordsFileName;
        delete m_sInputSequencesFileName;
        delete m_sOutputFileName;
        delete m_iNumThreads;    
    };
private:
};

class ElementParameters : public Parameters {
public:
    ElementParameters();
    virtual ~ElementParameters();
    bool verifyParameters();
    void setBackgroundStatsFileNames(std::vector<std::string>);
    std::vector<std::string>* getBackgroundStatsFileNames();
    void setInputFileNames(std::vector<std::string>);
    std::vector<std::string>* getSequencesFileNames();
private:
    std::vector<std::string>* m_vSequencesFileNames;
    std::vector<std::string>* m_vBackgroundStatsFileNames;
};

#endif	/* PARAMETERS_H */
