#ifndef WORDS_H
#define	WORDS_H

#include <boost/unordered_map.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <string>
#include <vector>
#include "Parameters.h"
#include "stdafx.h"

class Words {
public:
    Words(Parameters*);
    Words(std::vector<std::string>*);
    virtual ~Words();
    boost::unordered_map<std::string, bool>* getWordsHash() { return m_dWords; }
    std::vector<std::string>* getWordsVector() { return m_vWords; }
private:
    bool loadWords(std::string*);
    boost::unordered_map<std::string, bool>*    m_dWords;
    std::vector<std::string>*                   m_vWords;
};

#endif	/* WORDS_H */
