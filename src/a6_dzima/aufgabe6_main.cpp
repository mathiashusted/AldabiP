#include "ACTrie.hpp"
#include <iostream>


int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: ./aufgabe6_main <QUERY> <Pattern1> <Pattern2> ...";
        return 1;
    }

    std::string query = argv[1];
    std::vector<std::string> patterns = std::vector<std::string>(argc - 2);
    for (int i = 2; i < argc; i++) {
        patterns[i - 2] = argv[i];
    }

    std::vector<Hit> hits;

    std::cout << "Query: " << query << std::endl;
    std::cout << "Patterns: ";
    for (const auto& pattern : patterns) {
        std::cout << pattern << " ";
    }
    std::cout << std::endl;

    ACTrie trie(patterns);
    std::cout << "Trie built" << std::endl;
    std::string tree = trie.getTree();
    std::cout << tree << std::endl;

    trie.setQuery(query);

    while (trie.next(hits)) {
        std::cout << "Found hits: ";
        for (const auto& hit : hits) {
            std::cout << hit.pos << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}