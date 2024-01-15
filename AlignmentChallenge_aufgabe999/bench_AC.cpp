#include <string>
#include <fstream>
#include <iostream>
#include <omp.h>

#include "AlignmentChallenge.h"

void printUsage(const char* argv[])
{
  std::cerr << "Usage: " << argv[0] << " filename.fa [threads=1 match=3 mismatch=-1 gap=-2]" << std::endl;
}

int main(int argc, const char* argv[]) 
{
  if (!(argc == 6 || argc == 2))
  {
    printUsage(argv);
    return 1;
  }
  int match = 3, mismatch = -1, gap = -2, threads = 1;
  if (argc == 6)
  {
    try
    {
      threads = std::stoi(argv[2]);
      match = std::stoi(argv[3]);
      mismatch = std::stoi(argv[4]);
      gap = std::stoi(argv[5]);
    } 
    catch (...)
    {
      std::cerr << "Failed to stoi scores. Arguments 3~5 invalid." << std::endl;
      printUsage(argv);
      return 1;
    }
  }

  std::vector<std::string> seqs;
  std::ifstream f(argv[1]);
  if (!f)
  {
    std::cerr << "Could not open " << argv[1] << "!\n";
    printUsage(argv);
    return 1;
  }
  std::string s;
  while (f)
  {
    std::getline(f, s); // skip annotation
    std::getline(f, s); // the sequence
    if (!s.empty()) seqs.push_back(s);
  }
  std::cout << "# sequences " << seqs.size() << "\n";
  int64_t total_score(0);
  Alignment a(match, mismatch, gap);
  double t_start = omp_get_wtime();
  total_score = a.compute(seqs, threads);
  std::cout << "score: " << total_score << "\n"
            << "time (wall): " << omp_get_wtime() - t_start << " sec" << std::endl;
  
}