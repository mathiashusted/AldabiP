#include "Alignment.hpp"
#include <iostream>


int main() {
  Alignment al("IMISSMISSISSIPPI", "MYMISSISAHIPPIE"); //aufgabe3_main IMISSMISSISSIPPI MYMISSISAHIPPIE 3 -4 -1 letsDoLocal

  al.compute(3,-4,-1,false);
  std::string a1, a2, gaps;
  al.getAlignment(a1, gaps, a2);
  return 0;
}
