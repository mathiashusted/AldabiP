#include "aufgabe2.hpp"

#include <cstdint>
#include <algorithm>
#include <iostream> //DEBUG

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
    hits.clear();
    if (query.length() > text.length()) return;

    uint32_t lp = 0;
    uint32_t rp = 0;
    uint32_t L = 0;
    uint32_t R = 0;
    uint32_t M = 0;
    uint32_t mlr = 0;
    uint32_t suffixMax = sa.size()-1;

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
            //std::cout << "(L,R) = (" << L << "," << R << ") => M = " << M << "\n";
            std::cout << "mlr = " << mlr << "\n";
            // if (query.substr(mlr) <= text.substr(sa[M + mlr]))
            if (query <= text.substr(sa[M]))
                R = M;
            else
                L = M;
        }
        lp = R;
    }
    // std::cout << "Final value: (L,R) = (" << L << "," << R << ")" << "\n";
    // std::cout << "\n\nLp = " << lp << "\n";

    if (query >= text.substr(sa[suffixMax], query.length())) {
        //std::cout << "query >= text.sub...\n";
        rp = suffixMax+1;
    }

    else if (query < text.substr(sa[0], query.length())) {
        //std::cout << "query >= text.sub...\n";
        rp = 0;
    }

    else {
        L = lp;
        R = sa.size()-1;

        while (R - L > 1) {
            M = (L + R+1)/2;
            //std::cout << "(L,R) = (" << L << "," << R << ") => M = " << M << "\n";
            if (query < text.substr(sa[M], query.length()))
                R = M;
            else
                L = M;
        }
        rp = L;
    }
    // std::cout << "Final value: (L,R) = (" << L << "," << R << ")" << "\n";
    // std::cout << "Rp = " << rp << "\n";

    hits.clear();
    if (!((rp == lp && lp < text.size() && query != text.substr(sa[lp], query.length())) || query.empty() || text.empty())) {
        for (uint32_t i = lp; i <= rp; i++) {
            if (i < sa.size())
                hits.push_back(sa[i]);
        }
    }
    std::sort(hits.begin(), hits.end());
}