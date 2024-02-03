#include "PDA.hpp"

void PDA::initComplementMap() {
  complementMap['a'] = 'u';
  complementMap['u'] = 'a';
  complementMap['c'] = 'g';
  complementMap['g'] = 'c';
}

PDA::PDA(const Language l)
    : currentState(State::IN_PROGRESS), currentLanguage(l) {
  // Implement production rules for the language
  // Add production rules to the stack
  if (currentLanguage == Language::HAIRPIN) {
    initComplementMap();
  } else if (currentLanguage == Language::BRACKETS) {
    // We don't need to implement this for now
  }
}

PDA::State PDA::next(const char a) {
  // Maintain FAIL state
  if (currentState == State::FAIL)
    return State::FAIL;

  // Check for the end
  if (a == '$') {
    currentState = stack.empty() ? State::SUCCESS : State::FAIL;
    return currentState;
  }

  // Check for complementarity
  auto complement = complementMap.find(a);
  if (complement != complementMap.end()) {
    if (!stack.empty() && stack.top() == a) {
      // Pop if the current character matches the top (indicating the start of
      // the loop or mirrored sequence)
      stack.pop();
    } else {
      // Otherwise, push the complement onto the stack
      stack.push(complement->second);
    }
  } else {
    // Here should be implemented the last production rules
  }

  currentState = State::IN_PROGRESS;
  return currentState;
}

void PDA::reset() {
  std::stack<char>().swap(stack);    // Clear the stack
  currentState = State::IN_PROGRESS; // Reset state
}
