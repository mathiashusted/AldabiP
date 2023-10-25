#include <iostream>
#include <string>
#include <vector>

#include "aufgabe1.h"

using namespace std;


int main(int argc, const char* argv[])
{
    if (argc != 3)
    {
        std::cout << argv[0] << " <TEXT> <PATTERN>" << std::endl;
        return 1;
    }

    Horspool h;
    
    h.setPattern(argv[2]);
    auto hits = h.getHits(argv[1]);
    
    std::cout << "Horspool search:\n\n-- Text is: --\n" << argv[1] << "\n-- Hits: --\n";
    
    for (const auto hit : hits)
    {
      std::cout << std::string(hit, ' ') << argv[2] << "\n";
    }

    return 0;
}
