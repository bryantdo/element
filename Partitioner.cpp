#include "Partitioner.h"

Partitioner::Partitioner(const int p_iElements, const int p_iSubsets, const int p_iMaxCeil) :
        m_iElements(p_iElements), m_iSubsets(p_iSubsets), m_iMaxCeil(p_iMaxCeil) {    
    if(m_iMaxCeil == PARTIT_BASE_NO_MAXCEIL || m_iMaxCeil > m_iElements - m_iSubsets + 1) {
        m_iMaxCeil = m_iElements - m_iSubsets + 1;   
    }
    m_iMaxElements = intCeil(m_iElements, m_iSubsets);
    
    if(m_iElements <= 0 || m_iSubsets <= 0 || m_iMaxCeil <= 0 || m_iMaxElements > m_iMaxCeil) { 
        m_dChildPartitioner = 0;
    } else {
        m_dChildPartitioner = (m_iSubsets > 1 && m_iMaxElements >= 1) ? 
           new Partitioner(m_iElements - m_iMaxElements, m_iSubsets - 1, m_iMaxElements) : 0;
    }
}

Partitioner::~Partitioner() { delete m_dChildPartitioner; }

std::vector<int> Partitioner::next() {
    std::vector<int> l_vResults;
    
    if(m_dChildPartitioner) { l_vResults = m_dChildPartitioner->next(); }
    else if(m_iSubsets == 1) {
        l_vResults.clear();
        if(m_iMaxElements++ <= m_iMaxCeil) { l_vResults.push_back(m_iElements); }
        return l_vResults;
    } else { return l_vResults; }

    if(l_vResults.size()) { l_vResults.push_back(m_iMaxElements); }
    else {
        delete m_dChildPartitioner; m_dChildPartitioner = 0;
        if(++m_iMaxElements <= m_iMaxCeil) {
            m_dChildPartitioner = (m_iMaxElements >= 1) ? 
                new Partitioner(m_iElements - m_iMaxElements, m_iSubsets - 1, m_iMaxElements) : 0;
            return next();
        }        
    }
    return l_vResults;
}