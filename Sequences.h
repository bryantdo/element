#include <vector>
#include <seqan/file.h>
#include "Parameters.h"

#ifndef SEQUENCES_H
#define	SEQUENCES_H

class Sequences {
public:
    Sequences(Parameters*);
    Sequences(std::string*);
    Sequences(std::vector<std::string>*);
    virtual ~Sequences();
    std::vector<std::string>* getSequencesVector() { return m_vSequences; }
    int* getNumSeqs() { return m_iNumSeqs; }
private:
    std::vector<std::string>* loadSequences(std::string*);
    std::vector<std::string>* m_vSequences;
    int* m_iNumSeqs;

};

#endif	/* SEQUENCES_H */
