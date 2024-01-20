#include <iostream>
#include "ACTrie.hpp"


int main() {
    std::vector<std::string> needles {"ga", "c"};
    ACTrie trie(needles);

    std::cout << "Execuring getTree:\n" << trie.getTree();
}