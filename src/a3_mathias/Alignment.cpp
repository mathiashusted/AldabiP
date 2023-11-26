#include "Alignment.hpp"
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <iostream> // Debug
#include <iomanip> // Debug


Alignment::Alignment(const std::string& seq_v, const std::string& seq_h) {
    this->seqv = seq_v;
    this->seqh = seq_h;
}

void Alignment::compute(const int match, const int mismatch, const int gap, const bool local_align) {
    computeCalled = true;
    int d = gap; // Gap cost
    long maxScore = 0;
    int matchScore = 0;


    f.resize(seqh.size()+1, std::vector<long>(seqv.size()));
    t.resize(seqh.size()+1, std::vector<Traceback>(seqv.size()));
    std::cout << "Matrix Borders:\n " << seqh << "\n"; // DEBUG
    for (uint32_t i = 0; i < seqv.size(); i++) std::cout << seqv[i] << "\n"; // DEBUG

    // Initialization
    f[0][0] = 0;
    t[0][0] = Traceback::NONE;
    
    for (uint32_t i = 1; i < seqh.size(); i++) {
        f[i][0] = static_cast<long>(i) * d;
        t[i][0] = Traceback::HORIZONTAL;
    }
    for (uint32_t j = 1; j < seqv.size(); j++) {
        f[0][j] = static_cast<long>(j) * d;
        t[0][j] = Traceback::VERTICAL;

        // Initialize first row (aligned with gap)
        matchScore = mismatch;

        maxScore = f[0][j-1] + matchScore; // F(i-1,j-1) + s(xi, xj)
        t[1][j] = Traceback::DIAGONAL;

        if ((f[0][j] + d) > maxScore) {
            maxScore = f[0][j] + d;
            t[1][j] = Traceback::HORIZONTAL;
        }      

        if ((f[1][j] + d) > maxScore) {
            maxScore = f[1][j-1] + d;
            t[1][j] = Traceback::VERTICAL;
        }
        f[1][j] = maxScore;
    }

    // Recurrence
    for (uint32_t i = 2; i < seqh.size()+1; i++) {
        for (uint32_t j = 1; j < seqv.size(); j++) {
            // Get max score. Start by initializing to the diagonal, then correct if necessary.
            //std::cout << "Comparing " << seqh[i] << " to " << seqv[j] << "\n"; 
            if (seqh[i-1] == seqv[j])
                matchScore = match;
            else
                matchScore = mismatch;
            
            maxScore = f[i-1][j-1] + matchScore; // F(i-1,j-1) + s(xi, xj)
            t[i][j] = Traceback::DIAGONAL;

            if ((f[i-1][j] + d) > maxScore) {
                maxScore = f[i-1][j] + d;
                t[i][j] = Traceback::HORIZONTAL;
            }
            if ((f[i][j-1] + d) > maxScore) {
                maxScore = f[i][j-1] + d;
                t[i][j] = Traceback::VERTICAL;
            }
            f[i][j] = maxScore;
        }
    }
    score = f[seqh.size()][seqv.size()-1];



    // Debug: Print matrix
    for (uint32_t j = 0; j < seqv.size(); j++) {
        for (uint32_t i = 0; i < seqh.size()+1; i++) {
            std::cout << "[";
            if (t[i][j] == Traceback::DIAGONAL) std::cout << "\\";
            else if (t[i][j] == Traceback::HORIZONTAL) std::cout << "<";
            else if (t[i][j] == Traceback::VERTICAL) std::cout << "|";
            else if (t[i][j] == Traceback::NONE) std::cout << "X";
            std::cout << "" << std::setw(4) << f[i][j] << "] ";
        }
        std::cout << "\n";
    }
}

int Alignment::getScore() const {
    if (!computeCalled) throw(std::runtime_error("Compute hasn't been called!"));
    return score;
}


void Alignment::getAlignment(std::string& a1, std::string& gaps, std::string& a2) const {
    if (!computeCalled) throw(std::runtime_error("Compute hasn't been called!"));

    uint32_t t1 = seqh.size();
    uint32_t t2 = seqv.size()-1;

    while (!(t1 == 0 && t2 == 0)) {
        if (t[t1][t2] == Traceback::DIAGONAL) {
            a1 += seqv[t2];
            gaps += "|";
            a2 += seqh[t1-1];
            t1--; t2--;
        }
        else if (t[t1][t2] == Traceback::HORIZONTAL) {
            a1 += "-";
            gaps += " ";
            a2 += seqh[t1-1];
            t1--;
        }
        else if (t[t1][t2] == Traceback::VERTICAL) {
            a1 += seqv[t2];
            gaps += " ";
            a2 += "-";
            t2--;
        }

    }

    std::reverse(a1.begin(), a1.end());
    std::reverse(gaps.begin(), gaps.end());
    std::reverse(a2.begin(), a2.end());

    std::cout << a1 << "\n" << gaps << "\n" << a2 << "\n";
}