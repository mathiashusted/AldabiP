// compile with
// g++ -std=c++17 -Wall -pedantic -O3 -D_GLIBCXX_ASSERTIONS -g -fsanitize=address aufgabe2_bench.cpp aufgabe2.cpp -o aufgabe2_bench
//
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <ctime>

#include "aufgabe2.hpp"

using namespace std;


int main(int argc, const char* argv[])
{
    if (argc != 3)
    {
        std::cout << argv[0] << " <FASTA file> <# queries>\n"
                  << "  e.g. " << argv[0] << "Candidatus.Solibacter.usitatusEllin6076.fasta 1000000" << std::endl;
        return 1;
    }
    std::ifstream  is(argv[1]);
    if (!is.good())
    { // read file instead of treating as text
      std::cerr << "Cannot open file '" << argv[1] << "'\n";
      return 1;
    }
    
    string tmp, text;
    getline(is, tmp);  // header (optional)
    if (tmp[0] != '>') text = tmp;
    while (getline(is, tmp))
    {
      text += tmp;
    }
    std::cout << "Text length: " << text.size() << " starting with " << text.substr(0, 100) << "...\n";

    int query_count = atoi(argv[2]);
    std::cout << "Performing " << query_count << " queries\n";
    if (query_count < 1)
    {
      std::cerr << "Number of queries must be positive!\n";
      return 1;
    }
    
    vector<uint32_t> sa;
    vector<uint32_t> hits;

    double t_construct = (double) clock () / (double) CLOCKS_PER_SEC;
    std::cout << "Construction starts ... \n";
    construct(sa, text);
    t_construct = (double) clock () / (double) CLOCKS_PER_SEC - t_construct;
    std::cout << "Construction time: " << t_construct << std::endl;

    double t_find = (double) clock () / (double) CLOCKS_PER_SEC;
    std::cout << "Search starts ... \n";
    string query;
    long total_hits(0);
    double parts = (double)text.size() / (double)query_count;
    for (int i = 0; i < query_count; ++i)
    {
      query = text.substr(int(i * parts), 60);
      find(query, sa, text, hits);
      total_hits += hits.size();
    }
    t_find = (double) clock () / (double) CLOCKS_PER_SEC - t_find;
    std::cout << "Search time: " << t_find << " for " << total_hits << " hits" << std::endl;
    
    if (query_count == 1e6)
    { // hardcoded # hits expected (only applicable to Candidatus.Solibacter.usitatusEllin6076.fasta)
      if (total_hits != 1062218)
      {
        std::cerr << "\nWrong number of hits -- go fix your code!\n";
        return 1;
      }
    }
    
    std::cerr << "Total time: " << t_find + t_construct << " sec\n";
    
    return 0;
}
