#include <iostream>
#include "ACTrie.hpp"


int main() {
    std::vector<std::string> needles {"AC", "AB", "ABD", "ABE"};
    ACTrie trie(needles);

    std::cout << "Execuring getTree:\n" << trie.getTree();
}