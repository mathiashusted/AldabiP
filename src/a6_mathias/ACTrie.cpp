#include "ACTrie.hpp"

#include <stdexcept>


ACTrie::ACTrie(const std::vector<std::string>& needles) {
    if (needles.empty()) throw std::logic_error("Input vector is empty!\n");

    // Start at root
    ACNode* iterator = &this->root;
    bool has_reached_end = false;

    while (!has_reached_end) {
        for (const auto& child : iterator->children) {
            for (const auto& letter : needles) {
            }
        }
    }

}