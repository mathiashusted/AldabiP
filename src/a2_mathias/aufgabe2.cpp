#include "aufgabe2.hpp"

#include <cstdint>
#include <algorithm>
#include <utility>



void construct(std::vector<uint32_t>& sa, const std::string& text) {
    std::vector<std::pair<uint32_t, std::string>> suftab {};
    std::string ti;

    // Reset our array in case it is not empty to construct it from scratch
    if (!sa.empty()) sa.clear();

    for (uint32_t i = 0; i < text.length(); i++) {
        ti = "";

        for (uint32_t j = 0+i; j < text.length(); j++)
            ti += text[j];

        suftab.push_back(std::make_pair(i, ti));
    }

    // Sorting our array with a custom lambda expression (as per cppreference.com) whichs sorts by our second pair value (the string part)
    std::sort(suftab.begin(), suftab.end(), [](const std::pair<uint32_t, std::string> a, const std::pair<uint32_t, std::string> b) {
        return a.second < b.second;
    });
    
    for (size_t i = 0; i < suftab.size(); i++)
        sa.push_back(suftab[i].first);

    return;
}

void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits) {

}