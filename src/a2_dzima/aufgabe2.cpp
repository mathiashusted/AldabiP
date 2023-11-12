#include "aufgabe2.hpp"

#include <iostream>
#include <cstdint>
#include <algorithm>
#include <numeric>  // For std::iota


void construct(std::vector<uint32_t>& suffixArray, const std::string& text) {
    /**
     * @brief Construct the suffix array for a given text.
     *      The function works in 2 steps:
     *         1. Initialize the suffix array with positional index of the suffix for 
     *            each of the suffices of the text.
     *         2. Sort the suffix array based on the lexicographical order of the suffixes they 
     *            represent.
     * 
     * @param suffixArray The suffix array to be constructed.
     * @param text The text to construct the suffix array for.
    */
    suffixArray.resize(text.length());
    std::iota(suffixArray.begin(), suffixArray.end(), 0);

    // Define the lambda function for suffix comparison
    auto compareSuffixes = [&text](const uint32_t & suffixStartIndex1, const uint32_t & suffixStartIndex2) {
        return std::lexicographical_compare(
            text.begin() + suffixStartIndex1, text.end(),
            text.begin() + suffixStartIndex2, text.end());
    };

    // Sort the suffix array based on the lexicographical order of the suffixes they represent
    std::sort(suffixArray.begin(), suffixArray.end(), compareSuffixes);
}

void find(const std::string& query, const std::vector<uint32_t>& suffixArray, const std::string& text, std::vector<uint32_t>& hits) {
    // Clear the hits vector
    hits.clear();

    // Initialize the left and right border of the hits
    uint32_t startBorder;
    uint32_t endBorder;

    // If the query is empty, return an empty hits vector
    if (query.empty()) {
        return;
    }

    // MLR: Binary search for the start border of the hits
    uint32_t leftBorder = 0;
    uint32_t rightBorder = suffixArray.size() - 1;
    uint32_t middlePosition = 0;
    if (query < text.substr(suffixArray[0])) {
        // The query is smaller than the smallest suffix in the suffix array
        // -> The query is not contained in the text
        startBorder = 0;
    }
    else if (query > text.substr(suffixArray[rightBorder])) {
        // The query is larger than the largest suffix in the suffix array
        // -> The query is not contained in the text
        startBorder = suffixArray.size();
    }
    else {
        // The query is contained in the text
        while (rightBorder - leftBorder > 1) {
            // middlePosition = (leftBorder + rightBorder) / 2;
            // Round up
            middlePosition = (leftBorder + rightBorder + 1) / 2;
            // Debug
            std::cout << "Left, middle, right: "  << leftBorder << " " << middlePosition << " " << rightBorder << std::endl;
            if (query <= text.substr(suffixArray[middlePosition])) {
                rightBorder = middlePosition;
            }
            else {
                leftBorder = middlePosition;
            }
        }
        startBorder = rightBorder;
    }

    std::cout << std::endl;

    // MLR: Binary search for the end border of the hits
    leftBorder = startBorder;
    rightBorder = suffixArray.size() - 1;
    middlePosition = 0;
    if (query < text.substr(suffixArray[0])) {
        // The query is smaller than the smallest suffix in the suffix array
        // -> The query is not contained in the text
        endBorder = 0;
    }
    else if (query > text.substr(suffixArray[rightBorder])) {
        // The query is larger than the largest suffix in the suffix array
        // -> The query is not contained in the text
        endBorder = suffixArray.size();
    }
    else {
        // The query is contained in the text
        while (rightBorder - leftBorder > 1) {
            middlePosition = (leftBorder + rightBorder + 1) / 2;
            // Debug
            std::cout << "Left, middle, right: "  << leftBorder << " " << middlePosition << " " << rightBorder << std::endl;
            if (query < text.substr(suffixArray[middlePosition])) {
                rightBorder = middlePosition;
            }
            else {
                leftBorder = middlePosition;
            }
        }
        endBorder = rightBorder;
    }

    // Debug
    std::cout << "Start border: " << startBorder << std::endl;
    std::cout << "End border: " << endBorder << std::endl;

    // Clear the hits vector
    hits.clear();
    
    // Add the hits to the hits vector
    for (uint32_t i = startBorder; i <= endBorder; i++) {
        hits.push_back(suffixArray[i]);
    }

    // Sort the hits vector
    std::sort(hits.begin(), hits.end());
}