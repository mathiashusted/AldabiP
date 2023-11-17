#include "aufgabe2.hpp"
#include <iostream>


// Function to get the suffix of a text at a given index
std::string suftabText(const std::vector<uint32_t>& sa, const std::string& text, const uint32_t& index) {
    std::string output = "";
    for (uint32_t i = index; i < text.length(); i++) {
        output += text[i];
    }
    return output;
}


// int main() {
//     // Test string
//     std::string testString = "mississippi";

//     // Vector to hold the suffix array
//     std::vector<uint32_t> suffixArray;

//     // Construct the suffix array from the test string
//     construct(suffixArray, testString);

//     // Print the suffix array
//     std::cout << "Suffix Array for '" << testString << "':" << std::endl;
//     std::cout << "Length: " << suffixArray.size() << std::endl;
//     std::cout << "Indices: ";
//     for (auto index : suffixArray) {
//         std::cout << index << " ";
//     }
//     std::cout << std::endl;

//     // Print the suffixes of the test string in the order of the suffix array
//     std::cout << "Suffixes of '" << testString << "' in the order of the suffix array:" << std::endl;
//     for (auto index : suffixArray) {
//         std::cout << suftabText(suffixArray, testString, index) << std::endl;
//     }

//     // Test search
//     std::string query = "ss";
//     std::vector<uint32_t> hits;
//     find(query, suffixArray, testString, hits);

//     // Print the hits
//     std::cout << "Hits for '" << query << "':" << std::endl;
//     for (auto hit : hits) {
//         std::cout << hit << " ";
//     }
//     std::cout << std::endl;

//     // Print the suffixes of the hits
//     std::cout << "Suffixes of '" << testString << "' for the hits:" << std::endl;
//     for (auto hit : hits) {
//         std::cout << suftabText(suffixArray, testString, hit) << std::endl;
//     }

//     return 0;
// }

int main(int argc, const char* argv[]) {
    // Mode 1:
    if (argc == 2) {
        std::vector<uint32_t> suffixArray {};
        std::vector<uint32_t> hits {};
        construct(suffixArray, argv[1]);
        for (size_t i = 0; i < suffixArray.size(); i++)
            std::cout << suffixArray[i] << "\n";
            // std::cout << suffixArray[i] << " " << suftabText(suffixArray, argv[1], suffixArray[i]) << "\n"; // DEBUG
        //DEBUG
        // find("si", suffixArray, argv[1], hits);
    }
    else if (argc > 2) {
        // Costruct suffix array
        std::vector<uint32_t> suffixArray {};
        construct(suffixArray, argv[1]);

        // Find all occurences of the query for each query
        for (int i = 2; i < argc; i++) {
            std::vector<uint32_t> hits {};
            find(argv[i], suffixArray, argv[1], hits);
            
            std::cout << argv[i] << ": ";
            for (auto hit : hits) {
                std::cout << hit << " ";
            }
            std::cout << std::endl;
        }
    }
    else {
        std::cout << "----------USAGE----------\n"
        "Mode 1: aufgabe2_main \"string\" -> Outputs the suffix array of the corresponding string\n"
        "Mode 2: aufgabe2_main \"string 1\" \"string 2\" \"string 3\" ... \"string n\" -> Scans string 1 for string 2 to string n\n\n";
    }
    
    return 0;
}