/**
 * Praktikum Algorithmen und Datenstrukturen
 * WS 2023/2024
 * 
 * Aufgabe 1
 * Horpool-Algorithmus
 * 
 * Authors: Dzmitry Hramyka, Mathias Husted 
*/

#include "aufgabe1.h"

void Horspool::setPattern(const std::string& pat) {
  this->pattern = pat;
  shiftMap.clear();
  uint32_t length = pattern.length();
  for (size_t i = 0; i < length - 1; i++) {
    shiftMap[pattern[i]] = length - i - 1;
  }
}

const std::string& Horspool::getPattern() const {
  static const std::string empty;
  return (this->pattern.empty()) ? empty : this->pattern;
}

/**
 * @brief Get the Shift object
 * Here we first check that the argument isn't out of range in our lookup table.
 * If it is out of range, then we return a default value of length of the pattern.
 * This is the behaviour we want because of the following 2 cases:
 *   1. The character isn't in the lookup table at all -> skip maximum.
 *      (... and avoid throwing an exception :D)
 *   2. The character's index+1 is the length of the table itself -> skip the 
 *      whole text.
*/
uint32_t Horspool::getShift_(const char last_char) const {
  return (shiftMap.find(last_char) != this->shiftMap.end()) ? shiftMap.at(last_char) : this->pattern.length();
}

std::vector<size_t> Horspool::getHits(const std::string& text) const
{
    // Check pattern, text correctness
    if (this->pattern.empty() || text.empty())
    {
        return {};
    }

    // Initialize variables
    uint32_t textPosition = 0;
    int32_t i;
    uint32_t patternLength = this->pattern.length();
    uint32_t textLength = text.length();
    // Reference to pattern for faster access (enabling caching)
    const std::string& patternRef = this->pattern;
    std::vector<size_t> output;

    while (textPosition <= (textLength - patternLength))
    {
        alignCheck_(textPosition);
        i = patternLength - 1;

        // Check if pattern mathces text
        while (i > 0 && (text[textPosition + i] == patternRef[i]))// || patternRef[i] == '?' || text[textPosition + i] == '?'))
        {
            --i;
        }
        // If all characters match, add position to output
        if ((i == 0) && (text[textPosition + i] == patternRef[i]))
        {
            output.push_back(textPosition);
        }

        // Shift text position
        textPosition += getShift_(text[textPosition + patternLength - 1]);
    }

    return output;
}