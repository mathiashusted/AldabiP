#include "QGramIndex.hpp"
#include <iostream> // DEBUG
#include <bitset> // DEBUG

int main() {
    std::string pattern = "ACCGTCGTC";
    uint8_t q_length = 2;
    QGramIndex instance(pattern, q_length);

    // std::string test_string = "GA";
    // uint32_t hash_test = instance.hash(test_string);
    
    // std::cout << "Hash of String \"" << test_string << "\"\nAs binary\n" << std::bitset<32>(hash_test) << "\n";
    // std::cout << "Now using mask\n" << std::bitset<32>(~0 << 2*q_length) << "on our input\n";
    // hash_test = instance.hashNext(hash_test, 'T');
    // std::cout << "After cutting:\n" << std::bitset<32>(hash_test) << "\n";
    return 0;
}