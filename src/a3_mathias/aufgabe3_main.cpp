#include "Alignment.hpp"
#include <iostream>


int main() {
  Alignment al("test", "tlst");

  al.compute(2,3,4,false);
  return 0;
}
