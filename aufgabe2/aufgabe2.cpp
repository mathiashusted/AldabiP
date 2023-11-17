#include "aufgabe2.hpp"

#include <cstdint>
#include <algorithm>
// #include <iostream> //DEBUG


uint32_t calculateMLR(const std::string& firstString, const std::string& secondString) {
    /**
     * @brief Calculates the MLR of two given strings.
     * 
     * @param firstString The first string to be compared.
     * @param secondString The second string to be compared.
     * 
     * @return The MLR of the two given strings.
    */

    // Initial checks
    if (firstString.empty() || secondString.empty())
        return 0;

    // Calculate the MLR
    uint32_t mlr = 0;
    uint32_t minStringLength = std::min(firstString.length(), secondString.length());
    for (uint32_t i = 0; i < minStringLength; i++) {
        if (firstString[i] == secondString[i])
            mlr++;
        else
            break;
    }
    return mlr;
}

void construct(std::vector<uint32_t>& sa, const std::string& text) {
    /**
     * @brief Constructs the suffix array of a given text.
     * 
     * @param sa The suffix array to be constructed.
     * @param text The text to be used.
    */
    uint32_t len = text.length();
    sa.resize(len);

    for (uint32_t i = 0; i < len; i++)
        sa[i] = i;
    std::sort(sa.begin(), sa.end(), [&text](const uint32_t & a, const uint32_t & b) {
        return (std::lexicographical_compare((text.begin() + a), text.end(), (text.begin() + b), text.end()));
    });
    return;
}

void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits) {
    /**
     * @brief Finds all occurences of a given query in a given text.
     * 
     * @param query The query to be searched for.
     * @param suffixArray The suffix array to be used for lookup.
     * @param text The text to be searched in.
    */

   // Clear the hits vector and check if the query&text are valid
    hits.clear();
    if (query.length() > text.length()) return;

    uint32_t lp = 0;
    uint32_t rp = 0;
    uint32_t L = 0;
    uint32_t R = 0;
    uint32_t M = 0;
    uint32_t mlr = 0;
    uint32_t suffixMax = sa.size()-1;

    // Search for the leftmost index
    if (query <= text.substr(sa[0]))
        lp = 0;

    else if (query > text.substr(sa[suffixMax]))
        lp = suffixMax+1;

    else {
        L = 0;
        R = suffixMax;

        while (R - L > 1) {
            mlr = calculateMLR(text.substr(sa[L]), text.substr(sa[R]));
            M = (L + R)/2;
            if (query.substr(mlr) <= text.substr(sa[M] + mlr))
                R = M;
            else
                L = M;
        }
        lp = R;
    }

    // Search for the rightmost index
    if (query >= text.substr(sa[suffixMax], query.length())) {
        rp = suffixMax+1;
    }

    else if (query < text.substr(sa[0], query.length())) {
        rp = 0;
    }

    else {
        L = lp;
        R = sa.size()-1;

        while (R - L > 1) {
            mlr = calculateMLR(text.substr(sa[L]), text.substr(sa[R]));
            M = (L + R+1)/2;
            if (query.substr(mlr) < text.substr(sa[M] + mlr, query.length()))
                R = M;
            else
                L = M;
        }
        rp = L;
    }

    // Add all hits to the hits vector
    if (!((rp == lp && lp < text.size() && query != text.substr(sa[lp], query.length())) || query.empty() || text.empty())) {
        for (uint32_t i = lp; i <= rp; i++) {
            if (i < sa.size())
                hits.push_back(sa[i]);
        }
    }
    std::sort(hits.begin(), hits.end());
}