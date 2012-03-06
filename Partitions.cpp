#include "Partitions.h"

Partitions::Partitions() {
    m_dPartitionings = new boost::unordered_map<std::vector<int>, std::vector<std::vector<int> > >();
}

Partitions::~Partitions() {
    delete m_dPartitionings;
}

std::vector<std::vector<int> >* Partitions::getPartitions(const int p_iSum, const int p_iSubsets) {
    std::vector<std::vector<int> >* l_dReturnPartitions;
    std::vector<int> l_dParameters(p_iSum, p_iSubsets);
    if(m_dPartitionings->find(l_dParameters) == m_dPartitionings->end()) {
        populatePartitionings(p_iSum, p_iSubsets);
    }
    l_dReturnPartitions = &(m_dPartitionings->at(l_dParameters));    
    return l_dReturnPartitions;
}

bool Partitions::populatePartitionings(const int p_iSum, const int p_iSubsets) {
    std::vector<std::vector<int> > l_dPartitionings;
    std::vector<int> l_dParameters(p_iSum, p_iSubsets);
    for(int i = 1; i <= p_iSubsets; i++) {        
        Partitioner l_dPartitioner(p_iSum, i);
        std::vector<int> l_dIntermediatePartitioning;
        do {
            l_dIntermediatePartitioning = l_dPartitioner.next();
            if(l_dIntermediatePartitioning.size()) {
                
                for(int j = 0; j < p_iSubsets - i; j++) {
                    l_dIntermediatePartitioning.push_back(0);
                }
                
                std::sort(l_dIntermediatePartitioning.begin(), l_dIntermediatePartitioning.end());
                l_dPartitionings.push_back(l_dIntermediatePartitioning);
                while(std::next_permutation(l_dIntermediatePartitioning.begin(), l_dIntermediatePartitioning.end())) {
                    l_dPartitionings.push_back(l_dIntermediatePartitioning);
                }
            }
        } while(l_dIntermediatePartitioning.size());
        }
    m_dPartitionings->insert(std::make_pair(l_dParameters, l_dPartitionings));
    return true;
}