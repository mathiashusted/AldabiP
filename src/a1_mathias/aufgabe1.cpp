#include "aufgabe1.h"

uint32_t Horspool::getShift_(const char last_char) const {
  return (patternMap.find(last_char) != this->patternMap.end()) ? patternMap.at(last_char) : this->pattern.length();
  // Here we first check that the argument isn't out of range in our lookup table.
  // If it is out of range, then we return a default value of 0.
  // This is the behaviour we want because of the following 2 cases:
  //    1. The character isn't in the lookup table at all -> skip the whole text (... and avoid throwing an exception :D)
  //    2. The character's index+1 is the length of the table itself -> skip the whole text
}


// This function will generate a lookup table and store the pattern
void Horspool::setPattern(const std::string& pat) {
  pattern = pat;
  patternMap.clear();
  uint32_t length = pattern.length();
  for (size_t i = 0; i < length; i++)
    patternMap[pattern[i]] = length;
  for (size_t j = 0; j < (length-1); j++)
    patternMap[pattern[j]] = length - j - 1;
  return;
}

const std::string& Horspool::getPattern() const {
  static const std::string empty;
  return (this->pattern.empty()) ? empty : this->pattern;
}

std::vector<size_t> Horspool::getHits(const std::string& text) const {
  uint32_t pos = 0;
  uint32_t j = 0;
  uint32_t m = pattern.length();
  uint32_t n = text.length();
  uint32_t shift = 0;
  std::vector<size_t> output{};
  // Here we initialize all variables to avoid compiler complications... (looking at you, macOS🤨)
  while (m <= n && pos <= (n - m) && !(text.empty()) && !(pattern.empty())) {
    j = m;
    shift = 0;

    while (j > 0 && (pos + j) <= n && text[pos+j-1] == pattern[j-1])
      j--;

    if (j == 0)
      output.push_back(pos);

    shift = getShift_(text[pos+m-1]); // Here we call the internal getShift_ function

    alignCheck_(pos); // Internal function. Perform checks...

    pos = (shift != 0) ? (pos + shift) : (pos + m);
  }
  return output;
}