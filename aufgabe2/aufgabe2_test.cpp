// compile with
// make aufgabe2_test
// or (same thing):
// g++ -Wall -pedantic -O3 -D_GLIBCXX_ASSERTIONS -g -fsanitize=address aufgabe2_test.cpp aufgabe2.cpp -o aufgabe2_test
//
#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include "aufgabe2.hpp"

using namespace std;

static vector<uint32_t> sa;
static vector<uint32_t> hits;

bool test_banana_create()
{
   string text = "bananpank";
   construct(sa, text);
   vector<uint32_t> res{1, 6, 3, 0, 8, 2, 7, 4, 5};
   if (sa != res) return false;

   return true;
}

bool test_banana_search()
{
  string text = "bananpank";
  construct(sa, text);
   vector<uint32_t> res{1, 6, 3, 0, 8, 2, 7, 4, 5};
  if (sa != res) return false;

  find("nan", sa, text, hits);
  if (hits != vector<uint32_t>{2}) return false;

  find("ba", sa, text, hits);
  if (hits != vector<uint32_t>{0}) return false;

  find("na", sa, text, hits);
  if (hits != vector<uint32_t>{2}) return false;

  find("an", sa, text, hits); // requires sorting
  if (hits != vector<uint32_t>{1, 3, 6}) return false;

  return true;
}


bool test_empty_text()
{
    string text;
    string query("hello");
    construct(sa, text);
    hits = { 111 };
    find(query, sa, text, hits);
    bool res = (hits.empty());
    
    return res;
}
bool test_empty_query()
{
    string text = "hello";
    string query;
    construct(sa, text);
    hits = { 111 };
    find(query, sa, text, hits);
    bool res = (hits.empty());
    
    return res;
}

bool test_same_word()
{
    string text = "Hello";
    string query(text);
    construct(sa, text);
    find(query, sa, text, hits);
    bool res = (hits.size()==1 && hits[0]==0);
    
    return res;
}

bool test_single_char()
{
    string text = "Helleaeb";
    string query = "e";
    construct(sa, text);
    find(query, sa, text, hits);
    return (hits == vector<uint32_t>{1, 4, 6});
}

bool test_overlap()
{
    string text = "xxxxxxxxxxx";
    string query = "xxx";
    construct(sa, text);
    find(query, sa, text, hits);
    bool res = (hits.size() == 1 + text.size() - query.size());
    if (!res) return false;
    
    for (unsigned i = 0; i < hits.size(); ++i)
    {
        if (hits[i] != i) return false;
    }
    return true;
}

bool test_2ndUsage()
{
    string text = "xxxxyyxxxx";
    string query = "xxx";
    construct(sa, text);
    if (sa.size() != text.size()) return false;
    // construct again
    text = "xxxxxxxxxxx";
    construct(sa, text);
    if (sa.size() != text.size()) return false;

    find(query, sa, text, hits);
    // search again
    find("x", sa, text, hits);
    bool res = (hits.size() == text.size());
    if (!res) return false;
    
    for (unsigned i = 0; i < hits.size(); ++i)
    {
        if (hits[i] != i) return false;
    }
    return true;
}

bool test_no_hit()
{
  { // local scope
    string text = "BANANA";
    string query = "X";   // beyond last entry in SA
    construct(sa, text);
    hits = { 111 };
    find(query, sa, text, hits);
    if (!hits.empty()) return false;
  } 
  {
    string text = "BANANA";
    string query = "KKK"; // hit in the middle of SA
    construct(sa, text);
    hits = { 111 };
    find(query, sa, text, hits);
    if (!hits.empty()) return false;
  }
  {
    string text = "BANANA";
    string query = " "; // hit smaller than SA
    construct(sa, text);
    hits = { 111 };
    find(query, sa, text, hits);
    if (!hits.empty()) return false;
  }
  return true;
}


// test if the implementation does not use the naive implementation using std::vector<string>
bool test_speed()
{
    srand(0);
    std::string big(100000, ' '); // 'large' random text
    for (auto& c : big) c = rand() % 128;
    // time the construction
    auto begin = std::chrono::steady_clock::now();
    construct(sa, big);
    auto end = std::chrono::steady_clock::now();

    size_t time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    
    std::cout << "Construction time: " << time << "\n";
    
    if (time < 100)
    {
      std::cout << "SA Construction seems correctly implemented using indices\n";
      return true;
    }
    else
    {   
      std::cout << "SA Construction takes way too long. Did you compile with -O3 and are you using a real CPU :)? Are you sure you're just sorting numbers?\n";
      return false;
    }
}


int main()
{
            
    int points = 0;
    
    if (test_banana_create())
    {
        points += 2;
    }
    else std::cerr << "test_banana_create failed!\n";

    if (test_banana_search())
    {
      points += 3;
    }
    else std::cerr << "test_banana_search failed!\n";

    if (test_same_word())
    {
      ++points;
    }
    else std::cerr << "test_same_word failed!\n";
      
    if (test_single_char())
    {
      ++points;
    }
    else std::cerr << "test_single_char failed!\n";

    if (test_overlap())
    {
        ++points;
    }
    else std::cerr << "test_overlap failed!\n";

    if (test_2ndUsage())
    {
        ++points;
    }
    else std::cerr << "test_2ndUsage failed!\n";
    
    if (test_no_hit() && test_empty_query() && test_empty_text())
    {
      ++points;
    }
    else std::cerr << "test_no_hit or test_empty_query or test_empty_text failed!\n";

    if (!test_speed())
    {
      points -= 5;
      std::cerr << "test_speed failed! (your implementation will be inspected)\n";
    }
      
    std::cout << "Total points: " << points << "\n";
    std::cout << "(if code is correctly pushed into Git && aufgabe2_main.cpp is correct && your implementation uses MLR )\n";
    
    return 100 + points;
}