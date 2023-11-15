#include "aufgabe2.hpp"

#include <cstdint>
#include <algorithm>
#include <iostream> //DEBUG


// This function offers four functionalities:
// 1. Compare pattern <= given suffix text (lessEqual = 0)
// 2. Compare pattern > given suffix text (lessEqual = 1)
// 2. Compare pattern >= given suffix text (lessEqual = 2)
// 2. Compare pattern < given suffix text (lessEqual = 3)
bool patternCompare(const std::string& pattern, const std::vector<uint32_t>& sa, const std::string& suffixText, const uint32_t& startingPosition, short lessEqual) {
    uint32_t i;
    for (i = 0;
    (i < pattern.length())
    && (startingPosition+i < suffixText.length())
    && pattern[i] == suffixText[startingPosition+i]; i++) {}

    char patternChar = (i < pattern.length()) ? pattern[i] : '\0';
    char suffixChar = (startingPosition + i < suffixText.length()) ? suffixText[startingPosition + i] : '\0';

    std::cout << "COMPARE " << patternChar << " WITH " << suffixChar << "\n";

    switch (lessEqual) {
        case 0: return patternChar <= suffixChar;
        case 1: return patternChar > suffixChar;
        case 2: return patternChar >= suffixChar;
        case 3: return patternChar < suffixChar;
        default: return false;
    }

}


void construct(std::vector<uint32_t>& sa, const std::string& text) {

    uint32_t len = text.length();
    sa.resize(len);
    size_t counter = 0;

    for (uint32_t i = 0; i < len; i++)
        sa[i] = i;

    // Sorting our array with a custom lambda expression (as per cppreference.com) which sorts by comparing the text of two sa entries
    // &text and &counter are captured, so that we can use them for lookup inside of our lambda function
    // a and b are automatically taken from sa, which holds values from 1..len (our suffix start positions)
    // Two entries are compared by their ASCII values and return 1 if the a-suffix is smaller than the b-suffix, otherwise 0.
    std::sort(sa.begin(), sa.end(), [&text, &counter](const uint32_t & a, const uint32_t & b) {
        for (counter = 0; text[a+counter] == text[b+counter];)
            counter++;

        return (text[a+counter] < text[b+counter]);
    });
    return;
}

void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits) {
    uint32_t lp = 0;
    uint32_t rp = 0;
    uint32_t L = 0;
    uint32_t R = 0;
    uint32_t m = 0;

    if (patternCompare(query, sa, text, sa[0], 0)) {
        lp = 0;
    }

    else if (patternCompare(query, sa, text, sa[sa.size()-1], 1)) {
        lp = sa.size();
    }

    else {
        L = 0;
        R = sa.size()-1;

        while (R - L > 1) {
            m = (L + R)/2;
            std::cout << "(L,R) = (" << L << "," << R << ") => M = " << m << "\n";
            if (patternCompare(query, sa, text, sa[m], 0)) {
                R = m;
            }
            else {
                L = m;
            }
        }
        lp = R;
    }
    std::cout << "Final value: (L,R) = (" << L << "," << R << ")" << "\n";
    std::cout << "\n\nLp = " << lp << "\n\n\n";







    if (patternCompare(query, sa, text, sa[sa.size()-1], 2)) {
        std::cout << query << " <= T[suftab[0]]\n";
        rp = sa.size()-1;
    }

    else if (patternCompare(query, sa, text, sa[0], 3)) {
        std::cout << query << " > T[suftab[n]] with value: " << sa[sa.size()-1] << "\n";
        rp = 0;
    }

    else {
        L = lp;
        R = sa.size()-1;

        while (R - L > 1) {
            m = (L + R+1)/2;
            std::cout << "(L,R) = (" << L << "," << R << ") => M = " << m << "\n";
            if (patternCompare(query, sa, text, sa[m], 3)) {
            //if ((std::lexicographical_compare(query.begin(), query.end(), text.begin()+sa[m], text.end()))) {
                L = m;
                std::cout << query << " < " << text.substr(sa[m]) << "\n";
            }
            else {
                R = m;
                std::cout << query << " >= " << text.substr(sa[m]) << "\n";
            }
        }
        rp = R;
    }
    std::cout << "Final value: (L,R) = (" << L << "," << R << ")" << "\n";
    std::cout << "\n\nRp = " << rp << "\n\n\n";

    hits.clear();
    for (uint32_t i = lp; i <= rp; i++) {
        hits.push_back(sa[i]);
    }
    std::sort(hits.begin(), hits.end());
}