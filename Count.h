#ifndef COUNT_H
#define	COUNT_H

#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include "Parameters.h"
#include "Words.h"
#include "Sequences.h"
#include "Counter.h"


class Count {
public:
    Count(int, char**);
    virtual ~Count();
private:
    void outputUsage();
    bool run();
    CountParameters*    m_dCountParams;
    Words*              m_dWords;
    Sequences*          m_dSequences;  
};

#endif	/* COUNT_H */
