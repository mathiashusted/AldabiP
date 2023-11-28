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
    if (local_align) smithWaterman = true;

    f.clear();
    t.clear();

    uint32_t width = seqh.size()+1;
    uint32_t height = seqv.size()+1;
    long maxScore = 0;
    int matchScore = 0;


    f.resize(width, std::vector<long>(height));
    t.resize(width, std::vector<Traceback>(height));
    //std::cout << "Matrix Borders:\n " << seqh << "\n"; // DEBUG
    //for (uint32_t i = 0; i < seqv.size(); i++) std::cout << seqv[i] << "\n"; // DEBUG

    // Initialization
    f[0][0] = 0;
    t[0][0] = Traceback::NONE;

    for (uint32_t i = 0; i < width; i++) {
        if (smithWaterman) {
            f[i][0] = 0;
            t[i][0] = Traceback::NONE;
        }
        else {
            f[i][0] = static_cast<long>(i) * gap;
            t[i][0] = Traceback::HORIZONTAL;
        }
    }
    for (uint32_t j = 0; j < height; j++) {
        if (smithWaterman) {
            f[0][j] = 0;
            t[0][j] = Traceback::NONE;
        }
        else {
            f[0][j] = static_cast<long>(j) * gap;
            t[0][j] = Traceback::VERTICAL;
        }
    }

    // Recurrence

    if (local_align) {
        long globalMaxScore = 0;

        for (uint32_t i = 1; i < width; i++) {
            for (uint32_t j = 1; j < height; j++) {
                matchScore = (seqh[i-1] == seqv[j-1]) ? match : mismatch;

                long scoreDiagonal = f[i-1][j-1] + matchScore;
                long scoreUp = f[i-1][j] + gap;
                long scoreLeft = f[i][j-1] + gap;
                long scoreZero = 0;

                maxScore = std::max({scoreDiagonal, scoreUp, scoreLeft, scoreZero});
                f[i][j] = maxScore;

                if (f[i-1][j] + gap > maxScore) {
                    maxScore = f[i-1][j] + gap;
                    t[i][j] = Traceback::HORIZONTAL;
                }
                if (f[i-1][j-1] + matchScore > maxScore) {
                    maxScore = f[i-1][j-1] + matchScore;
                    t[i][j] = Traceback::DIAGONAL;
                }

                // Update traceback
                if (maxScore == scoreDiagonal) {
                    t[i][j] = Traceback::DIAGONAL;
                } else if (maxScore == scoreUp) {
                    t[i][j] = Traceback::HORIZONTAL;
                } else if (maxScore == scoreLeft) {
                    t[i][j] = Traceback::VERTICAL;
                } else {
                    t[i][j] = Traceback::NONE;
                }

                // Update global max score
                if (maxScore > globalMaxScore) {
                    globalMaxScore = maxScore;
                    localStartI = i;
                    localStartJ = j;
                }
            }
        }

        score = globalMaxScore;
    } else {
        for (uint32_t i = 1; i < width; i++) {
            for (uint32_t j = 1; j < height; j++) {
                matchScore = (seqh[i-1] == seqv[j-1]) ? match : mismatch;

                maxScore = f[i][j-1] + gap;
                t[i][j] = Traceback::VERTICAL;


                if (f[i-1][j] + gap > maxScore) {
                    maxScore = f[i-1][j] + gap;
                    t[i][j] = Traceback::HORIZONTAL;
                }
                if (f[i-1][j-1] + matchScore >= maxScore) {
                    maxScore = f[i-1][j-1] + matchScore;
                    t[i][j] = Traceback::DIAGONAL;
                }

                // Smith-Waterman
                if (smithWaterman) {
                    if (0 >= maxScore) {
                        maxScore = 0;
                        t[i][j] = Traceback::NONE;
                    }
                }

                f[i][j] = maxScore;

            }
        }

        score = f[width-1][height-1];
    }

    //Debug: Print matrix
    for (uint32_t j = 0; j < height; j++) {
        for (uint32_t i = 0; i < width; i++) {
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

    a1 = "";
    a2 = "";
    gaps = "";
    uint32_t i = seqh.size();
    uint32_t j = seqv.size();
    if (smithWaterman) {
        i = localStartI;
        j = localStartJ;
    }

    while (!(i == 0 && j == 0) && (t[i][j] != Traceback::NONE)) {
        if (t[i][j] == Traceback::DIAGONAL) {
            a1 += seqv[j-1];
            a2 += seqh[i-1];
            gaps += (seqv[j-1] == seqh[i-1]) ? "|" : " ";
            i--; j--;
        }
        else if (t[i][j] == Traceback::HORIZONTAL) {
            a1 += "-";
            a2 += seqh[i-1];
            gaps += " ";
            i--;
        }
        else {
            a1 += seqv[j-1];
            a2 += "-";
            gaps += " ";
            j--;
        }
    }
    // Traceback
    // while (i > 0 && j > 0) {
    //     if (smithWaterman && f[i][j] == 0) break; // Termination condition for Smith-Waterman

    //     if (t[i][j] == Traceback::DIAGONAL) {
    //         a1 += seqv[j-1];
    //         a2 += seqh[i-1];
    //         gaps += (seqv[j-1] == seqh[i-1]) ? "|" : " ";
    //         i--; j--;
    //     } else if (t[i][j] == Traceback::HORIZONTAL) {
    //         a1 += "-";
    //         a2 += seqh[i-1];
    //         gaps += " ";
    //         i--;
    //     } else if (t[i][j] == Traceback::VERTICAL) {
    //         a1 += seqv[j-1];
    //         a2 += "-";
    //         gaps += " ";
    //         j--;
    //     }
    // }

    std::reverse(a1.begin(), a1.end());
    std::reverse(gaps.begin(), gaps.end());
    std::reverse(a2.begin(), a2.end());

    std::cout << a1 << "\n" << gaps << "\n" << a2 << "\n";
}