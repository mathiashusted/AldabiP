/**
 * Praktikum Algorithmen und Datenstrukturen
 * WS 2023/2024
 *
 * Aufgabe 3
 * Alignment von Sequenzen
 *
 * Authors: Dzmitry Hramyka, Mathias Husted
 */

#include "Alignment.hpp"

/**
 * @brief Construct a new Alignment:: Alignment object
 *
 * @param seq_a The first sequence
 * @param seq_b The second sequence
 */
Alignment::Alignment(const std::string &seq_a, const std::string &seq_b)
    : seq_first(seq_a), seq_second(seq_b), score(0) {}

/**
 * @brief Compute the alignment
 * 
 * This function computes the alignment of the two sequences
 * using the Needleman-Wunsch algorithm. It works by calling
 * the private functions initializeMatrix(), fillMatrix()
 * and constructAlignment().
 * 
 * @param match The score for a match
 * @param mismatch The score for a mismatch
 * @param gap The score for a gap
 * @param local_align Whether to use local alignment
 */
void Alignment::compute(const int match, const int mismatch, const int gap, bool local_align) {
    this->match_cost = match;
    this->mismatch_cost = mismatch;
    this->gap_cost = gap;
    this->local_align = local_align;
    
    initializeMatrix();
    doRecursion();
    constructAlignment();
}

// ?? Should we throw an exception if the alignment has not been computed?

/**
 * @brief Get the score of the alignment
 * 
 * This function returns the score of the alignment.
 * 
 * @return int The score of the alignment
 */
int Alignment::getScore() const {
    if (!this->matrix.size()) {
        throw std::runtime_error("Alignment has not been computed");
    }
    return this->score;
}

/**
 * @brief Get the alignment
 * 
 * This function returns the alignment as three strings.
 * 
 * @param a1 The first sequence
 * @param gaps The gaps
 * @param a2 The second sequence
 */
void Alignment::getAlignment(std::string &a1, std::string &gaps, std::string &a2) const {
    if (!this->matrix.size()) {
        throw std::runtime_error("Alignment has not been computed");
    }
    a1 = this->aligned_seq_first;
    gaps = this->aligned_gaps;
    a2 = this->aligned_seq_second;
}

/**
 * @brief Initialize the matrix
 * 
 * This function initializes the matrix with the
 * correct values.
 */
void Alignment::initializeMatrix() {
    // Clear the matrix and traceback
    this->matrix.clear();
    this->traceback.clear();

    // Initialize the matrix and traceback
    this->matrix.resize(this->seq_first.size() + 1, std::vector<int>(this->seq_second.size() + 1));
    this->traceback.resize(this->seq_first.size() + 1, std::vector<int>(this->seq_second.size() + 1, NONE));

    // Initialize the first row and column
    if (this->local_align) {
        for (size_t i = 0; i <= this->seq_first.size(); ++i) {
            this->matrix[i][0] = 0;
            this->traceback[i][0] = NONE;
        }
        for (size_t j = 0; j <= this->seq_second.size(); ++j) {
            this->matrix[0][j] = 0;
            this->traceback[0][j] = NONE;
        }
    } else {
        for (size_t i = 0; i <= this->seq_first.size(); ++i) {
            this->matrix[i][0] = i * this->gap_cost;
            this->traceback[i][0] = UP;
        }
        for (size_t j = 0; j <= this->seq_second.size(); ++j) {
            this->matrix[0][j] = j * this->gap_cost;
            this->traceback[0][j] = LEFT;
        }
    }
}

/**
 * @brief Fill the matrix
 * 
 * This function fills the matrix with the correct
 * values.
 */
void Alignment::doRecursion() {
    if (this->local_align) {
        int maxScore = 0;
        size_t maxI = 0;
        size_t maxJ = 0;
        for (size_t i = 1; i <= this->seq_first.size(); ++i) {
            for (size_t j = 1; j <= this->seq_second.size(); ++j) {
                int diagonalScore = this->matrix[i - 1][j - 1] + 
                        (this->seq_first[i - 1] == this->seq_second[j - 1] 
                            ? this->match_cost 
                            : this->mismatch_cost);
                int verticalScore = this->matrix[i - 1][j] + this->gap_cost;
                int horizontalScore = this->matrix[i][j - 1] + this->gap_cost;

                TracebackDirection trace = getMaxDirection(diagonalScore, verticalScore, horizontalScore);
                int maxScore = std::max({diagonalScore, verticalScore, horizontalScore, 0});
                this->matrix[i][j] = maxScore;
                this->traceback[i][j] = (maxScore == 0) ? NONE : trace;

                if (this->matrix[i][j] > maxScore) {
                    maxScore = this->matrix[i][j];
                    maxI = i;
                    maxJ = j;
                }
            }
        }
        this->score = maxScore;
        this->maxI = maxI;
        this->maxJ = maxJ;
    } else {
        for (size_t i = 1; i <= this->seq_first.size(); ++i) {
            for (size_t j = 1; j <= this->seq_second.size(); ++j) {
                int diagonalScore = this->matrix[i - 1][j - 1] + 
                        (this->seq_first[i - 1] == this->seq_second[j - 1] 
                            ? this->match_cost 
                            : this->mismatch_cost);
                int verticalScore = this->matrix[i - 1][j] + this->gap_cost;
                int horizontalScore = this->matrix[i][j - 1] + this->gap_cost;

                TracebackDirection trace = getMaxDirection(diagonalScore, verticalScore, horizontalScore);
                this->matrix[i][j] = std::max({diagonalScore, verticalScore, horizontalScore}); // ?? Should we add 0?
                this->traceback[i][j] = trace;
            }
        }
        this->score = this->matrix[this->seq_first.size()][this->seq_second.size()];
    }
}

/**
 * @brief Construct the alignment
 * 
 * This function constructs the alignment from the
 * traceback matrix.
 */
void Alignment::constructAlignment() {
    size_t i = this->seq_first.size();
    size_t j = this->seq_second.size();
    // Clear the aligned sequences and gaps
    this->aligned_seq_first = "";
    this->aligned_seq_second = "";
    this->aligned_gaps = "";

    // Recursively construct the alignment
    while (i > 0 || j > 0) {
        switch (this->traceback[i][j]) {
            case DIAGONAL:
                this->aligned_seq_first = this->seq_first[--i] + this->aligned_seq_first;
                this->aligned_seq_second = this->seq_second[--j] + this->aligned_seq_second;
                this->aligned_gaps = (this->seq_first[i] == this->seq_second[j] ? '|' : ' ') + this->aligned_gaps;
                break;
            case UP:
                this->aligned_seq_first = this->seq_first[--i] + this->aligned_seq_first;
                this->aligned_seq_second = '-' + this->aligned_seq_second;
                this->aligned_gaps = ' ' + this->aligned_gaps;
                break;
            case LEFT:
                this->aligned_seq_first = '-' + this->aligned_seq_first;
                this->aligned_seq_second = this->seq_second[--j] + this->aligned_seq_second;
                this->aligned_gaps = ' ' + this->aligned_gaps;
                break;
            default:
                break;
        }
    }
}

/**
 * @brief Get the maximum direction
 * 
 * This function returns the maximum direction
 * from the three given values.
 * 
 * @param matchScore The score for a match
 * @param verticalScore The score for a deletion
 * @param horizontalScore The score for an insertion
 * @return TracebackDirection The maximum direction
 */
Alignment::TracebackDirection Alignment::getMaxDirection(int matchScore, int verticalScore, int horizontalScore) const {
    if (matchScore >= verticalScore && matchScore >= horizontalScore) {
        return DIAGONAL;
    } else if (verticalScore >= matchScore && verticalScore >= horizontalScore) {
        return UP;
    } else {
        return LEFT;
    }
}