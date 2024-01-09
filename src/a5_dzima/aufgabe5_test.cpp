#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <algorithm>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <omp.h>

#include "QGramIndex.hpp"
#include "a5_util.hpp"

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
  std::cout << "Test: '" << testname << "' [" << new_score.first << "/" << new_score.second  << "]\n"
            << "current points: " << points << "\n\n";
  return;
}


TRT test_getHits()
{
  int points(0);
  // std::vector<uint32_t> getHits(const uint32_t h) const;
  // @throws std::invalid_argument("Invalid hash!"); if h is outside of valid hash values
  std::string s = "GATTTACAGATTTACA";
  QGramIndex qgi(s, 3);

  auto r = qgi.getHits(0); // = AAA
  auto r2 = qgi.getHits(62); // = TTG
  if (r.empty() && r2.empty()) ++points;

  r = qgi.getHits(35); // = GAT  2*4^2 + 0 + 3
  if (r.size() == 2 && r[0] == 8 && r[1] == 0) points += 2;

  // test hits to largest Q-gram (usually special bounds checking required)
  r = qgi.getHits(63); // = TTT
  if (r.size() == 2 && r[0] == 10 && r[1] == 2) ++points;                                      
  
  return make_pair(points, 4);
}

TRT test_hash()
{
  int points = 0;
  //uint32_t hash(const std::string& qgram) const;
  std::string s = "AAAAAAAAAAAAAAAAA";
  QGramIndex qgi(s, 2);

  if (qgi.hash("AG") == ordValue('G') + 4 * ordValue('A'))
  {
    ++points;
  }
  if (qgi.hash("TT") == ordValue('T') + 4 * ordValue('T'))
  {
    ++points;
  }
  
  QGramIndex qgi2(s, 12);

  if (qgi2.hash(string(12, 'A')) == 0)
  {
    ++points;
  }
  std::cout << qgi2.hash(string(12, 'T')) << "\n" << ((1 << (2 * 12 + 1)) - 1) << "\n";
  if (qgi2.hash(string(12, 'T')) == ((1 << (2*12))-1))
  {
    ++points;
  }

  return make_pair(points / 4, 1);
}

TRT test_hashNext()
{
  int points = 0;
  //uint32_t hashNext(const uint32_t prev_hash, const char new_pos) const;          
  std::string s = "AAAAAAAAAAAAAAA";
  QGramIndex qgi(s, 2);
  uint32_t h = qgi.hashNext(qgi.hash("GG"), 'T');
  // "GT"
  if (h == ordValue('T') + 4 * ordValue('G'))
  {
    ++points;
  }

  if (qgi.hashNext(qgi.hash("TT"), 'T') == ordValue('T') + 4 * ordValue('T'))
  {
    ++points;
  }

  QGramIndex qgi2(s, 12);

  if (qgi2.hashNext(0, 'A') == 0)
  {
    ++points;
  }
  uint32_t hash_allT = ((1 << (2 * 12)) - 1);
  if (qgi2.hashNext(hash_allT, 'A') == hash_allT - 3)
  {
    ++points;
  }

  return make_pair(points / 2, 2);
}

TRT test_q_and_text()
{
  int points = 0;
  string text = "AAAAAATTTGGGGGGGGGG";
  int q = 4;
  QGramIndex qgi(text, q);
  if (qgi.getQ() == q) ++points;
  if (qgi.getText() == text) ++points; // check before
  text += "A"; // change text; make sure it is stored as a reference internally
  if (qgi.getText() == text) ++points; // and after
    

  return make_pair(points / 3, 1);
}

TRT test_exceptions()
{
  int cases = 0;
  std::string s = "AAAAAA";
  QGramIndex qgi(s, 2);
  try {
    // should throw (wrong q-gram length)
    uint32_t h = qgi.hash("A");
    std::cout << "Wrong.. hash(\"A\")=" << h << " should have thrown...\n";
  }
  catch (std::invalid_argument&)
  {
    ++cases;
  }
  catch (...)
  { // in case wrong exception is thrown
  }

  try {
    // should throw (wrong q-gram length)
    QGramIndex qgi("AAAAAA", 0);
  }
  catch (std::invalid_argument&)
  {
    try {
      // should throw (wrong q-gram length)
      QGramIndex qgi("AAAAAA", 14);
    }
    catch (std::invalid_argument&)
    {
      ++cases;
    }
    catch (...)
    { // in case wrong exception is thrown
    }
  }
  catch (...)
  { // in case wrong exception is thrown
  }

  // @throws std::invalid_argument("Invalid hash!"); if h is outside of valid hash values
  // std::vector<uint32_t> getHits(const uint32_t h) const;
  s = "GATTACAGATTACA";
  QGramIndex qgi2(s, 2);
  try
  {
    auto r = qgi2.getHits( 16 ); // check 16; max allowed is 15
  }
  catch (std::invalid_argument&)
  {
    ++cases;
  }
  catch (...)
  { // in case wrong exception is thrown
  }

  return make_pair((int)std::round(cases * 2 / 3), 2); // 1 case: 1pt; 2 cases: 1 pt; 3 cases 2 pts;
}

TRT test_BONUS(const string& filename)
{
  ifstream s(filename + "/random100M.text");
  if (!s.good())
  {
    std::cerr << "Could not read " << filename << "\n";
    exit(-1);
  }
  string text;
  double t1, t2; //, d;
  //std::cin >> d;
  getline(s, text);
  //text += text; // double the text


  // benchmark
  string search = "GAT";
  std::vector<uint32_t> h1, h2;
  //std::cin >> d;
  {
    omp_set_num_threads(1);
    t1 = -omp_get_wtime();
    QGramIndex idx(text, (uint8_t)search.size());
    t1 += omp_get_wtime();
    h1 = idx.getHits(idx.hash(search));
    std::cout << "Thread test: " << t1 << " sec (1 Thread)\n";
    //std::cin >> d;
  }
  {
    int threads = 3;
    omp_set_num_threads(threads);
    t2 = -omp_get_wtime();
    QGramIndex idx(text, (uint8_t)search.size());
    t2 += omp_get_wtime();
    h2 = idx.getHits(idx.hash(search));
    std::cout << "Thread test: " << t2 << " sec (" << threads << " Threads)\n";
  }
  //std::cin >> d;
  // test results are equal:
  // sort hits since partial cumulative sum leads to different orders (usually its reversed)
  std::cout << "Sorting hits (" << h2.size() << ")\n";
  sort(h2.begin(), h2.end(), std::greater<uint32_t>());
  bool eq = (h1 == h2);
  std::cout << (eq ? "Results equal" : "Results differ :/");

  // should be twice as fast... roughly...
  double ratio = t1 / t2;
  bool pass_time = ratio > 1.5;
  std::cout << " --> Ratio: " << ratio << " (yours) > 1.5 (minimal) ? " << (pass_time ? "YES :)" : "NO :/") << "\n";


  return make_pair((pass_time & eq) * 5, 5);
}


int main(int argc, const char * argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <folder-to-genome-files (where random100M.text lives)>\n"
              << "  e.g. " << argv[0] << " c:\\temp\\" << "\n";
    return -1;
  }
  string file = argv[1];
  

  int points = 0;
  
  // even if OMP is supported, do all the tests without parallelization (since hits might be in mangled order due to partial sums)
  omp_set_num_threads(1);

  report(points, &test_exceptions, "test_exceptions");  // 2
  report(points, &test_hash, "test_hash");              // 1
  report(points, &test_hashNext, "test_hashNext");      // 2
  report(points, &test_q_and_text, "test_q_and_text");  // 1
  report(points, &test_getHits, "test_getHits");        // 4

  // bonus points
  report(points, [&file]() { return test_BONUS(file);}, "test_BONUS OpenMP");

  std::cout << "Final score: " << points << "\n\n";

  return 100 + points;
}