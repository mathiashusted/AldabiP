/**
 * Praktikum Algorithmen und Datenstrukturen
 * WS 2023/2024
 *
 * Aufgabe 3
 * Alignment von Sequenzen
 *
 * Authors: Dzmitry Hramyka, Mathias Husted
 */

#include <iostream>
#include "Alignment.hpp"

int main(int argc, char* argv[]) {
    if (argc != 6 && argc != 7) {
        std::cerr << "Usage: " << argv[0] << " <sequence1> <sequence2> <match_score> <mismatch_penalty> <gap_penalty>\n";
        return 1;
    }

    std::string seq1 = argv[1];
    std::string seq2 = argv[2];
    int match = std::stoi(argv[3]);
    int mismatch = std::stoi(argv[4]);
    int gap = std::stoi(argv[5]);

    // Check if local alignment should be used
    bool local_align;
    if (argc == 7) {
        local_align = true;
    } else {
        local_align = false;
    }

    try {
        Alignment alignment(seq1, seq2);
        alignment.compute(match, mismatch, gap, local_align);

        std::string a1, gaps, a2;
        alignment.getAlignment(a1, gaps, a2);
        int score = alignment.getScore();

        std::cout << a1 << "\n";
        std::cout << gaps << "\n";
        std::cout << a2 << "\n";
        std::cout << "score:" << score << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}