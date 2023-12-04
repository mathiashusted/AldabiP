#include "Alignment.hpp"


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
    int maxScore = 0;
    int matchScore = 0;


    f.resize(width, std::vector<int>(height));
    t.resize(width, std::vector<Traceback>(height));

    // Initialization

    f[0][0] = 0;
    t[0][0] = Traceback::NONE;

    for (uint32_t i = 0; i < width; i++) {
        if (smithWaterman) {
            f[i][0] = 0;
            t[i][0] = Traceback::NONE;
        }
        else {
            f[i][0] = static_cast<int>(i) * gap;
            t[i][0] = Traceback::HORIZONTAL;
        }
    }
    for (uint32_t j = 0; j < height; j++) {
        if (smithWaterman) {
            f[0][j] = 0;
            t[0][j] = Traceback::NONE;
        }
        else {
            f[0][j] = static_cast<int>(j) * gap;
            t[0][j] = Traceback::VERTICAL;
        }
    }

    // Recurrence

    if (smithWaterman) {
        int globalMaxScore = 0;

        for (uint32_t i = 1; i < width; i++) {
            for (uint32_t j = 1; j < height; j++) {
                matchScore = (seqh[i-1] == seqv[j-1]) ? match : mismatch;

                int scoreDiagonal = f[i-1][j-1] + matchScore;
                int scoreUp = f[i-1][j] + gap;
                int scoreLeft = f[i][j-1] + gap;

                maxScore = std::max({0, scoreDiagonal, scoreUp, scoreLeft});
                f[i][j] = maxScore;

                // Update traceback
                if (maxScore == scoreLeft) {
                    t[i][j] = Traceback::VERTICAL;
                } else if (maxScore == scoreUp) {
                    t[i][j] = Traceback::HORIZONTAL;
                } else if (maxScore == scoreDiagonal) {
                    t[i][j] = Traceback::DIAGONAL;
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

                f[i][j] = maxScore;

            }
        }
        score = f[width-1][height-1];
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
        else if (t[i][j] == Traceback::VERTICAL) {
            a1 += seqv[j-1];
            a2 += "-";
            gaps += " ";
            j--;
        }
        else  {
            a1 += "-";
            a2 += seqh[i-1];
            gaps += " ";
            i--;
        }
    }

    std::reverse(a1.begin(), a1.end());
    std::reverse(gaps.begin(), gaps.end());
    std::reverse(a2.begin(), a2.end());

    std::cout << a1 << "\n" << gaps << "\n" << a2 << "\n";
}