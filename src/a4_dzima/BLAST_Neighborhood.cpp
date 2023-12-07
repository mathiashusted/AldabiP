#include "BLAST_Neighborhood.hpp"
#include "a4_util.h"  // include the utility header

// Helper function to recursively generate words
void generateWordsHelper(std::vector<std::string>& words, std::string currentWord, int wordSize) {
    static const char aminoAcids[] = {'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y'};

    if (currentWord.size() == wordSize) {
        words.push_back(currentWord);
        return;
    }

    for (char aminoAcid : aminoAcids) {
        generateWordsHelper(words, currentWord + aminoAcid, wordSize);
    }
}

// Main function to generate all words
std::vector<std::string> generateAllWords(int wordSize) {
    std::vector<std::string> words;
    generateWordsHelper(words, "", wordSize);
    return words;
}

std::vector<NHResult> BLAST_Neighborhood::generateNeighborhood(const std::string& query,
                                                               const ScoreMatrix& matrix,
                                                               const int word_size,
                                                               const int score_threshold,
                                                               const int threads) {
    if (threads != 1) {
        throw std::runtime_error("Multithreading not implemented, threads must be 1");
    }

    std::vector<NHResult> neighborhoodResults;
    // Iterate through each infix of the query
    for (size_t i = 0; i <= query.size() - word_size; ++i) {
        std::string infix = query.substr(i, word_size);
        std::vector<std::pair<std::string, int>> neighbors;

        // Generate all possible words and calculate scores
        for (const std::string& word : generateAllWords(word_size)) {
            int score = 0;
            for (size_t j = 0; j < word_size; ++j) {
                score += matrix.score(infix[j], word[j]);
            }
            if (score >= score_threshold) {
                neighbors.emplace_back(word, score);
            }
        }

        // Sort neighbors alphabetically
        std::sort(neighbors.begin(), neighbors.end());
        neighborhoodResults.push_back({infix, neighbors});
    }

    return neighborhoodResults;
}
