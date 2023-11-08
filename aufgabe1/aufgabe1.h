// WARNING:
// only ADD things to this file. Do NOT modify, move or delete existing function declarations!
//

#pragma once

#include <string>
#include <vector>

// additional includes (should you need them), go here
//  ...
#include <unordered_map>
#include <cstdint>


class Horspool
{
public:

  /**
   * @brief Preprocess (=generate lookup table) and store the pattern 
   * @param pat The pattern to search later on.
  */
  void setPattern(const std::string& pat);

  /**
   * @brief Return the currently set pattern (or empty if not set yet)
   * @return The pattern
  */
  const std::string& getPattern() const;

  /**
   * @brief Get all hits of pattern (previously set using setPattern()) in @p text.
   * @param text The haystack/text to search
   * @return Indices of hits (0-based) of pattern in the text
  */
  std::vector<size_t> getHits(const std::string& text) const;


protected:
  /**
   * @brief Use the lookup table created in 'setPattern' to obtain the maximum Horspool shift distance
            given the last character of the current alignment in the text
 
   * @param last_char The last character of the text infix from the current alignment
   * @return The shift distance
  */
  uint32_t getShift_(const char last_char) const;
  
  /**
   * @brief Internal check, which should be called whenever you test an alignment at text position @p text_pos.
   *
   * This method should be called within `getHits()`, whenever the pattern is tested against an infix of the text.
   * It is only required for testing the shift offsets.
   * 
   * @param text_pos Position (0-based) in the text where comparison starts.
  */
  virtual void alignCheck_(const size_t text_pos) const {}; // leave this function empty. Just call it whenever you check an alignment.


  // your data members (and helper functions, if you need them), go here:
  //  ...
  // 
  std::string pattern;
  std::uint32_t maxShift;
  std::unordered_map<char, uint32_t> shiftMap;
};
