#ifndef ELEMENT_H
#define	ELEMENT_H

#include <iostream>
#include <vector>
#include <string>
#include "Parameters.h"
#include "Words.h"
#include "Sequences.h"
#include "Counter.h"
#include "Elementer.h"

class Element {
public:
    Element(int, char**);
    virtual ~Element();
private:
    void outputUsage();
    bool run();
    ElementParameters*          m_dElementParameters;
    Words*                      m_dWords;
    Sequences*                  m_dSequences;
    std::vector<Sequences* >*   m_vSequences;
    std::vector<Counter* >*     m_vBGroundCounters;
};

#endif	/* ELEMENT_H */
