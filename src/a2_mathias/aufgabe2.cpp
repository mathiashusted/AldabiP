#include "aufgabe2.hpp"

#include <cstdint>
#include <algorithm>
#include <utility>

struct suffix {
    uint32_t index;
    std::string text;

    suffix(const uint32_t & i, const std::string & t) : index(i), text(t) {}

    bool operator<(const suffix & s) {
        return text < s.text;
    }
};

void construct(std::vector<uint32_t>& sa, const std::string& text) {
    std::vector<suffix> suftab {};
    std::string ti;

    // Reset our array in case it is not empty to construct it from scratch
    if (!sa.empty()) sa.clear();

    for (uint32_t i = 0; i < text.length(); i++) {
        ti = "";

        for (uint32_t j = 0+i; j < text.length(); j++)
            ti += text[j];

        suftab.push_back(suffix(i, ti));
    }

    // Sorting our array with a custom lambda expression (as per cppreference.com) whichs sorts by our second pair value (the string part)
    std::sort(suftab.begin(), suftab.end());
    
    for (size_t i = 0; i < suftab.size(); i++)
        sa.push_back(suftab[i].index);

    return;
}

void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits) {

}