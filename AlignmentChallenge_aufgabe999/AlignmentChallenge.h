#include <string>
#include <vector>

class Alignment
{
public:
  // DO NOT CHANGE THE PUBLIC INTERFACE!
  
  /// This makes the default constructor private
  /// i.e. an object of the class can only be created with sequences (see below)
  Alignment() = delete;  // no NOT implement this function. Just leave it as is.
  
  /// Constructor to define the scoring scheme
  Alignment(const int match, const int mismatch, const int gap);
  
  /// compute the global aligment of all vs. all sequences and return the sum of scores
  int compute(const std::vector<std::string>& seqs, const int threads);
  
 
  
private:
  
  // add your private functions and member variables here
  ///....
  

};
