#pragma once

#include <string>

// additional includes can go here:
// ...
#include <vector>

class Alignment
{
public:
  // DO NOT CHANGE THE PUBLIC INTERFACE!
  
  /// This makes the default constructor private
  /// i.e. an object of the class can only be created with sequences (see below)
  Alignment() = delete;  // no NOT implement this function. Just leave it as is.
  
  /// Constructor with two sequences
  /// Makes an internal copy of the sequences.
  Alignment(const std::string& seq_v, const std::string& seq_h);
  
  /// Compute the aligment (i.e. score and traceback)
  /// given the three alignment parameters match, mismatch and gap.
  /// If local_align == true, compute the local Smith-Waterman (SW) alignment (extra points), or throw
  /// an exception if your implementation does not support SW.
  void compute(const int match, const int mismatch, const int gap, const bool local_align = false);
  
  /// Return the score of the alignment;
  /// Throws an exception if compute(...) was not called first
  int getScore() const;
  
  /// Output alignment into three strings.
  /// Gaps are denoted as '-' in sequences.
  /// The gap-string uses '|' (match) and ' ' (mismatch/gap).
  /// Note: all strings are equal in size
  /// e.g.
  /// a1:   "IMISSMISSIS-SIPPI-"
  /// gaps: " |   ||||||  |||| "
  /// a2:   "-M--YMISSISAHIPPIE"
  /// , where a1 corresponds to seq1, etc.
  /// Throws an exception if compute(...) was not called first
  void getAlignment(std::string& a1, std::string& gaps, std::string& a2) const;
  
private:
  // add your private functions and member variables here
  // ...

  /** Enum for the traceback directions. */
  enum TracebackDirection: int8_t { NONE, LEFT, UP, DIAGONAL };

  /** Local variables. */
  std::string seq_first;
  std::string seq_second;
  std::string aligned_seq_first;
  std::string aligned_seq_second;
  std::string aligned_gaps;
  int score;
  int maxI;
  int maxJ;
  int match_cost;
  int mismatch_cost;
  int gap_cost;
  bool local_align;
  std::vector<std::vector<int>> matrix;
  std::vector<std::vector<int>> traceback;

  /**
   * @brief Initialize the matrix
   * 
   * This function initializes the matrix with the
   * correct values.
  */
  void initializeMatrix();

  /**
   * @brief Fill the matrix
   * 
   * This function fills the matrix with the correct
   * values by using recursion.
  */
  void doRecursion();

  /**
   * @brief Construct the alignment
   * 
   * This function constructs the alignment from the
   * traceback matrix.
  */
  void constructAlignment();

  /**
   * @brief Get the maximum direction
   * 
   * This function returns the maximum direction
   * from the three given values.
   * 
   * @param matchScore The score for a match
   * @param deleteScore The score for a deletion
   * @param insertScore The score for an insertion
   * @return TracebackDirection The maximum direction
  */
  TracebackDirection getMaxDirection(int matchScore, int deleteScore, int insertScore) const;
};
