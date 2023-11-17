

#include <cstdint>
#include <algorithm>
#include <iostream> //DEBUG
#include <numeric>
#include "aufgabe2.hpp"


void construct(std::vector<uint32_t>& suffixArray, const std::string& text) {
    /**
     * @brief Constructs a suffix array from a given text.
     * 
     * @param suffixArray The suffix array to be constructed.
     * @param text The text to be used for construction.
    */

    // Clearing the suffix array and reserving enough space for the suffixes
    uint32_t textLen = text.length();
    suffixArray.resize(textLen);
    size_t counter = 0;

    for (uint32_t i = 0; i < textLen; i++)
        suffixArray[i] = i;

    /** 
     * Sorting our array with a custom lambda expression (as per cppreference.com) which sorts by comparing the text 
     * of two suffixArray entries &text and &counter are captured, so that we can use them for lookup inside of our 
     * lambda function a and b are automatically taken from suffixArray, which holds values from 1..textLen (our suffix 
     * start positions) Two entries are compared by their ASCII values and return 1 if the a-suffix is smaller than the 
     * b-suffix, otherwise 0. 
     */
    std::sort(suffixArray.begin(), suffixArray.end(), [&text, &counter](const uint32_t & a, const uint32_t & b) {
        for (counter = 0; text[a+counter] == text[b+counter];)
            counter++;

        return (text[a+counter] < text[b+counter]);
    });
    return;
}

// bool checkSubstringMatch(const std::string& query, const std::string& text, const uint32_t& suffixArrayIndex) {
//     /**
//      * @brief Checks if a given query matches a substring of a given text at a given index.
//      * 
//      * @param query The query to be checked.
//      * @param text The text to be checked in.
//      * @param suffixArrayIndex The index of the suffix array to be checked.
//      * 
//      * @return True if the query matches the substring, false otherwise.
//     */

//     // Initial checks
//     if (query.empty() || text.length() == 0 || suffixArrayIndex > text.length())
//         return false;

//     // Check if the query matches the substring
//     for (uint32_t i = 0; i < query.length(); i++) {
//         if (query[i] != text[suffixArrayIndex+i])
//             return false;
//     }
//     return true;
// }

// uint32_t calculateMLR(const std::string& firstString, const std::string& secondString) {
//     /**
//      * @brief Calculates the MLR of two given strings.
//      * 
//      * @param firstString The first string to be compared.
//      * @param secondString The second string to be compared.
//      * 
//      * @return The MLR of the two given strings.
//     */

//     // Initial checks
//     if (firstString.empty() || secondString.empty())
//         return 0;

//     // Calculate the MLR
//     uint32_t mlr = 0;
//     uint32_t minStringLength = std::min(firstString.length(), secondString.length());
//     for (uint32_t i = 0; i < minStringLength; i++) {
//         if (firstString[i] == secondString[i])
//             mlr++;
//         else
//             break;
//     }
//     return mlr;
// }

// void find(const std::string& query, const std::vector<uint32_t>& suffixArray, const std::string& text, std::vector<uint32_t>& hits) {
//     /**
//      * @brief Finds all occurences of a given query in a given text.
//      * 
//      * @param query The query to be searched for.
//      * @param suffixArray The suffix array to be used for lookup.
//      * @param text The text to be searched in.
//     */

//    // Debug
//    // Suffix Array
//     std::cout << "Suffix Array: \n";
//     for (uint32_t i = 0; i < suffixArray.size(); i++)
//         std::cout << i << ": " << suffixArray[i] << ": " << text.substr(suffixArray[i]) << "\n";

    // hits.clear();
    // if (query.empty() || suffixArray.empty() || text.empty())
    //     return;

//     uint32_t lp = 0;
//     uint32_t rp = 0;
//     uint32_t L = 0;
//     uint32_t R = 0;
//     uint32_t M = 0;
//     uint32_t mlr;
//     uint32_t suffixMaxIndex = suffixArray.size()-1;

//     // If the query is bigger than the biggest suffix
//     if (query[0] > text[suffixArray[suffixMaxIndex]]) {
//         return;
//     }

//     // If the query is smaller than the smallest suffix
//     if (query[0] < text[suffixArray[0]])
//         lp = 0;

//     else {
//         L = 0;
//         R = suffixMaxIndex;

//         while (R > L) {
//             mlr = calculateMLR(text.substr(suffixArray[L]), text.substr(suffixArray[R]));
//             M = (L + R+1)/2;
//             std::cout << "(L,R) = (" << L << "," << R << ") => M = " << M << "\n";
//             std::cout << "mlr = " << mlr << "\n";
//             if (query.substr(mlr) <= text.substr(suffixArray[M] + mlr))
//                 R = M;
//             else
//                 L = M;
//         }
//         lp = R;
//     }
//     std::cout << "Final value: (L,R) = (" << L << "," << R << ")" << "\n";
//     std::cout << "\n\nLp = " << lp << "\n";

//     L = lp;
//     // R = suffixArray.size()-1;
//     R = suffixMaxIndex;

//     while (R > L) {
//         mlr = calculateMLR(text.substr(suffixArray[L]), text.substr(suffixArray[R]));
//         M = (L + R+1)/2;
//         std::cout << "(L,R) = (" << L << "," << R << ") => M = " << M << "\n";
//         std::cout << "mlr = " << mlr << "\n";
//         if (query < text.substr(suffixArray[M], query.length()))
//             R = M;
//         else
//             L = M;
//     }
//     rp = L;

//     //std::cout << "Final value: (L,R) = (" << L << "," << R << ")" << "\n";
//     // std::cout << "Rp = " << rp << "\n";

//     hits.clear();
//     for (uint32_t i = lp; i <= rp; i++) {
//         if (i < suffixArray.size()-1)
//             hits.push_back(suffixArray[i]);
//     }
//     std::sort(hits.begin(), hits.end());
// }


void find(const std::string& query, const std::vector<uint32_t>& suffixArray, const std::string& text, std::vector<uint32_t>& hits) {
    uint32_t L = 0;
    uint32_t R = suffixArray.size() - 1;
    uint32_t mlr = 0; // Assuming you have a function to calculate this
    uint32_t M;

    // Find the lower bound of the query in the suffix array
    while (R - L > 1) {
        M = (L + R) / 2;
        if (query.compare(0, query.size(), text, suffixArray[M], query.size()) <= 0)
            R = M;
        else
            L = M;
    }

    uint32_t start = (query.compare(0, query.size(), text, suffixArray[L], query.size()) <= 0) ? L : R;

    // Find the upper bound of the query in the suffix array
    L = 0;
    R = suffixArray.size() - 1;
    while (R - L > 1) {
        M = (L + R) / 2;
        if (query.compare(0, query.size(), text, suffixArray[M], query.size()) < 0)
            R = M;
        else
            L = M;
    }

    uint32_t end = (query.compare(0, query.size(), text, suffixArray[R], query.size()) < 0) ? L : R;

    // Populate hits with indices where the query is found
    for (uint32_t i = start; i <= end; ++i) {
        if (text.compare(suffixArray[i], query.size(), query) == 0) {
            hits.push_back(suffixArray[i]);
        }
    }
}

