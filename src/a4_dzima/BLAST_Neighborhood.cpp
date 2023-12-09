#include "BLAST_Neighborhood.hpp"
#include "a4_util.h"
#include <omp.h>


void BLAST_Neighborhood::generateWordsRecursively(
    std::vector<std::string> &words, std::string currentWord, int wordSize) {
  // Recursion anchor
  if (static_cast<int>(currentWord.size()) == wordSize) {
    words.push_back(currentWord);
    return;
  }

  // Recursively generate words
  for (char aminoAcid : this->aminoAcids) {
    generateWordsRecursively(words, currentWord + aminoAcid, wordSize);
  }
}

std::vector<std::string> BLAST_Neighborhood::generateAllWords(int wordSize) {
  std::vector<std::string> words;
  generateWordsRecursively(words, "", wordSize);
  return words;
}

std::vector<NHResult> BLAST_Neighborhood::generateNeighborhood(
    const std::string &query, const ScoreMatrix &matrix, const int word_size,
    const int score_threshold, const int threads) {
  if (threads != 1) {
    throw std::runtime_error(
        "Multithreading not implemented, threads must be 1");
  }

  std::vector<NHResult> neighborhoodResults;
  //#pragma omp parallel for
  for (size_t i = 0; i <= query.size() - word_size; ++i) {
    std::string infix = query.substr(i, word_size); // Get infix
    std::vector<std::pair<std::string, int>> neighbors;

    // Generate all possible words and calculate scores
    for (const std::string &word : generateAllWords(word_size)) {
      int score = 0;
      for (size_t j = 0; static_cast<int>(j) < word_size; ++j) {
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
