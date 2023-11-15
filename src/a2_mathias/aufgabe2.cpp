#include "aufgabe2.hpp"

#include <cstdint>
#include <algorithm>
#include <iostream> //DEBUG


// This function offers four functionalities:
// 1. Compare pattern <= given suffix text (lessEqual = 0)
// 2. Compare pattern > given suffix text (lessEqual = 1)
// 3. Compare pattern >= given suffix text (lessEqual = 2)
// 4. Compare pattern < given suffix text (lessEqual = 3)
bool patternCompare(const std::string& pattern, const std::string& suffixText, const uint32_t& startingPosition, short comparison = 3) {
    uint32_t i;
    for (i = 0;
    (i < pattern.length())
    && (startingPosition+i < suffixText.length())
    && pattern[i] == suffixText[startingPosition+i]; i++) {}

    char patternChar = pattern[i];
    char suffixChar = suffixText[startingPosition + i];

    std::cout << "COMPARE " << patternChar << " WITH " << suffixChar << "\n";

    switch (comparison) {
        case 0: return patternChar <= suffixChar;
        case 1: return patternChar > suffixChar;
        case 2: return patternChar >= suffixChar;
        case 3: return patternChar < suffixChar;
        default: return false;
    }

}

uint32_t findFirstDeviatingChar(const std::string& pattern, const std::string& suffixText, const uint32_t& startingPosition, bool scanOnlySubstring = false) {
    uint32_t i = 0;
    if (scanOnlySubstring == false)
        for (; i < suffixText.length() && startingPosition+i < suffixText.length() && pattern[i] == suffixText[startingPosition+i]; i++) {}
    else
        for (; i < pattern.length() && startingPosition+i < suffixText.length() && pattern[i] == suffixText[startingPosition+i]; i++) {}
    return i;
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
    uint32_t m, mDiff;
    uint32_t firstDiff = findFirstDeviatingChar(query, text, sa[0]);
    uint32_t lastDiff = findFirstDeviatingChar(query, text, sa[sa.size()-1]);
    uint32_t suffixMax = sa.size()-1;

    if (patternCompare(query, text, sa[0], 0)) {
    //if (query[firstDiff] <= text[firstDiff+sa[0]]) {
        lp = 0;
    }

    else if (patternCompare(query, text, sa[sa.size()-1], 1)) {
    //else if (query[lastDiff] > text[lastDiff+query.length()]) {
        lp = suffixMax;
    }

    else {
        L = 0;
        R = suffixMax;

        while (R - L > 1) {
            m = (L + R)/2;
            mDiff = findFirstDeviatingChar(query, text, sa[m]);
            std::cout << "(L,R) = (" << L << "," << R << ") => M = " << m << "\n";
            if (patternCompare(query, text, sa[m], 0)) {
            //if (query[firstDiff] <= text[firstDiff+sa[m]]) {
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







    if (patternCompare(query, text, sa[sa.size()-1], 2)) {
    //if (query[lastDiff] >= text[lastDiff+suffixMax]) {
        std::cout << query << " <= T[suftab[0]]\n";
        rp = sa.size()-1;
    }

    else if (patternCompare(query, text, sa[0], 3)) {
    //else if (query[firstDiff] < text[firstDiff+sa[0]]) {
        std::cout << query << " > T[suftab[n]] with value: " << sa[sa.size()-1] << "\n";
        rp = 0;
    }

    else {
        L = lp;
        R = sa.size()-1;

        while (R - L > 1) {
            m = (L + R+1)/2;
            mDiff = findFirstDeviatingChar(query, text, sa[m], true);
            std::cout << "(L,R) = (" << L << "," << R << ") => M = " << m << "\n";
            if (patternCompare(query, text.substr(query.length()), sa[m], 3)) {
            //if (query[mDiff] < text[mDiff+query.length()-1]) {
                L = m;
                std::cout << query.substr(0,query.length()) << " < " << text.substr(sa[m], query.length()) << "\n";
                std::cout << query << " < " << text.substr(sa[m]) << "\n";
            }
            else {
                R = m;
                std::cout << query.substr(0,query.length()) << " >= " << text.substr(0, query.length()) << "\n";
                std::cout << query << " >= " << text.substr(sa[m]) << "\n";
            }
        }
        rp = R-1;
    }
    std::cout << "Final value: (L,R) = (" << L << "," << R << ")" << "\n";
    std::cout << "\n\nRp = " << rp << "\n\n\n";

    hits.clear();
    for (uint32_t i = lp; i <= rp; i++) {
        hits.push_back(sa[i]);
    }
    std::sort(hits.begin(), hits.end());

}