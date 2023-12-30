#include "QGramIndex.hpp"
#include <iostream>
#include <fstream>
#include <cstring>

int main(int argc, char** argv) {
    // std::string pattern = "ACCGTCGTC";
    // uint8_t q_length = 2;
    // QGramIndex instance(pattern, q_length);
    // instance.getHits(instance.hash("AA"));
    if (argc != 3) {
        std::cout << "Usage: ./aufgabe5_main <GENOME_FILE> <QUERY>";
        return 1;
    }

    uint32_t q_length = std::strlen(argv[2]);


    std::ifstream genome(argv[1]);
    std::string text;
    if (genome.is_open()) {
        if (std::getline(genome, text)) {
            // Success, do nothing
        }
        else {
            std::cout << "Couldn't read the file, try something else...";
            return 1;
        }
        genome.close();
    }
    else {
        std::cout << "Couldn't find the file, check spelling...";
        return 1;
    }

    try {
        QGramIndex instance(text, q_length);
        std::vector<uint32_t> matches = instance.getHits(instance.hash(argv[2]));

        std::cout << argv[2] << ": " << q_length << " " << matches.size() << "\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}