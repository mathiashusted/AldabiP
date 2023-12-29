#pragma once

// call this function to convert a char to an ordinal value
// i.e.: 
//       a|A|* -> 0, c|C -> 1, g|G -> 2, t|T|u|U -> 3
unsigned ordValue(const unsigned char x);

// call this function to convert an ordinal value (0-3) to a character
unsigned char dna(const unsigned ordVal);
