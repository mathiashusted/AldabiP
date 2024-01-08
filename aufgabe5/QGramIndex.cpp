#include "QGramIndex.hpp"
#include <iostream>
#include "a5_util.hpp"
// #include <bitset> // DEBUG
#include <cmath>


uint32_t QGramIndex::hash(const std::string& qgram) const {
    if (qgram.size() != q_length) throw std::invalid_argument("Invalid q-gram. Wrong length!");
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
    // Now we'll "cut" the characters from the previous q-gram out
    return hash_value & ~mask;
}

QGramIndex::QGramIndex(const std::string& text, const uint8_t q) :  q_length(q), search_text(text) {
    if (q_length < 1 || q_length > 13) throw std::invalid_argument("Invalid q!");

    search_text_length = search_text.length();
    mask = ~0 << 2*q_length; // Our bit mask for our hash function

    // Initialize the q-gram
    // Start by sorting using counting sort
    // 1. Initializiation phase
    for (size_t i = 0; i < std::pow(alphabet_length, q_length); i++) {
        dir.push_back(0);
    }

    // 2. Fetch occurences
    // We need to add the first occurence manually in order to use the rolling hash function
    uint32_t previous_hash = hash(search_text.substr(0, q_length));
    dir[previous_hash]++;
    for (size_t i = 1; i < search_text_length - q_length+1; i++) {
        uint32_t j = hashNext(previous_hash, search_text[i+q_length-1]);
        dir[j]++;
        previous_hash = j;
    }

    // 3. Create cumulative sum
    for (size_t i = 1; i < dir.size(); i++) {
        dir[i] += dir[i-1];
    }

    // 3.5 Initialize suffix array
    // The cumulated sum from dir (last position in dir) is the size of our suffix array
    for (size_t i = 0; i < dir[dir.size()-1]; i++) {
        suffix_array.push_back(0);
    }

    // 4. Use dir to create our suffix array
    // Here we need to use our hash function in the same manner as in step 2
    previous_hash = hash(search_text.substr(0, q_length));
    dir[previous_hash]--;
    suffix_array[dir[previous_hash]] = 0;
    for (size_t i = 1; i < search_text_length - q_length+1; i++) {
        uint32_t j = hashNext(previous_hash, search_text[i+q_length-1]);
        dir[j]--;
        suffix_array[dir[j]] = i;
        previous_hash = j;
    }
}

std::vector<uint32_t> QGramIndex::getHits(const uint32_t h) const {
    if (h > std::pow(alphabet_length, q_length) - 1) throw(std::invalid_argument("Invalid hash!"));

    std::vector<uint32_t> output{};
    uint32_t num_occurences;

    // Check if we are scanning the last q-gram (TTT...T)
    // ~mask is equivalent to the hash of the last q-gram
    if (h == ~mask) num_occurences = search_text_length - dir[h] - q_length + 1;
    else num_occurences = dir[h+1] - dir[h];

    for (uint32_t i = 0; i < num_occurences; i++)
        output.push_back(suffix_array[dir[h]+i]);

    return output;
}

uint8_t QGramIndex::getQ() const {
    return q_length;
}

const std::string& QGramIndex::getText() const {
    return search_text;
}