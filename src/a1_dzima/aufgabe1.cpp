/*
Horspool Algorithmus.

Compile with:
g++ -std=c++17 -Wall -pedantic -O3 aufgabe1.cpp aufgabe1_main.cpp -o
aufgabe1_main Run with:
./aufgabe1_main  "Yet another text?" "really?"
Test with:
g++ -std=c++17 -Wall -pedantic -O3 aufgabe1.cpp aufgabe1_test.cpp -o
aufgabe1_test
./aufgabe1_test
*/

#include "aufgabe1.h"

uint32_t Horspool::getShift_(const char last_char) const {
  // Directly access shift Value from shiftVec
  return this->shiftVec[static_cast<unsigned char>(last_char)];
}

void Horspool::setPattern(const std::string &pat) {
  // Set pattern and initialize shiftVec
  this->pattern = pat;
  uint32_t patternLength = pat.length();

  // Set all values in shiftVec to patternLength
  std::fill(this->shiftVec, this->shiftVec + 256, patternLength);

  // Set values in shiftVec for each character in pattern
  for (uint32_t j = 0; j < patternLength - 1; j++) {
    this->shiftVec[static_cast<unsigned char>(pat[j])] = patternLength - j - 1;
  }

  // Set value for last character in pattern to 1
  this->shiftVec[static_cast<unsigned char>(pat[patternLength - 1])] = 1;
}

const std::string &Horspool::getPattern() const {
  static const std::string empty;
  return (this->pattern.empty()) ? empty : this->pattern;
}

std::vector<size_t> Horspool::getHits(const std::string &text) const {
  // Check pattern, text correctness
  if (this->pattern.empty() || text.empty()) {
    return {};
  }

  // Initialize variables
  uint32_t textPosition = 0;
  uint32_t i;
  uint32_t patternLength = this->pattern.length();
  uint32_t textLength = text.length();
  // Reference to pattern for faster access (enabling caching)
  const std::string &patternRef = this->pattern;
  uint32_t shift;
  std::vector<size_t> output;

  while (textPosition <= (textLength - patternLength)) {
    alignCheck_(textPosition);
    i = patternLength - 1;

    // Check if pattern mathces text
    while (i >= 0 && (text[textPosition + i] == patternRef[i] ||
                      patternRef[i] == '?' || text[textPosition + i] == '?')) {
      --i;
    }
    // If all characters match, add position to output
    if (i == -1) {
      output.push_back(textPosition);
    }

    shift = getShift_(text[textPosition + patternLength - 1]);
    textPosition += shift;
  }

  return output;
}
