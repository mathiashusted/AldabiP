#include "ACTrie.hpp"
#include <iostream>


ACTrie::ACTrie(const std::vector<std::string>& needles) {
    if (needles.empty()) throw std::logic_error("Input vector is empty!\n");

    // Check if all needles are valid
    for (const auto& needle : needles) {
        for (const auto& letter : needle) {
            if (letter < 'A' || letter > 'Z') throw std::exception();
        }
    }

    // Clear artifacts from previous runs
    this->clear();

    this->needles = needles;
    this->root = new ACNode();
    this->currentNode = this->root;

    // Add all needles to the trie
    for (size_t i = 0; i < needles.size(); i++) {
        this->addNeedle(needles[i], i);
    }

    // Set supply links
    this->buildSupplyLinks();

    // Set output links
    this->buildOutputLinks();
}

void ACTrie::clear() {
    this->root = nullptr;
    this->haystack = "";
    this-> haystackPos = 0;
    this->currentNode = nullptr;
    this->needles.clear();
    this->hits.clear();
}

void ACTrie::addNeedle(const std::string& needle, int index) {
    ACNode* activeNode = this->root;
    for (const auto& letter : needle) {
        if (activeNode->children.find(letter) == activeNode->children.end()) {
            ACNode* newNode = new ACNode(letter, activeNode, activeNode->depth + 1);
            activeNode->children[letter] = newNode;
        }
        activeNode = activeNode->children[letter];
    }
    activeNode->patternIndices.push_back(index);
}

void ACTrie::buildSupplyLinks() {
    // Initialize a queue to hold nodes for processing
    std::queue<ACNode*> nodeQueue;
    nodeQueue.push(this->root); // Start with the root node

    // Queue BFS: Process nodes until the queue is empty
    while (!nodeQueue.empty()) {
        ACNode* activeNode = nodeQueue.front(); // Get the front node
        nodeQueue.pop(); // Remove the front node from the queue

        // Add all children of the current node to the queue for processing
        for (const auto& child : activeNode->children) {
            nodeQueue.push(child.second);
        }

        // If the current node is the root or its parent is the root,
        // set the supply link to root (base case)
        if (activeNode == this->root || activeNode->parent_link == this->root) {
            activeNode->supply_link = this->root;
        }
        else {
            // Traverse up the supply links to find the longest valid suffix
            // This forms the supply link for the current node
            ACNode* supplyLink = activeNode->parent_link->supply_link;
            while (supplyLink != nullptr && supplyLink->children.find(activeNode->parent_link->children.begin()->first) == supplyLink->children.end()) {
                supplyLink = supplyLink->supply_link;
            }

            // If no valid suffix is found, link to root
            if (supplyLink == nullptr) {
                activeNode->supply_link = this->root;
            }
            else {
                // Else, link to the child node of the found suffix
                activeNode->supply_link = supplyLink->children[activeNode->parent_link->children.begin()->first];
            }
        }
    }
}

void ACTrie::buildOutputLinks() {
    // Initialize a queue to hold nodes for processing
    std::queue<ACNode*> nodeQueue;
    nodeQueue.push(this->root); // Start with the root node

    // Queue BFS: Process nodes until the queue is empty
    while (!nodeQueue.empty()) {
        ACNode* activeNode = nodeQueue.front(); // Get the front node
        nodeQueue.pop(); // Remove the front node from the queue

        // Add all children of the current node to the queue for processing
        for (const auto& child : activeNode->children) {
            nodeQueue.push(child.second);
        }

        // If the current node is the root, set the output link to root (base case)
        if (activeNode == this->root) {
            activeNode->output_link = this->root;
        }
        else {
            // If the supply link of the current node is a leaf node, set the output link to the supply link
            if (activeNode->supply_link->patternIndices.size() > 0) {
                activeNode->output_link = activeNode->supply_link;
            }
            else {
                // Else, traverse up the supply links to find the first node with a valid output link
                // This forms the output link for the current node
                ACNode* outputLink = activeNode->supply_link->output_link;
                while (outputLink != nullptr && outputLink != this->root && outputLink->patternIndices.size() == 0) {
                    outputLink = outputLink->output_link;
                }

                // If no valid output link is found, link to root
                if (outputLink == nullptr) {
                    activeNode->output_link = this->root;
                }
                else {
                    // Else, link to the found output link
                    activeNode->output_link = outputLink;
                }
            }
        }
    }
}

void ACTrie::setQuery(const std::string& haystack) {
    this->haystack = haystack;
    this->haystackPos = 0;
    this->currentNode = this->root;
    this->hits.clear();
}

bool ACTrie::next(std::vector<Hit>& hits) {
    // If the end of the haystack is reached, return false
    if (this->haystackPos >= this->haystack.size()) {
        return false;
    }

    // Traverse the trie until a match is found or the end of the haystack is reached
    while (this->haystackPos < this->haystack.size()) {
        // If the current node has a child with the current letter, traverse to that child
        if (this->currentNode->children.find(this->haystack[this->haystackPos]) != this->currentNode->children.end()) {
            this->currentNode = this->currentNode->children[this->haystack[this->haystackPos]];
            this->haystackPos++;
            break;
        }
        // Else, if the current node is the root, increment the haystack position
        else if (this->currentNode == this->root) {
            this->haystackPos++;
        }
        // Else, traverse up the supply links until a valid suffix is found
        else {
            this->currentNode = this->currentNode->supply_link;
        }
    }

    // If the end of the haystack is reached, return false
    if (this->haystackPos >= this->haystack.size()) {
        return false;
    }

    // Traverse the output links to find all patterns that end at the current node
    ACNode* outputLink = this->currentNode;
    while (outputLink != nullptr) {
        // If the current node is a leaf node, add all patterns to the hits vector
        if (outputLink->patternIndices.size() > 0) {
            for (const auto& index : outputLink->patternIndices) {
                hits.push_back(Hit(this->haystackPos - this->needles[index].size(), index));
            }
        }
        outputLink = outputLink->output_link;
    }

    return true;
}

std::string ACTrie::getTree() const {
    // Initialize an empty string to represent the trie
    std::string tree = "";
    
    // A queue to hold nodes for breadth-first traversal of the trie
    std::queue<ACNode*> nodeQueue;
    nodeQueue.push(this->root); // Start with the root node

    // Queue BFS: Process nodes until the queue is empty
    while (!nodeQueue.empty()) {
        ACNode* activeNode = nodeQueue.front(); // Get the front node
        nodeQueue.pop(); // Remove the front node from the queue

        // Exclude the root node from being wrapped in parentheses
        if (activeNode != this->root) {
            tree += "(";
        }

        // Add the depth of the current node to the tree representation
        tree += activeNode->symbol;

        // Add all children of the current node to the queue for processing
        for (const auto& child : activeNode->children) {
            nodeQueue.push(child.second);
        }

        // Close the parentheses for non-root nodes
        if (activeNode != this->root) {
            tree += ")";
        }
    }

    // Return the string representation of the trie
    return tree;
}
