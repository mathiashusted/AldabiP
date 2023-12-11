#include "BLAST_Neighborhood.hpp"
#include <omp.h>
#include <algorithm>


void BLAST_Neighborhood::generateWords(const std::string& query, int word_size, std::vector<std::string>& output) {
    output.resize(query.length() - word_size + 1); // Resize vector in advance to avoid dynamic allocations in each iteration

    for (size_t i = 0; i < query.length() - word_size + 1; i++) {
        output[i] = query.substr(i, word_size);
    }
}

void BLAST_Neighborhood::generateAllKMers(int k, std::string current, std::vector<std::string>& output) {
    if (k == 0) {
        output.push_back(current);
        return;
    }
    for (auto& acid : amino_acids) generateAllKMers(k-1, current + acid, output);
}


std::vector<NHResult> BLAST_Neighborhood::generateNeighborhood(const std::string& query,
                                                            const ScoreMatrix& matrix, const int word_size,
                                                            const int score_threshold, const int threads) {
    if (query.size() < static_cast<unsigned>(word_size)) return {};
    if (word_size < 1) throw std::runtime_error("Wortgröße muss >= 1 sein!\n");
    if (threads < 1) throw std::runtime_error("Thread-Anzahl muss mindestens 1 betragen!\n");
    std::vector<std::string> words;
    
    // 1. Generate k-mers
    generateWords(query, word_size, words);
    // 2. Expand neighborhood
    std::vector<NHResult> output;
    std::vector<std::string> kMers {};
    generateAllKMers(word_size, "", kMers);

    for (const auto& word : words) {
        std::string infix = word;
        std::vector<std::pair <std::string, int>> neighbors {};
        for (const auto& kMer : kMers) {
            int score = 0;
            for (int x = 0; x < word_size; x++) {
                score += matrix.score(word[x], kMer[x]);
            }
            if (score >= score_threshold) {
                neighbors.push_back(std::make_pair(kMer, score));
            }
        }
        std::sort(neighbors.begin(), neighbors.end());
        output.push_back({infix, neighbors});
    }
    return output;
}