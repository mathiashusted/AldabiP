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
    long maxFunction = 0;
    int matchFunction = 0;


    f.resize(seqh.size(), std::vector<long>(seqv.size()));
    t.resize(seqh.size(), std::vector<Traceback>(seqv.size()));
    std::cout << "Matrix Borders:\n " << seqh << "\n"; // DEBUG
    for (uint32_t i = 0; i < seqv.size(); i++) std::cout << seqv[i] << "\n"; // DEBUG

    // Initialization
    f[1][1] = 0;
    t[1][1] = Traceback::NONE;
    
    for (uint32_t i = 1; i < seqh.size(); i++) {
        f[i][0] = 0;
        t[i][0] = Traceback::NONE;
        f[i][1] = -static_cast<long>(i) * d;
        t[i][1] = Traceback::HORIZONTAL;
    }
    for (uint32_t j = 1; j < seqv.size(); j++) {
        // Create dummy row first, then the initialization row
        f[0][j] = 0;
        t[0][j] = Traceback::NONE;
        f[1][j] = -static_cast<long>(j) * d;
        t[1][j] = Traceback::VERTICAL;
    }

    // Recurrence
    for (uint32_t i = 2; i < seqh.size(); i++) {
        for (uint32_t j = 2; j < seqv.size(); j++) {
            // Get max score. Start by initializing to the diagonal, then correct if necessary.
            //std::cout << "Comparing " << seqh[i] << " to " << seqv[j] << "\n"; 
            if (seqh[i] == seqv[j])
                matchFunction = match;
            else
                matchFunction = mismatch;
            
            maxFunction = f[i-1][j-1] + matchFunction; // F(i-1,j-1) + s(xi, xj)
            t[i][j] = Traceback::DIAGONAL;

            if ((f[i-1][j] + d) > maxFunction) {
                maxFunction = f[i-1][j] + d;
                t[i][j] = Traceback::HORIZONTAL;
            }
            if ((f[i][j-1] + d) > maxFunction) {
                maxFunction = f[i][j-1] + d;
                t[i][j] = Traceback::VERTICAL;
            }
            f[i][j] = maxFunction;
        }
    }
    score = f[seqh.size()-1][seqv.size()-1];



    // Debug: Print matrix
    for (uint32_t j = 0; j < seqv.size(); j++) {
        for (uint32_t i = 0; i < seqh.size(); i++) {
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
    std::pair<uint32_t, uint32_t> tracer = std::make_pair(seqh.size()-1, seqv.size()-1);

    while (tracer.first >= 0 && tracer.second >= 0) {
        if (t[tracer.first][tracer.second] == Traceback::DIAGONAL) {
            std::cout << "Diagonal: (" << tracer.first << ", " << tracer.second << ")" << "\n";
            a1     += seqh[tracer.first];
            a2     += seqv[tracer.second];
            gaps   += "|"; 
            tracer.first--;
            tracer.second--;
        }
        else if (t[tracer.first][tracer.second] == Traceback::HORIZONTAL) {
            std::cout << "Horizontal: (" << tracer.first << ", " << tracer.second << ")" << "\n";
            a1     += seqh[tracer.first];
            a2     += "-";
            gaps   += " ";
            tracer.first--;
        }
        else if (t[tracer.first][tracer.second] == Traceback::VERTICAL) {
            std::cout << "Vertical: (" << tracer.first << ", " << tracer.second << ")" << "\n";
            a1     += "-";
            a2     += seqv[tracer.second];
            gaps   += " ";
            tracer.second--;
        }
        else if (t[tracer.first][tracer.second] == Traceback::NONE) {
            std::cout << "Reached dead end, exiting...\n";
            break;
        }
    }

    std::reverse(a1.begin(), a1.end());
    std::reverse(gaps.begin(), gaps.end());
    std::reverse(a2.begin(), a2.end());

    std::cout << a1 << "\n" << gaps << "\n" << a2 << "\n";
}