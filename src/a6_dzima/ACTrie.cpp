#include "ACTrie.hpp"


void ACTrie::addPattern(const std::string& pattern, int patternIndex) {
    ACNode* current = root;
    for (char ch : pattern) {
        if (current->children.find(ch) == current->children.end()) {
            current->children[ch] = new ACNode(current->depth + 1);
            current->children[ch]->parent_link = current;
        }
        current = current->children[ch];
    }
    current->pattern_indices.push_back(patternIndex);
}

void ACTrie::buildTrie(const std::vector<std::string>& patterns) {
    for (int i = 0; i < patterns.size(); i++) {
        addPattern(patterns[i], i);
    }
}

void ACTrie::buildSupplyLinks() {
    std::queue<ACNode*> q;
    q.push(this->root);
    while (!q.empty()) {
        ACNode* current = q.front();
        q.pop();
        for (auto it = current->children.begin(); it != current->children.end(); it++) {
            ACNode* child = it->second;
            ACNode* parent_link = current->parent_link;
            while (parent_link != nullptr && parent_link->children.find(it->first) == parent_link->children.end()) {
                parent_link = parent_link->parent_link;
            }
            if (parent_link == nullptr) {
                child->supply_link = root;
            }
            else {
                child->supply_link = parent_link->children[it->first];
            }
            q.push(child);
        }
    }
}

void ACTrie::buildOutputLinks() {
    std::queue<ACNode*> q;
    q.push(this->root);
    while (!q.empty()) {
        ACNode* current = q.front();
        q.pop();
        for (auto it = current->children.begin(); it != current->children.end(); it++) {
            ACNode* child = it->second;
            if (child->supply_link->pattern_indices.size() > 0) {
                child->output_link = child->supply_link;
            }
            else {
                child->output_link = child->supply_link->output_link;
            }
            q.push(child);
        }
    }
}

ACTrie::ACTrie(const std::vector<std::string>& patterns) {
    this->root = new ACNode();
    // Check if patterns is empty
    if (patterns.size() == 0) {
        throw std::logic_error("Needles vector is empty");
    }
    // Check if patterns contain only letters from 'A' - 'Z'
    for (std::string pattern : patterns) {
        for (char ch : pattern) {
            if (ch < 'A' || ch > 'Z') {
                throw std::exception();
            }
        }
    }

    buildTrie(patterns);
    buildSupplyLinks();
    buildOutputLinks();
}

void ACTrie::setQuery(const std::string& haystack) {
    this->haystack = haystack;
    this->haystack_pos = 0;
    this->current_node = this->root;
}

bool ACTrie::next(std::vector<Hit>& hits) {
    if (this->haystack_pos >= this->haystack.size()) {
        return false;
    }
    char ch = this->haystack[this->haystack_pos];
    while (this->current_node != nullptr && this->current_node->children.find(ch) == this->current_node->children.end()) {
        this->current_node = this->current_node->supply_link;
    }
    if (this->current_node == nullptr) {
        this->current_node = this->root;
    }
    else {
        this->current_node = this->current_node->children[ch];
    }
    if (this->current_node->pattern_indices.size() > 0) {
        for (int pattern_index : this->current_node->pattern_indices) {
            hits.push_back(Hit(pattern_index, this->haystack_pos - patterns[pattern_index].size() + 1));
        }
    }
    ACNode* output_link = this->current_node->output_link;
    while (output_link != nullptr) {
        for (int pattern_index : output_link->pattern_indices) {
            hits.push_back(Hit(pattern_index, this->haystack_pos - patterns[pattern_index].size() + 1));
        }
        output_link = output_link->output_link;
    }
    this->haystack_pos++;
    return true;
}

std::string ACTrie::getTree() const {
    std::string tree = "";
    std::queue<ACNode*> q;
    q.push(this->root);
    while (!q.empty()) {
        ACNode* current = q.front();
        q.pop();
        tree += std::to_string(current->depth) + " ";
        for (auto it = current->children.begin(); it != current->children.end(); it++) {
            ACNode* child = it->second;
            tree += it->first;
            q.push(child);
        }
        tree += "\n";
    }
    return tree;
}