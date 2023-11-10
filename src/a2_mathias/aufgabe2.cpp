#include "aufgabe2.hpp"

#include <cstdint>
#include <algorithm>


void construct(std::vector<uint32_t>& sa, const std::string& text) {

    uint32_t len = text.length();
    sa.resize(len);

    for (uint32_t i = 0; i < len; i++)
        sa[i] = i;

    // Sorting our array with a custom lambda expression (as per cppreference.com) which sorts by our second pair value (the string part)
    // &text is captured, so that we can use it for lookup inside of our lambda function
    // a and b are automatically taken from sa, which holds values from 1..len (our suffix "cursor" positions)
    // Then we base our sorting (return value) on the alphabetical sorting from positions begin + a and begin + b and apply it to sa
    std::sort(sa.begin(), sa.end(), [&text](const uint32_t & a, const uint32_t & b) {
        return (std::lexicographical_compare((text.begin() + a), text.end(), (text.begin() + b), text.end()));
    });
    //lexicographical_compare function found: https://www.appsloveworld.com/cplus/100/23/is-there-a-standard-way-to-compare-two-ranges-in-c
    return;
}

void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits) {

}