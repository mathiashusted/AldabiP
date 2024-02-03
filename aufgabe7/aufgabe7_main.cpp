#include "PDA.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  // Check for exactly one input string
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_string>" << std::endl;
    return 1; // Exit with an error code
  }

  // Extract the input string from command line arguments
  std::string input = argv[1];

  // Create a PDA instance for hairpin recognition
  PDA pda(PDA::Language::HAIRPIN);

  // Process each character of the input string
  for (char c : input) {
    PDA::State state = pda.next(c);
    if (state == PDA::State::FAIL) {
      std::cout << "Input sequence is not valid." << std::endl;
      return 1; // Exit with an error code
    }
  }

  // Check for the final state by simulating the end of the sequence
  PDA::State finalState = pda.next('$');

  // Output the result based on the final state
  if (finalState == PDA::State::SUCCESS) {
    std::cout << "Input sequence is valid." << std::endl;
  } else {
    std::cout << "Input sequence is not valid." << std::endl;
  }

  return 0; // Success
}
