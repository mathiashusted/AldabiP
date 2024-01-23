#include "ACTrie.hpp"

#include <stdexcept>
#include <iostream>
#include <queue>


ACTrie::ACTrie(const std::vector<std::string>& needles) {
    if (needles.empty()) throw std::logic_error("Input vector is empty!\n");

    // Check if all needles are valid
    for (const auto& needle : needles) {
        for (const auto& letter : needle) {
            if (letter < 'A' || letter > 'Z') throw std::exception();
        }
    }

    // Perform initial cleanup
    this->clear();

    // Start at root
    this->needles = needles;
    this->nodes.push_back(ACNode()); // Generate a new root node...
    this->root = 0; // ... which becomes index 0
    this->nodes[root].parent_link = -1;
    this->nodes[root].needle_indices = {};

    
    // Create the nodes for each needle
    for (const auto& needle : needles) {
        // std::cout << "\n\nNeedle: " << needle << "\n";
        this->add_child(needle);
        this->nodes[this->current_node].output = true;
    }

    // Perform BFS search using a queue
    this->create_supply_link();

}


void ACTrie::create_supply_link() {
    // Initialize a queue to hold nodes for processing
    std::queue<unsigned> q;
    std::vector<bool> visited {}; // Hold the visited nodes by their index
    visited.resize(this->nodes.size(), 0);
    q.push(this->root);

    // Queue BFS: Process nodes until the queue is empty
    while (!q.empty()) {
        unsigned next = q.front();
        q.pop();  // Remove the front node from the queue

        for (const auto& adjacent : this->nodes[next].children) {
            if (!visited[adjacent]) {
                visited[adjacent] = true;
                q.push(adjacent);
            }
        }
        // std::cout << "Status: " << nodes[next].symbol << "\n";
        if (next == 0 || this->nodes[next].parent_link == 0) {
            // std::cout << "Set to 0\n";
            this->nodes[next].supply_link = 0;
        }
        else {
            int& supply_link = this->nodes[this->nodes[next].parent_link].supply_link; // Create a reference to the current node's parent's supply link
            // ...
        }
    }

    
}

void ACTrie::insert_child(char to_insert) {
    // Pushing back a node into our nodes vector, and thereafter setting it as our selected node
    nodes.push_back(ACNode(to_insert, current_node, nodes[current_node].depth + 1));
    unsigned new_index = nodes.size() - 1;
    nodes[current_node].children.push_back(new_index);
    nodes[new_index].parent_link = current_node;
    current_node = new_index;
}


void ACTrie::add_child(const std::string& needle) {
    // For each iteration, we start at the root
    this->current_node = this->root;
    for (const auto& letter : needle) {
        // std::cout << "Current position: " << current_node << ", " << nodes[current_node].symbol << "\n";
        // std::cout << "Executing add_child for " << letter << "\n";
        // Create new node if our current position has nowhere to go
        if (nodes[current_node].children.empty()) {
            // std::cout << "No children, creating...\n";
            this->insert_child(letter);
        }
        // Otherwise, keep following the path
        else {
            bool child_has_been_found = false;
            // std::cout << "Node has children, searching there instead...\n";
            for (const auto& child : nodes[current_node].children) {
                if (nodes[child].symbol == letter) {
                    // std::cout << "Found previous child, continuing from there...\n";
                    current_node = child;
                    child_has_been_found = true;
                    break;
                }
            }
            // If we haven't found a child, create a new one:
            if (!child_has_been_found) {
                // std::cout << "Hm, didn't find one, create it instead!\n";
                this->insert_child(letter);
            }
        }
    }
    return;
}

std::string ACTrie::getTree() const {
    return this->generate_tree(this->root, false);
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

bool ACTrie::next(std::vector<Hit>& hits) {
    return false;
}

void ACTrie::setQuery(const std::string& haystack) {
    this->haystack = haystack;
}