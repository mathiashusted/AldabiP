#pragma once

#include <string>

// additional includes can go here:
// ...
#include <cstdint>
#include <vector>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <iomanip>


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
    enum class Traceback : int8_t{
    NONE,
    DIAGONAL,
    HORIZONTAL,
    VERTICAL,
  };
  std::string seqv;
  std::string seqh;
  std::vector<std::vector<int>> f;
  std::vector<std::vector<Traceback>> t;
  int score = 0;
  uint32_t localStartI = 0;
  uint32_t localStartJ = 0;
  bool computeCalled = false;
  bool smithWaterman = false;
};
