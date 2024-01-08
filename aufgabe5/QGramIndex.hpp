#pragma once

#include <vector>
#include <string>
// your includes here ... (should not be needed though)
#include <cstdint>

/**
   The Q-Gram-Index implementation using Counting Sort for SA Construction
   assuming DNA alphabet (|Sigma| = 4).
   
   The text may be up to 2^32 in length, i.e. uint32_t is used internally for suffix array and q-gram index.
   The maximal supported value for q is 13.
   
*/
class QGramIndex 
{
public:
    // AS ALWAYS: DO NOT CHANGE THE INTERFACE!

    /**
     @brief Constructor: build up a q-gram index from @p text.

     Internally, @p text is stored as const reference (const std::string& ), 
     i.e. make sure the text does not go out of scope as long as the class object is used.
     
     The index is build immediately.
    
     @param text The sequence (genome, ...) to be indexed
     @param q Length of q-gram (from 1 to 13).
     @throws std::invalid_argument("Invalid q!") unless: 1 <= q <= 13
    */
    QGramIndex(const std::string& text, const uint8_t q);

    /**
      @brief Returns the text.
      
      @return the full text
    */
    const std::string& getText() const;
    
    /**
      @brief Returns a vector of indices into text, where the q-gram with hash @p h occurs.
      
      The vector might be empty, if the q-gram does not exist, i.e. empty range within the suffix array.
      The hash value must be within [0...|SIGMA|^q), otherwise an exception is thrown.
      
      @note The indices are in reverse order of occurrence in the text, or might be even "unordered"
      when OpenMP multithreading is used.    
      
      @param h Hash value of the q-gram
      @return Vector of hits (indices into text)
      @throws std::invalid_argument("Invalid hash!"); if h is outside of valid hash values
    */
    std::vector<uint32_t> getHits(const uint32_t h) const;
    
    /**
      @brief Get the length of q-grams, i.e. 'q'.
    */
    uint8_t getQ() const;
    
    /**
      @brief Compute a full hash for a given q-gram.
      
      Warning: use this function only when absolutely needed! Prefer hashNext() when possible!
      
      @param qgram A q-gram; must have size 'q'
      @return hash value of @p qgram
      @throws std::invalid_argument("Invalid q-gram. Wrong length!"); if qgram.size() != q
      
    */
    uint32_t hash(const std::string& qgram) const;
    
    /**
       @brief Returns the next rolling hash for given new character and previous hash (of previous q-gram).
       
       The first character going out-of-scope is removed internally by bitmasking (i.e. no need to specify it).
       
       @param prev_hash Previous hash value
       @param new_pos new character (last position of current q-gram)
       @return The updated hash value
    */
    uint32_t hashNext(const uint32_t prev_hash, const char new_pos) const;

private:
   // YOUR PRIVATE MEMBER FUNCTIONS and VARIABLES HERE
  
  const uint8_t q_length;
  const uint8_t alphabet_length = 4; // Our alphabet will always consist of {A, C, G, T}
  const uint8_t bit_shift_value = 2; // Valid for as long as there exists a k, so that 2^k = alphabet_length
  uint32_t mask;
  const std::string& search_text;
  std::vector<uint32_t> suffix_array;
  std::vector<uint32_t> dir;
  size_t search_text_length;

};

