#include "BLAST_Neighborhood.hpp"
#include "a4_util.h"
#include <fstream>
#include <iostream>
#include <omp.h>
#include <sstream>

int main(int argc, char *argv[]) {
  if (argc != 6) {
    std::cerr << "Usage: " << argv[0]
              << " <sequence> <scoring matrix file> <word size> <score "
                 "threshold> <threads>\n";
    return 1;
  }

  std::string sequence = argv[1];
  std::string matrixFile = argv[2];
  int wordSize = std::stoi(argv[3]);
  int scoreThreshold = std::stoi(argv[4]);
  int threads = std::stoi(argv[5]);
  try {
    double start_time = omp_get_wtime(); // Start timing

    // Generate neighborhood
    ScoreMatrix matrix;
    matrix.load(matrixFile.c_str());

    BLAST_Neighborhood blastNeighborhood;
    auto neighborhoodResults = blastNeighborhood.generateNeighborhood(
        sequence, matrix, wordSize, scoreThreshold, threads);

    // Output results
    for (const auto &result : neighborhoodResults) {
      std::cout << "Infix: " << result.infix << "\nNeighbors:\n";
      for (const auto &neighbor : result.neighbors) {
        std::cout << "\t" << neighbor.first << ", " << neighbor.second << "\n";
      }
    }

    double end_time = omp_get_wtime(); // End timing
    std::cout << "time: " << (end_time - start_time) << "s" << std::endl;

  } catch (const IOException &e) {
    std::cerr << "IO Error: " << e.what() << std::endl;
    return e.getErrorState();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 2;
  }

  return 0;
}
