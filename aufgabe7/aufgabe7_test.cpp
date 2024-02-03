#include <iostream>
#include <utility>
#include "PDA.hpp"

using namespace std;

typedef int opt_score;
typedef int has_score;
typedef pair<has_score, opt_score> TRT; // test return-type

template <typename FUNC>
void report(int& points, FUNC fn, string testname)
{
  std::cout << "##########################\ntest function '" << testname << "' ...\n";
  TRT new_score;
  try {
    new_score = fn(); // call the function
  }
  catch (...)
  {
    std::cout << "Error: test function '" << testname << "' threw unexpected exception\n";
  }
  points += new_score.first;
  std::cout << "Test: '" << testname << "' [" << new_score.first << "/" << new_score.second << "]\n"
            << "current points: " << points << "\n\n";
  return;
}


TRT test_hairpin()
{
  int points(0);
  
  PDA p1(PDA::Language::HAIRPIN);
  if (p1.next('A') == PDA::State::FAIL) ++points;
  else
  {
    std::cerr << "'A' should return FAIL since not in alphabet {a,c,g,u}, but did not.\n";
  }
  p1.reset();
  if (p1.next('t') == PDA::State::FAIL) ++points;
  else
  {
    std::cerr << "'t' should return FAIL since not in alphabet {a,c,g,u}, but did not.\n";
  }
  points /= 2; // one point for the two tests above
  
  // check immediate '$'
  p1.reset();
  if (p1.next('$') == PDA::State::FAIL) ++points;
  else std::cerr << "'$' should return FAIL, but did not.\n";
  
  p1.reset();
  p1.next('g'); // change internal state of stack ... so reset() is forced to clear it
  
  p1.reset();
  PDA::State s;
  for (auto e : string("gccgcaaggc"))
  {
    if ((s = p1.next(e)) != PDA::State::IN_PROGRESS) break;
  }
  if (s == PDA::State::IN_PROGRESS) points += 2;
  else std::cerr << "'gccgcaaggc' should return IN_PROGRESS for every iteration, but did not.\n";
  if ((s = p1.next('$')) == PDA::State::SUCCESS) ++points;
  else std::cerr << "'gccgcaaggc$' should return SUCCESS, but did not.\n";

  // 2nd motif with partial sequence
  p1.reset();
  for (auto e : string("aucgaaagau"))
  {
    if ((s = p1.next(e)) != PDA::State::IN_PROGRESS) break;
  }
  if (s == PDA::State::IN_PROGRESS) points += 2;
  else std::cerr << "'aucgaaagau' should return IN_PROGRESS for every iteration, but did not.\n";
  if ((s = p1.next('$')) == PDA::State::SUCCESS) ++points;
  else std::cerr << "'aucgaaagau$' should return SUCCESS, but did not.\n";
  
  p1.reset();
  for (auto e : string("gccgc$"))
  {
    s = p1.next(e);
  }
  if (s == PDA::State::FAIL)  points += 1;
  else std::cerr << "'gccgc$' should return FAIL, but did not.\n";
  if ((s = p1.next('$')) == PDA::State::FAIL) ++points;
  else std::cerr << "'gccgc$$' should return FAIL, but did not.\n";

  
  return make_pair(points, 10);
}

TRT test_bonus()
{
  int points(0);

  PDA p1(PDA::Language::BRACKETS);
  PDA::State s;

  // expect: SUCCESS
  p1.reset();
  for (auto e : string("()"))
  {
    if ((s = p1.next(e)) != PDA::State::IN_PROGRESS) break;
  }
  if (s != PDA::State::IN_PROGRESS) std::cerr << "'()' should return IN_PROGRESS for every iteration, but did not.\n";
  else if ((s = p1.next('$')) == PDA::State::SUCCESS) ++points;
  else std::cerr << "'()$' should return SUCCESS, but did not.\n";

  // expect: SUCCESS
  p1.reset();
  for (auto e : string("()((()))"))
  {
    if ((s = p1.next(e)) != PDA::State::IN_PROGRESS) break;
  }
  if (s != PDA::State::IN_PROGRESS) std::cerr << "'()((()))' should return IN_PROGRESS for every iteration, but did not.\n";
  else if ((s = p1.next('$')) == PDA::State::SUCCESS) ++points;
  else std::cerr << "'()((()))$' should return SUCCESS, but did not.\n";

  // only test FAIL cases if SUCCESSful ones were ok (otherwise everyone would get points)
  if (points == 2)
  {
    int p_fail{0};
    // expect: FAIL
    p1.reset();
    if (p1.next('A') == PDA::State::FAIL) ++p_fail;
    else
    {
      std::cerr << "'A' should return FAIL since not in alphabet, but did not.\n";
    }

    p1.reset();
    for (auto e : string("(()"))
    {
      if ((s = p1.next(e)) != PDA::State::IN_PROGRESS) break;
    }
    if (s != PDA::State::IN_PROGRESS) std::cerr << "'(()' should return IN_PROGRESS for every iteration, but did not.\n";
    else if ((s = p1.next('$')) == PDA::State::FAIL) ++p_fail;
    else std::cerr << "'(()$' should return FAIL, but did not.\n";

    // expect: FAIL
    p1.reset();
    for (auto e : string(")("))
    {
      if ((s = p1.next(e)) != PDA::State::FAIL) break;
    }
    if (s != PDA::State::FAIL) std::cerr << "')(' should return FAIL for every iteration, but did not.\n";
    else if ((s = p1.next('$')) == PDA::State::FAIL) ++p_fail;
    else std::cerr << "'()$' should return FAIL, but did not.\n";
    points += p_fail / 3;
  }


  return make_pair(points, 3);
}


int main(int argc, const char * argv[])
{
  if (argc != 1)
  {
    std::cerr << "Usage: " << argv[0] << "\n";
    return -1;
  }
  std::cout << "Note: Compile using the Makefile (i.e. with AddressSanitizer) to make sure there are no hidden segfaults (0 points)\n\n";

  int points{0};

  report(points, &test_hairpin, "test_hairpin");
  report(points, &test_bonus, "test_bonus");
  
  std::cout << "Final score: " << points << "\n";

  return 100 + points;
}