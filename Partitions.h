#ifndef PARTITIONS_H
#define	PARTITIONS_H

#include <boost/unordered_map.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include "Partitioner.h"

class Partitions {
public:
    Partitions();
    virtual ~Partitions();
    std::vector<std::vector<int> >* getPartitions(const int, const int);
private:
    bool populatePartitionings(const int, const int);
    boost::unordered_map<std::vector<int>, std::vector<std::vector<int> > >* m_dPartitionings;
};

#endif	/* PARTITIONS_H */

