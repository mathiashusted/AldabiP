#include "ACTrie.hpp"

#include <stdexcept>
#include <iostream>


ACTrie::ACTrie(const std::vector<std::string>& needles) {
    if (needles.empty()) throw std::logic_error("Input vector is empty!\n");

    // Perform initial cleanup
    this->clear();

    // Start at root
    this->needles = needles;
    this->nodes.push_back(ACNode()); // Generate a new root node...
    this->root = 0; // ... which becomes index 0
    

    for (const auto& needle : needles) {
        std::cout << "\n\nNeedle: " << needle << "\n";
        this->add_child(needle);
        this->nodes[this->current_node].output = true;
    }

}

void ACTrie::insert_child(char to_insert) {
    nodes.push_back(ACNode(to_insert, current_node, nodes[current_node].depth + 1));
    unsigned new_index = nodes.size() - 1;
    nodes[current_node].children.push_back(new_index);
    current_node = new_index;
}


void ACTrie::add_child(const std::string& needle) {
    // For each iteration, we start at the root
    this->current_node = this->root;
    for (const auto& letter : needle) {
        std::cout << "Current position: " << current_node << ", " << nodes[current_node].symbol << "\n";
        std::cout << "Executing add_child for " << letter << "\n";
        // Create new node if our current position has nowhere to go
        if (nodes[current_node].children.empty()) {
            std::cout << "No children, creating...\n";
            this->insert_child(letter);
        }
        // Otherwise, keep following the path
        else {
            bool child_has_been_found = false;
            std::cout << "Node has children, searching there instead...\n";
            for (const auto& child : nodes[current_node].children) {
                if (nodes[child].symbol == letter) {
                    std::cout << "Found previous child, continuing from there...\n";
                    current_node = child;
                    child_has_been_found = true;
                    break;
                }
            }
            // If we haven't found a child, create a new one:
            if (!child_has_been_found) {
                std::cout << "Hm, didn't find one, create it instead!\n";
                this->insert_child(letter);
            }
        }
    }
    return;
}

std::string ACTrie::getTree() const {
    std::string tree = this->generate_tree(this->root, false);
    return tree;
}

std::string ACTrie::generate_tree(int starting_node, bool called) const {
    if (this->nodes[starting_node].children.empty())
        return std::string {this->nodes[starting_node].symbol};
    
    std::string output = "";
    output += this->nodes[starting_node].symbol;
    for (const auto& child : this->nodes[starting_node].children)
        output += "(" + this->generate_tree(child, true) + ")";

    return output;
}

void ACTrie::clear() {
    this->nodes.clear();
    this->haystack = "";
    this->current_node = 0;
    this->needles = std::vector<std::string> {};

}