#include "Alignment.hpp"
#include <iostream> // Debug


Alignment::Alignment(const std::string& seq_v, const std::string& seq_h) {
    this->seqv = seq_v;
    this->seqh = seq_h;
}

void Alignment::compute(const int match, const int mismatch, const int gap, const bool local_align) {
    int d = gap; // Gap cost
    long long maxScore = 0;
    f.resize(seqh.size(), std::vector<long long>(seqv.size()));
    t.resize(seqh.size(), std::vector<char>(seqv.size()));


    f[0][0] = 0;
    
    for (uint32_t i = 1; i < seqh.size(); i++) {
        f[i][0] = -i * d;
        t[i][0] = 0; // <-
    }
    for (uint32_t j = 1; j < seqv.size(); j++) {
        f[0][j] = -j * d;
        t[0][j] = 2; // |
    }


    for (uint32_t i = 1; i < seqh.size(); i++) {
        for (uint32_t j = 1; j < seqh.size(); j++) {
            // Get max score. Start by initializing to the diagonal, then correct if necessary.
            maxScore = f[i-1][j-1] + match; // F(i-1,j-1) + s(xi, xj) CORRECT TO INCLUDE MISMATCHES!!!
            t[i][j] = 1;

            if ((f[i-1][j] - d) > maxScore) {
                maxScore = f[i-1][j] - d;
                t[i][j] = 0;
            }
            if ((f[i][j-1] - d) > maxScore) {
                maxScore = f[i][j-1] - d;
                t[i][j] = 2;
            }
            std::cout << "[" << maxScore << "] ";
        }
        std::cout << "\n";
    }
}