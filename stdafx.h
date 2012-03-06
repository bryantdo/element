#ifndef STDAFX_H
#define	STDAFX_H

#include <iostream>

namespace __utility {
    static std::string findReverseComplement(std::string* p_sInputRead) {
        const std::string l_sInputRead = *p_sInputRead;
        std::string l_sReverseComplementRead = "";
        std::string::const_iterator l_siReadIterator = l_sInputRead.end();
        while (l_siReadIterator != l_sInputRead.begin()) {
            l_siReadIterator--;
            switch (*l_siReadIterator) {
                case 'a':
                case 'A':
                    l_sReverseComplementRead += "T";
                    break;
                case 'c':
                case 'C':
                    l_sReverseComplementRead += "G";
                    break;
                case 't':
                case 'T':
                    l_sReverseComplementRead += "A";
                    break;
                case 'g':
                case 'G':
                    l_sReverseComplementRead += "C";
                    break;
                default:
                    //throw exception
                    break;
            }
        }
        return l_sReverseComplementRead;
    }

    static std::string findReverseComplement(std::string p_sInputRead) {
        std::string l_sReverseComplementRead = "";
        std::string::const_iterator l_siReadIterator = p_sInputRead.end();
        while (l_siReadIterator != p_sInputRead.begin()) {
            l_siReadIterator--;
            switch (*l_siReadIterator) {
                case 'a':
                case 'A':
                    l_sReverseComplementRead += "T";
                    break;
                case 'c':
                case 'C':
                    l_sReverseComplementRead += "G";
                    break;
                case 't':
                case 'T':
                    l_sReverseComplementRead += "A";
                    break;
                case 'g':
                case 'G':
                    l_sReverseComplementRead += "C";
                    break;
                default:
                    //throw exception
                    break;
            }
        }
        return l_sReverseComplementRead;
    }
}

#endif	/* STDAFX_H */

