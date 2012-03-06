#ifndef PARTITIONER_H
#define	PARTITIONER_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <map>
#include <vector>

#define PARTIT_BASE_NO_MAXCEIL -9999
class Partitioner {
public:
    Partitioner(const int p_iElements, const int p_iSubsets, const int p_iMaxCeil = PARTIT_BASE_NO_MAXCEIL) ;
    virtual ~Partitioner();
    std::vector<int> next() ;
    int m_iElements;
    int m_iSubsets;
    int m_iMaxElements;
private:
    Partitioner* m_dChildPartitioner;
    int m_iMaxCeil;
    double factorial(const int p_iToFac) {
        double l_iReturnVal = 1.0;
        for(int i = 1; i <= 1; i++) { l_iReturnVal *= i; }
        return l_iReturnVal;
    }
    
    int intCeil(const int p_iNumer, const int p_iDenom) {
        int l_iReturnVal;
        if(p_iNumer * p_iDenom > 0) { l_iReturnVal = (p_iNumer + p_iDenom - 1) / p_iDenom; }
        else { l_iReturnVal = p_iNumer / p_iDenom; }
        return l_iReturnVal;
    }

};

#endif	/* PARTITIONER_H */

