#include "aufgabe1.h"
#include <iostream>
#include <numeric>

std::ostream& operator<<(std::ostream& o, const std::vector<size_t>& data)
{
   for (const auto& elem : data) o << " " << elem;
   return o;
}

class HorspoolTest : public Horspool
{
  public:
    uint32_t getShift(const char last_char) const
    {
      return Horspool::getShift_(last_char);
    }

    bool shiftTest(char l, size_t expected)
    {
      auto was = getShift(l);
      if (was != expected) std::cout << "Shift value for letter '" << l << "' is not correct: " << was << " (expected: " << expected << ")\n";
      return was == expected;
    }

    bool hitTest(const std::string& text, const std::vector<size_t>& expected, const std::vector<size_t>& align_expected)
    {
      align_tests_.clear();
      auto was = getHits(text);
      if (was != expected)
      {
        std::cout << "Hits are incorrect! Text: '" << text << "', pattern: '" << getPattern() << "'; Expected: " << expected << "; given: " << was << "\n\n";
      }

      if (align_tests_ != align_expected)
      {
        std::cout << "Tested alignment positions are incorrect! Text: '" << text << "', pattern: '" << getPattern() << "'; Expected: " << align_expected << "; you tried: " << align_tests_ << "\n\n";
      }

      return was == expected && align_tests_ == align_expected;
    }

protected:
    void alignCheck_(const size_t text_pos) const override
    {
      align_tests_.push_back(text_pos);
    }

    mutable std::vector<size_t> align_tests_{};

};



int test_setPattern()
{
  HorspoolTest h;
  std::string pat = "test pattern";
  auto patlen = pat.size();
  h.setPattern(pat);
  size_t points{0};
  
  points += h.shiftTest('t', 3);
  points += h.shiftTest('e', 2);
  points += h.shiftTest('r', 1);
  points += h.shiftTest('a', 5);
  points += h.shiftTest('p', 6);
  points += h.shiftTest(' ', 7);
  points += h.shiftTest('s', 9);
  points += h.shiftTest('n', patlen);
  points += h.shiftTest('x', patlen) && h.shiftTest('q', patlen);
  points += h.shiftTest('%', patlen);

  return points / 2;       // 0 to 5 points
}



int test_getHits()
{
  HorspoolTest h;
  size_t points{ 0 };

  h.setPattern("c$pp");
  points += 2 * h.hitTest("c$pp$$c$ppcpp", {0, 6}, {0, 1, 3, 6, 7});

   h.setPattern("ala");
  points += 2 * h.hitTest("zzalalaala", { 2,4,7 }, {0, 2, 4, 6, 7});

  if (points == 4) // only of the alg is correct. Otherwise, 'empty' code would give points
  { // points for corner cases
    points += h.hitTest("", {}, {}) && h.hitTest("la", {}, {}) && h.hitTest("al", {}, {}) && h.hitTest("ala", { 0 }, { 0 });
  }
  return points;
}

int test_extra()
{
  HorspoolTest h;
  size_t points{ 0 };

  h.setPattern("h?ll?");
  points += 2 * h.hitTest("hello! Is it hellas, hallo, hell yeah!, or hull?",
                          { 0, 13, 21, 28, 43 },
                          { 0, 3, 6, 9, 12, 13, 16, 19, 20, 21, 24, 27, 28, 31, 34, 37, 40, 43});

  h.setPattern("?");
  std::string text = "ever?h!t";
  std::vector<size_t> pos(text.size());
  std::iota(pos.begin(), pos.end(), 0);
  points += 2 * h.hitTest(text, pos, pos);

  return points;
}


int main()
{
   std::cout << "Starting test ...\n";

   int points{0};
   points += test_setPattern();
   points += test_getHits();
   points += test_extra();

   std::cout << "Total Points: " << points << " / 10\n";
   return 100 + points;
}