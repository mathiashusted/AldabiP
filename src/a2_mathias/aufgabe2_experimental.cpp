/* This program was a test to determine whether or not the runtime would improve by implementing a 
custom struct rather than using the built-in pair function. In addition, I tested the effects of
taking out the inner for loop and exchanging it for a custom lambda compare function that builds
each suffix. Unsurprisingly, it was a lot slower (~2x).

However, these tests did help determine that creating a custom struct with a custom < operator
is faster than using std::pair with the same lambda function by a factor of ~1.49x.
Thus, this change has been implemented in aufgabe2.cpp.
However, it seems further improvements are still needed to achieve the targeted complexity of (n*logn).
*/

#include "aufgabe2.hpp"

#include <cstdint>
#include <algorithm>
#include <iostream>


struct suffix {
    uint32_t index;
    uint32_t suf;
};

void construct(std::vector<uint32_t>& sa, const std::string& text) {
    std::vector<suffix> suftab {};

    // Reset our array in case it is not empty to construct it from scratch
    if (!sa.empty()) sa.clear();

    for (uint32_t i = 0; i < text.length(); i++) {
        suffix ss;
        ss.index = i;
        ss.suf = i;
        suftab.push_back(ss);
    }
    // Sorting our array with a custom lambda expression (as per cppreference.com) whichs sorts by our second pair value (the string part)
    std::sort(suftab.begin(), suftab.end(), [&text](const suffix a, const suffix b) {
        uint32_t aS = a.suf;
        uint32_t bS = b.suf;
        while (text[aS] == text[bS]) {
            aS++;
            bS++;
        }
        return (text[aS] < text[bS]);
    });
    
    for (size_t i = 0; i < text.length(); i++)
        sa.push_back(suftab[i].index);

    return;
}

void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits) {

}