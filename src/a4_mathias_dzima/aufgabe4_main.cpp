#include "BLAST_Neighborhood.hpp"
#include <omp.h>

int main(int argc, char** argv) {
    std::string query;
    ScoreMatrix matrix;
    int word_size;
    int score_threshold;
    int threads;

    if (argc != 6) {
        std::cout << "Usage: ./aufgabe4_main <SEQ> <SCORE_MATRIX> <WORD_SIZE> <THRESHOLD> <THREADS>\n";
        return 1;
    }
    query = argv[1];
    word_size = std::stoi(argv[3]);
    score_threshold = std::stoi(argv[4]);
    threads = std::stoi(argv[5]);

    try {
        double start_time = omp_get_wtime();

        BLAST_Neighborhood blast;

        matrix.load(argv[2]);

        auto output = blast.generateNeighborhood(query, matrix, word_size, score_threshold, threads);

        for (const auto& result : output) {
            std::cout << result.infix << ": ";
            for (const auto& neighbor : result.neighbors) {
                std::cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
            }
            std::cout << "\n";
        }
        double end_time = omp_get_wtime(); // End timing
        std::cout << "time: " << (end_time - start_time) << "s" << "\n";
    } catch (const IOException &e) {
        std::cerr << "IO Error: " << e.what() << std::endl;
        return e.getErrorState();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}