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


int main(int argc, const char* argv[]) {
    // Mode 1:
    if (argc == 2) {
        std::vector<uint32_t> suffixArray {};
        std::vector<uint32_t> hits {};
        construct(suffixArray, argv[1]);
        for (size_t i = 0; i < suffixArray.size(); i++)
            std::cout << suffixArray[i] << "\n";
            // std::cout << suffixArray[i] << " " << suftabText(suffixArray, argv[1], suffixArray[i]) << "\n"; // DEBUG
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