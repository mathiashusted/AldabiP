#include "Alignment.hpp"

Alignment::Alignment(const std::string& seq_v, const std::string& seq_h)
    : seq_v_(seq_v), seq_h_(seq_h), score_(0), match_(0), mismatch_(0), gap_(0), local_align_(false) {
}

void Alignment::compute(const int match, const int mismatch, const int gap, const bool local_align) {
    if (local_align) {
        throw std::runtime_error("Local alignment not supported");
    }

    match_ = match;
    mismatch_ = mismatch;
    gap_ = gap;
    local_align_ = local_align;

    initializeMatrix();
    fillMatrix();
    constructAlignment();
}

int Alignment::getScore() const {
    if (!matrix_.size()) {
        throw std::runtime_error("Alignment has not been computed");
    }
    return score_;
}

void Alignment::getAlignment(std::string& a1, std::string& gaps, std::string& a2) const {
    if (!matrix_.size()) {
        throw std::runtime_error("Alignment has not been computed");
    }

    a1 = a1_;
    gaps = gaps_;
    a2 = a2_;
}

void Alignment::initializeMatrix() {
    matrix_.resize(seq_v_.size() + 1, std::vector<int>(seq_h_.size() + 1));
    traceback_.resize(seq_v_.size() + 1, std::vector<int>(seq_h_.size() + 1, NONE));

    for (size_t i = 0; i <= seq_v_.size(); ++i) {
        matrix_[i][0] = i * gap_;
        traceback_[i][0] = UP;
    }
    for (size_t j = 0; j <= seq_h_.size(); ++j) {
        matrix_[0][j] = j * gap_;
        traceback_[0][j] = LEFT;
    }
}

void Alignment::fillMatrix() {
    for (size_t i = 1; i <= seq_v_.size(); ++i) {
        for (size_t j = 1; j <= seq_h_.size(); ++j) {
            int matchScore = matrix_[i - 1][j - 1] + (seq_v_[i - 1] == seq_h_[j - 1] ? match_ : mismatch_);
            int deleteScore = matrix_[i - 1][j] + gap_;
            int insertScore = matrix_[i][j - 1] + gap_;

            TracebackDirection dir = getMaxDirection(matchScore, deleteScore, insertScore);
            matrix_[i][j] = std::max({matchScore, deleteScore, insertScore, 0});
            traceback_[i][j] = dir;
        }
    }

    score_ = matrix_[seq_v_.size()][seq_h_.size()];
}

void Alignment::constructAlignment() {
    size_t i = seq_v_.size();
    size_t j = seq_h_.size();
    a1_ = "";
    a2_ = "";
    gaps_ = "";

    while (i > 0 || j > 0) {
        switch (traceback_[i][j]) {
            case DIAGONAL:
                a1_ = seq_v_[--i] + a1_;
                a2_ = seq_h_[--j] + a2_;
                gaps_ = (seq_v_[i] == seq_h_[j] ? '|' : ' ') + gaps_;
                break;
            case UP:
                a1_ = seq_v_[--i] + a1_;
                a2_ = '-' + a2_;
                gaps_ = ' ' + gaps_;
                break;
            case LEFT:
                a1_ = '-' + a1_;
                a2_ = seq_h_[--j] + a2_;
                gaps_ = ' ' + gaps_;
                break;
            default:
                throw std::runtime_error("Invalid traceback state");
        }
    }
}

Alignment::TracebackDirection Alignment::getMaxDirection(int matchScore, int deleteScore, int insertScore) const {
    if (matchScore >= deleteScore && matchScore >= insertScore) return DIAGONAL;
    if (deleteScore >= insertScore) return UP;
    return LEFT;
}
