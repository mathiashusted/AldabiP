#include "aufgabe2.hpp"

#include <cstdint>
#include <algorithm>


void construct(std::vector<uint32_t>& sa, const std::string& text) {

    uint32_t len = text.length();
    sa.resize(len);
    size_t counter = 0;

    for (uint32_t i = 0; i < len; i++)
        sa[i] = i;

    // Sorting our array with a custom lambda expression (as per cppreference.com) which sorts by comparing the text of two sa entries
    // &text and &counter are captured, so that we can use them for lookup inside of our lambda function
    // a and b are automatically taken from sa, which holds values from 1..len (our suffix start positions)
    // Two entries are compared by their ASCII values and return 1 if the a-suffix is smaller than the b-suffix, otherwise 0.
    std::sort(sa.begin(), sa.end(), [&text, &counter](const uint32_t & a, const uint32_t & b) {
        for (counter = 0; text[a+counter] == text[b+counter];)
            counter++;

        return (text[a+counter] < text[b+counter]);
    });
    return;
}

void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits) {

}