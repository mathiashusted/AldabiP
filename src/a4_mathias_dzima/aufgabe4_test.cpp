#include <iostream>
#include <string>
#include <algorithm>
#include <future>
#include <chrono>
#include <omp.h>

#include "BLAST_Neighborhood.hpp"
#include "a4_util.h"

using namespace std;

// global scoring matrix; do not do this at home :/ (at least read what a singleton is, and why they are bad)
ScoreMatrix m;

/// test equality of two NHResult objects
bool operator==(const NHResult& left, const NHResult& other)
{
  if (left.infix != other.infix || left.neighbors.size() != other.neighbors.size()) return false;
  for (unsigned i = 0; i < left.neighbors.size(); ++i)
  {
    if (left.neighbors[i] != other.neighbors[i]) return false;
  }
  return true;
}


void report(int& points, int(fn)(), string testname)
{
  std::cout << "##########################\ntest function '" << testname << "' ...\n";
  int new_score(0);
  try {
    new_score = fn(); // call the function
  }
  catch (...)
  {
    std::cout << "Error: test function '" << testname << "' threw unexpected exception\n";
  }
  points += new_score;
  std::cout << "Test: '" << testname << "' " << (new_score > 0 ? "success" : "failed") << "\n"
            << "Intermediate points: " << points << "\n\n";
  return;
}

inline bool file_exists_test (const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

int test_simple()
{
  BLAST_Neighborhood nh;
  vector<NHResult> r = nh.generateNeighborhood("AAHILNMY", m, 3, 14, 1);

  // test wordsize 3
  vector<NHResult> expect = { { "AAH", { {"AAH", 16} } },
                              { "AHI",{ { "AHI", 16 },{ "AHL", 14 },{ "AHV", 15 } } },
                              { "HIL",{ { "HII", 14 },{ "HIL", 16 },{ "HIM", 14 },{ "HLL", 14 },{ "HVL", 15 } } },
                              { "ILN",{ { "ILN", 14 } } },
                              { "LNM",{ { "LNM", 15 } } },
                              { "NMY",{ { "NIY", 14 },{ "NLY", 15 },{ "NMF", 14 },{ "NMY", 18 },{ "NVY", 14 } } },
  } ;
  if (r.size() != expect.size()) {
    std::cout << "Error: Result size is: " << r.size() << ". Expected: " << expect.size() << "\n";
    return 0;
  }
  if (!(r == expect))
  {
    std::cout << "Error: Result differs from expected\n";
    return 0;
  }

  // test wordsize 4
  vector<NHResult> r2 = nh.generateNeighborhood("AAHILNA", m, 4, 20, 1);
  vector<NHResult> expect2 = { { "AAHI",{ { "AAHI", 20 } } },
                               { "AHIL",{ { "AHIL", 20 } } },
                               { "HILN",{ { "HIIN", 20 },{ "HILN", 22 },{ "HIMN", 20 },{ "HLLN", 20 },{ "HVLN", 21 } } },
                               { "ILNA",{} } };
  if (r2.size() != expect2.size()) {
    std::cout << "Error: Result2 size is: " << r2.size() << ". Expected: " << expect2.size() << "\n";
    return 0;
  }
  if (!(r2 == expect2))
  {
    std::cout << "Error: Result2 differs from expected2\n";
    return 0;
  }

  // test multiple substitutions and last AA (=Y)
  vector<NHResult> r3 = nh.generateNeighborhood("YYYY", m, 4, 16, 1);
  NHResult expect3 = { "YYYY", { {"AYYY", 19}, {"CYYY", 19}, {"DYYY", 18}, {"EYYY", 19}, {"FFFY", 16}, {"FFYF", 16} } }; // there is more ...
  if (r3[0].neighbors.size() != 195) {
    std::cout << "Error: Result3 size is: " << r3[0].neighbors.size() << ". Expected: 195\n";
    return 0;
  }
  NHResult r3_slice = r3[0];
  r3_slice.neighbors.resize(6); // shrink to first 6
  if (!(r3_slice == expect3))
  {
    std::cout << "Error: Result3 differs from expected3\n";
    return 0;
  }

  return 7;
}

int test_highScoreThreshold()
{
  BLAST_Neighborhood nh;
  vector<NHResult> r = nh.generateNeighborhood("AAAA", m, 2, 1400, 1);
  if (r.size() != 3) return 0;
  for (auto a : r)
  { // all infixes are "AA", but have no neighbors
    if (a.infix != "AA" || !a.neighbors.empty()) return 0;
  }
  return 1;
}


int test_cornerCases()
{
  BLAST_Neighborhood nh;
  vector<NHResult> r = nh.generateNeighborhood("AAHILNMY", m, 10, 14, 1);
  if (!r.empty()) return 0;
  
  try { // should throw
    vector<NHResult> r = nh.generateNeighborhood("AAHILNMY", m, 2, 4, -1);
    return 0;
  }
  catch (...) {}
  return 1;
}



#if defined(_MSC_VER) || defined(__MINGW32__)
#define FILESEP ""
#define FILEEXT ".exe"
#else
#define FILESEP "./"
#define FILEEXT ""
#endif

int test_main()
{
  string filename = string(FILESEP) + string("aufgabe4_main") + string(FILEEXT);
  std::cout << "calling executable: '" << filename << "' (expecting error code)\n";
  if (!file_exists_test(filename))
  {
    std::cerr << "Executable '" << filename << "' not found in current directory. Is it compiled and is working directory correct?\n";
    return 0;
  }
  int r = system(filename.c_str());
  if (r == 0) {
    std::cout << "Error: './aufgabe4_main' returned with zero return code\n";
	  return 0;
  } 
  std::cout << "Success: './aufgabe4_main' returned with non-zero return code\n";
  
  std::cout << "\ncalling executable: ./aufgabe4_main   (expecting success)\n";
  r = system ((filename + string(" AAHILNMY blosum62 10 14 1")).c_str());
  if (r != 0) {
	  std::cout << "Error: './aufgabe4_main AAHILNMY blosum62 10 14 1' returned with non-zero return code\n";
    return 0;
  } 
  std::cout << "Success: './aufgabe4_main AAHILNMY blosum62 10 14 1' returned with zero return code\n";
  
  return 1;
}

int test_BONUS()
{
  BLAST_Neighborhood nh;
  double t_wall = -omp_get_wtime();
  vector<NHResult> r1 = nh.generateNeighborhood("AAHILNAAAHILNAAAHILNAAAHILNA", m, 4, -100, 1);
  t_wall += omp_get_wtime(); // Wall clock time
  std::cout << "Thread test: " << t_wall << " (1 Thread)\n";

  double t_wall2 = -omp_get_wtime();
  vector<NHResult> r2 = nh.generateNeighborhood("AAHILNAAAHILNAAAHILNAAAHILNA", m, 4, -100, 3);
  t_wall2 += omp_get_wtime(); // Wall clock time
  std::cout << "Thread test: " << t_wall2 << " (3 Threads)\n";

  // should be twice as fast... roughly...
  double ratio = t_wall / t_wall2;
  bool pass_time = ratio > 1.5;
  std::cout << " --> Ratio: " << ratio << " (yours) > 1.5 (minimal) ? "  << (pass_time ? "YES :)" : "NO :/") << "\n";
   
  // results actually need to be the same
  bool equality = (r1 == r2);
  if (!equality) std::cout << "Results are not identical! Some race condition occured?\n";

  return (pass_time && equality) ? 4 : 0;
}

int main()
{
  try {
    m.load("blosum62");
  }
  catch (IOException&)
  {
    try {
      m.load("../../blosum62");
    }
    catch (IOException&)
    {
      throw;
    }
  }

  int points = 0;

  std::cout << "Testing ... (remember: if the test crashes (e.g. segfault), you get 0 points) " << points << "\n\n";

  report(points, &test_simple, "test_simple: AAHILNMY blosum62 3 14 1   &&   AAHILNA blosum62 4 20 1");
  report(points, &test_highScoreThreshold, "test_highScoreThreshold: AAAA, m, 2, 1400, 1");
  report(points, &test_cornerCases, "test_cornerCases: ....");
  report(points, &test_main, "test_main: ./aufgabe4_main & ./aufgabe4_main AAHILNMY blosum62 10, 14, -1");

  // bonus points
  report(points, &test_BONUS, "test_BONUS OpenMP: AAHILNAAAHILNAAAHILNAAAHILNA blosum62 4 -100 <threads>");
  
  std::cout << "Final score: " << points << "\n\n";

  return 100 + points;
}
