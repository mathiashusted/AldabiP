#include "QGramIndex.hpp"
#include <iostream> // DEBUG
#include "a5_util.hpp"

#include <cmath>


uint32_t QGramIndex::hash(const std::string& qgram) const {
    uint32_t hash_value = 0;

    for (const auto& i : qgram) {
        hash_value <<= bit_shift_value; // Equivalent to hash_value *= alphabet_length (optimized by the compiler either way, but oh well...)
        hash_value |= ordValue(i); // Equivalent to hash_value += ordValue(i)
    }

    return hash_value;
}

uint32_t QGramIndex::hashNext(const uint32_t prev_hash, const char new_pos) const {
    uint32_t hash_value = prev_hash;
    hash_value <<= bit_shift_value;
    hash_value |= ordValue(new_pos);

    // Last step: Delete the characters of the overlapping q-gram
    uint32_t mask = ~0 << 2*q_length;
    // Now we'll "cut" the characters from the previous q-gram out
    return hash_value & ~mask;
}

QGramIndex::QGramIndex(const std::string& text, const uint8_t q) : pattern(text), q_length(q) {
    pattern_length = pattern.length();

    // Initialize the q-gram
    // Start by sorting using counting sort
    // 1. Initializiation phase
    for (uint16_t i = 0; i < std::pow(alphabet_length, q_length); i++)
        dir.push_back(0);

    // 2. Fetch occurences
    for (size_t i = 0; i < pattern_length - q_length; i++) {
        // j = hash of the q-gram at index i
        uint32_t j = hash(pattern.substr(i, i+q_length-1));
    }
}

uint8_t QGramIndex::getQ() const {
    return q_length;
}

const std::string& QGramIndex::getText() const {
    return pattern;
}