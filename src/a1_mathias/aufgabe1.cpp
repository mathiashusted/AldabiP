
#include "aufgabe1.h"


// This function will generate a lookup table and store the pattern
void Horspool::setPattern(const std::string& pat) {
  this->pattern = pat;
  for (size_t i = 0; i < pat.length(); i++) {
    this->patternMap[pat[i]] = pat.length();
  }
  for (size_t j = 0; j < pat.length(); j++) {
    this->patternMap[pat[j]] = pat.length() - (j+1);
  }

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
  std::vector<size_t> output{};

  while (pos <= (n - m) && !(text.empty()) && !(pattern.empty())) {
    j = m;

    while (j > 0 && (pos + j) <= n && text[pos+j-1] == pattern[j-1])
      j--;

    if (j == 0)
      output.push_back(pos);


    if (patternMap.find(text[pos+m-1]) != patternMap.end() && patternMap.at(text[pos+m-1]) != 0) 
      pos = pos + patternMap.at(text[pos+m-1]);
    else 
      pos = pos + m;
  }
  return output;
}