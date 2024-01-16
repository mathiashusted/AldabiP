#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <algorithm>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <sstream>

#include "ACTrie.hpp"


using namespace std;
typedef int opt_score;
typedef int has_score;
typedef pair<has_score, opt_score> TRT; // test return-type


template <typename FUNC>
void report(int& points, FUNC fn, const string& testname)
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

// 1 point
TRT test_exceptions()
{
  int cases = 0;
  try {
    // should throw (>=1 sequences required)
    ACTrie c1({ });
    std::cout << "Wrong number of patterns to Ctor .. should have thrown...\n";
  }
  catch (std::logic_error&)
  {
    ++cases;
  }
  catch (...)
  { // in case wrong exception is thrown
  }
  
  try {
    // should throw due to wrong alphabet
    ACTrie c2({ "ABCD", "D", "ABcD" });
    std::cout << "Wrong call to Ctor .. should have thrown...\n";
  }
  catch (std::exception&)
  {
    ++cases;
  }
  catch (...)
  { // in case wrong exception is thrown
  }
  try {
    // should throw due to wrong alphabet
    ACTrie c3({ "?AB", "ABcD" });
    std::cout << "Wrong call to Ctor .. should have thrown...\n";
  }
  catch (std::exception&)
  {
    ++cases;
  }
  catch (...)
  { // in case wrong exception is thrown
  }

  return make_pair(cases/3, 1);
}

// 3 points
TRT test_treeStructure()
{
  int points(0);
  
  auto compare = [](int& points, const string& got, const string& exp, std::vector<std::string> needles) {
    std::cout << "Comparing trees\n  yours: " << got << "\n  expected: " << exp << "\n";
    std::cout << "  for patterns: ";
    for (const auto& n : needles) std::cout << n << " ";
    std::cout << "\n";
    if (got == exp)
    {
      std::cout << " --> matches (1 point)\n";
      points += 1;
    } else {
      std::cout << " --> incorrect (no points)\n";
    }
  };
  
  std::vector<std::string> needles;
  
  // simple
  needles = { "AT", "TA", "A" };
  ACTrie c1(needles);
  compare(points, c1.getTree(), "0(A(T))(T(A))", needles);
  
  // with duplicates and linear
  needles = { "ZZZZ", "ZZ", "ZZZZ" };
  ACTrie c2(needles);
  compare(points, c2.getTree(), "0(Z(Z(Z(Z))))", needles);

  // a lot of branches
  needles = { "AT", "B", "C", "D", "E", "FA", "G" };
  ACTrie c3(needles);
  compare(points, c3.getTree(), "0(A(T))(B)(C)(D)(E)(F(A))(G)", needles);


  return make_pair(points, 3);
}

// 6 points
TRT test_next()
{
  int points(0);
  {
    /////////////////////////////////////////
    // simply query
    ACTrie c1({ "AT", "TA", "A" });
    c1.setQuery("GATTACA");
    
    const std::vector<std::vector<Hit>> expected { 
                                              { {2, 1} },
                                              { {0, 1} },
                                              { {1, 3}, {2, 4} },
                                              { {2, 6} } 
                                            };
    std::vector<std::vector<Hit>> got;
    std::vector<Hit> hits{ {6, 4} , { 10, 10} }; // some garbage...
    while (c1.next(hits)) got.push_back(hits);
    if (got == expected)
    {
      points += 2;
    } else std::cout << "failed: simply query\n";
  }
  
  /////////////////////////////////////////
  // duplicate patterns
  {
    std::vector<std::string> needles(500, "AHOCORASICK");
    needles.push_back("MYAHORCORASICK");
    needles.push_back("CORASICK");
    ACTrie c2( needles );
    c2.setQuery("AAAHOCORASICK");
    std::vector<Hit> hits{ {6, 4} , { 10, 10} }; // some garbage...
    if (!c2.next(hits) || (hits.size() != 501))
    {
      std::cout << "failed to report hits\n";
    }
    else
    {
      bool all_ok { true };
      for (size_t i = 0; i < 500; ++i)
      {
        if (hits[i].index != i || hits[i].pos != 2) all_ok = false;
      }
      if (hits[500].index != 501 || hits[500].pos != 5) all_ok = false;
      if (all_ok) points += 1; else std::cout << "failed: duplicate patterns\n";
    }
  }
  
  ///////////////////////////////////////////
  // repetitive patterns
  {   
    std::vector<std::string> needles{"A","B","C","D","E", "AB", "ABC", "ABE", "BABC"};
    ACTrie c2( needles );
    c2.setQuery("BABCZBABC");
    const std::vector<std::vector<Hit>> expected { 
                                              { {1, 0} },
                                              { {0, 1} },
                                              { {5, 1}, {1, 2} },
                                              { {8, 0}, {6, 1}, {2, 3} },
                                              { {1, 5} },
                                              { {0, 6} },
                                              { {5, 6}, {1, 7} },
                                              { {8, 5}, {6, 6}, {2, 8} }
                                            };
    std::vector<std::vector<Hit>> got;
    std::vector<Hit> hits{ {6, 4} , { 10, 10} }; // some garbage...
    while (c2.next(hits)) got.push_back(hits);
    if (got == expected)
    {
      points += 1;
    } else std::cout << "failed: repetitive patterns\n";
    
  }
  
  ////////////////////////////////////////////
  // reset Query
  {
    ACTrie c1({ "AT", "TA", "A" });
    c1.setQuery("GATTACA");
    
    const std::vector<std::vector<Hit>> expected { 
                                              { {2, 1} },
                                              { {0, 1} },
                                              { {1, 3}, {2, 4} },
                                              { {2, 6} } 
                                            };
    std::vector<Hit> hits{ {6, 4} , { 10, 10} }; // some garbage...
    while (c1.next(hits));
    std::vector<std::vector<Hit>> got;
    c1.setQuery("GATTACA");
    while (c1.next(hits)) got.push_back(hits);
    if (got == expected)
    {
      points += 1;
    } else std::cout << "failed: reset query\n";
  }
  ////////////////////////////////////////////
  // long query
  {
    ACTrie c1({ "AT", "TA", "A", "BTB", "C", "BTBB" });
    const size_t len {10000000};
    std::string garbage(len, 'Z');
    std::string q = garbage + "BTB" + garbage + "BTB";
    c1.setQuery(q);
    const std::vector<std::vector<Hit>> expected { 
                                              { {3, len } },
                                              { {3, 2*len+3} }
                                            };
    std::vector<std::vector<Hit>> got;
    std::vector<Hit> hits{ {6, 4} , { 10, 10} }; // some garbage...
    auto start = std::chrono::high_resolution_clock::now();
    // Code to time here...
    if (c1.next(hits)) got.push_back(hits);
    auto mid = std::chrono::high_resolution_clock::now();
    if (c1.next(hits)) got.push_back(hits);
    auto last = std::chrono::high_resolution_clock::now();
    double t1 = std::chrono::duration_cast<std::chrono::duration<float>>(mid - start).count(); // time for finding first hit
    double t2 = std::chrono::duration_cast<std::chrono::duration<float>>(last - mid).count();  // time for finding second hit
    std::cout << "timing (ratio): " << t1 << " vs. " << t2 << " s (" << t1/t2 << ")\n";
    // t1 should roughly equal t2
    if (got == expected && !c1.next(hits) && (t1/t2 > 0.5) && (t2/t1 > 0.5))
    {
      points += 1;
    } else std::cout << "failed: long query\n";
  }
  return make_pair(points, 6);
}

int main(int argc, const char * argv[])
{
  if (argc != 1)
  {
    std::cerr << "Usage: " << argv[0] << "\n";
    return -1;
  }
  std::cout << "Note: Compile using the Makefile (i.e. with AddressSanitizer) to make sure there are no hidden segfaults (0 points)\n\n";
  std::cout << "Note: Your program must finish within 30 seconds on the test machine to avoid a timeout (the sample implementation takes around 1.5 seconds on this very machine).\n\n";

  int points = 0;

  report(points, &test_exceptions, "test_exceptions");
  report(points, &test_treeStructure, "test_treeStructure");
  report(points, &test_next, "test_next");
  
  std::cout << "Final score: " << points << "\n";

  return 100 + points;
}