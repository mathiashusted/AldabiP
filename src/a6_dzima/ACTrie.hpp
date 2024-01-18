#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>

/// A needle (at position #i, as passed into ACTrie's constructor) was found in the haystack (query) at position 'p'
/// (all values are 0-based)
/// Do NOT change this structure!
struct Hit
{
  Hit() = default;
  Hit(size_t i, size_t p) : index(i), pos(p) {};
  size_t index;
  size_t pos;
  bool operator==(const Hit& rhs) const
  {
    return index==rhs.index && pos==rhs.pos;
  }
};

/// An index (into a vector) representing the location of a node
/// You do not have to use this class, but it might be useful...
/// It could also be extended to contain an edge label (character), if your design requires it...
class Index
{
public:
  Index() = default;
  Index(size_t index) : i_(index) {};
  /// is this Index actually valid?
  bool isNull() const { return i_ == std::numeric_limits<size_t>::max(); }
  /// what is the index? (might be invalid, i.e. .isNull() == true)
  size_t pos() const { return i_; }
  /// allows to set the index, using `index.pos() = 3;` or simply read its value
  size_t& pos() { return i_; }

private:
  size_t i_ = std::numeric_limits<size_t>::max();
};


class ACNode {
public:
    char symbol;
    ACNode* parent_link;  // snake-case to avoid confusions
    ACNode* supply_link;  // snake-case to avoid confusions
    ACNode* output_link;  // snake-case to avoid confusions
    std::unordered_map<char, ACNode*> children;
    std::vector<int> patternIndices;
    int depth;

    // Default constructor
    ACNode() {
        this->symbol = '0';
        this->parent_link = nullptr;
        this->supply_link = nullptr;
        this->output_link = nullptr;
        this->depth = 0;
    }

    // Construtor with symbol, parent link and depth
    ACNode(char symbol, ACNode* parent_link, int depth) {
        this->symbol = symbol;
        this->parent_link = parent_link;
        this->supply_link = nullptr;
        this->output_link = nullptr;
        this->depth = depth;
    }
};


class ACTrie
{
public:
  // do not change the public interface!
  
  /// Constructor with a vector of needles (i.e. multiple patterns to be found in a hackstack)
  /// Needles may only contain letters from 'A' - 'Z'. All other characters lead to a std::exception being thrown.
  /// At least one needle is required. Passing an empty vector leads to throwing of std::logic_error.
  ACTrie(const std::vector<std::string>& needles);

  /// Set a haystack (query) where the needles (patterns) are to be searched
  /// This also resets the current trie-node to ROOT
  void setQuery(const std::string& haystack);

  /// Resume at the last position in the haystack and node in the trie:
  ///   consume a character at a time until one or more patterns are found (mind to check output links at every step as well).
  /// Fill @p hits with all patterns found at the current node (incl. output links) and return true.
  /// If the end of the haystack is reached without a match, return false.
  /// When this function is called multiple times, it resumes from the last position (state of the trie).
  bool next(std::vector<Hit>& hits);
  
  
  /// Get Tree as string representation (for scoring the construction) using bracket notation.
  /// Starting at root (with special label '0'), enumerate all children in alphabetical order (!) in a depth-first search (DFS) and
  /// enclose each subtree in '()'.
  /// If you are implementing a 'fully resolved' AC trie, only print the edges which increase in depth (i.e. were not created by following suffix links).
  /// E.g. given the AC tree of 'ga' and 'c'
  ///       0
  ///     /   \
  ///    g     c
  ///   /    
  ///  a     
  /// Output: "0(c)(g(a))"
  std::string getTree() const;


private:
  // add your private functions and member variables here
  // ....
  ACNode* root;

  std::string haystack;
  size_t haystackPos;
  ACNode* currentNode;

  std::vector<std::string> needles;
  std::vector<Hit> hits;
  
  void clear();

  void addNeedle(const std::string& needle, int patternIndex);

  void buildSupplyLinks();

  void buildOutputLinks();

};



