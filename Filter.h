#ifndef FILTER_H
#define	FILTER_H

#include <iostream>
#include <stdlib.h>
#include "Parameters.h"
#include "Filterer.h"

class Filter {
public:
    Filter(int, char**);
    virtual ~Filter();
private:
    void outputUsage();
    bool run();
    FilterParameters*   m_dFilterParameters;
};

#endif	/* FILTER_H */
