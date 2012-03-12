#include "Sequences.h"

Sequences::Sequences(Parameters* p_dParameters) {
    m_vSequences        = Sequences::loadSequences(p_dParameters->getInputSequencesFileName());
    m_iNumSeqs          = new int(m_vSequences->size());
}

Sequences::Sequences(std::string* p_sInputFileName) {
    m_vSequences        = Sequences::loadSequences(p_sInputFileName);
    m_iNumSeqs          = new int(m_vSequences->size());
}

Sequences::Sequences(std::vector<std::string>* p_vSequences) {
    m_vSequences        = new std::vector<std::string>(*p_vSequences);
    m_iNumSeqs          = new int(m_vSequences->size());
}

Sequences::~Sequences() {
    delete m_vSequences;
    delete m_iNumSeqs;    
}

std::vector<std::string>* Sequences::loadSequences(std::string* p_sInputSequencesFileName) {
    using namespace seqan;
    std::vector<std::string>* l_vSequences = new std::vector<std::string>();
    MultiSeqFile l_dMultiSeqFile;
    open(l_dMultiSeqFile.concat, p_sInputSequencesFileName->c_str(), OPEN_RDONLY);
    AutoSeqFormat l_dFormat;
    guessFormat(l_dMultiSeqFile.concat, l_dFormat);
    split(l_dMultiSeqFile, l_dFormat);
    unsigned l_uSeqCount = length(l_dMultiSeqFile);
    CharString l_csID;
    String<Dna5Q> l_sSeq;
    for(unsigned int i = 0; i < l_uSeqCount; i++) {
        assignSeq(l_sSeq, l_dMultiSeqFile[i], l_dFormat);
        std::string l_sSequence; assign(l_sSequence, l_sSeq);
        std::transform(l_sSequence.begin(), l_sSequence.end(), l_sSequence.begin(), ::toupper);
        l_vSequences->push_back(l_sSequence);
    }
    return l_vSequences;
}
