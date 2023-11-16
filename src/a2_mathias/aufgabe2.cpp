#include "aufgabe2.hpp"

#include <cstdint>
#include <algorithm>
#include <iostream> //DEBUG

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
    uint32_t M = 0;
    uint32_t suffixMax = sa.size()-1;

    if (query <= text.substr(sa[0]))
        lp = 0;

    else if (query > text.substr(sa[suffixMax]))
        lp = suffixMax+1;

    else {
        L = 0;
        R = suffixMax;

        while (R - L > 1) {
            M = (L + R)/2;
            //std::cout << "(L,R) = (" << L << "," << R << ") => M = " << m << "\n";
            if (query<= text.substr(sa[M]))
                R = M;
            else
                L = M;
        }
        lp = R;
    }
    //std::cout << "Final value: (L,R) = (" << L << "," << R << ")" << "\n";
    std::cout << "\n\nLp = " << lp << "\n";

    if (query >= text.substr(sa[suffixMax]))
        rp = suffixMax+1;

    else if (query < text.substr(sa[0]))
        rp = 0;

    else {
        L = lp;
        R = sa.size()-1;

        while (R - L > 1) {
            M = (L + R+1)/2;
            //std::cout << "(L,R) = (" << L << "," << R << ") => M = " << m << "\n";
            if (query < text.substr(sa[M], query.length()))
                R = M;
            else
                L = M;
        }
        rp = L;
    }
    //std::cout << "Final value: (L,R) = (" << L << "," << R << ")" << "\n";
    std::cout << "Rp = " << rp << "\n";

    hits.clear();
    for (uint32_t i = lp; i <= rp; i++) {
        if (i < sa.size()-1)
            hits.push_back(sa[i]);
    }
    std::sort(hits.begin(), hits.end());
}