#include "ACTrie.hpp"
#include <iostream>


int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: ./aufgabe6_main <QUERY> <Pattern1> <Pattern2> ...";
        return 1;
    }

    std::string query = argv[1];
    std::vector<std::string> patterns = std::vector<std::string>(argc - 2);
    std::vector<Hit> hits;

    ACTrie instance(patterns);
    instance.setQuery(query);
    std::string treeRepresetation = instance.getTree();
    std::cout << "Tree representation: " << treeRepresetation << "\n";
    instance.next(hits);

    std::cout << "Hits: " << hits.size() << "\n";
    for (Hit hit : hits) {
        std::cout << hit.index << " " << hit.pos << "\n";
    }

    instance.next(hits);

    std::cout << "Hits: " << hits.size() << "\n";
    for (Hit hit : hits) {
        std::cout << hit.index << " " << hit.pos << "\n";
    }

    return 0;
}