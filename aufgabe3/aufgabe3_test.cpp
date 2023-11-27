#include <iostream>
#include <string>
#include <algorithm>
#include "Alignment.hpp"

using namespace std;

bool test_def_score()
{
  string seq_1 = "IMISSMISSISSIPPI";
  string seq_2 = "MYMISSISAHIPPIE";
  Alignment align(seq_1, seq_2);
  align.compute(3, -4, -6);
  return align.getScore() == -5;
}

bool test_custom_score()
{
  string seq_1 = "IMISSMISSISSIPPI";
  string seq_2 = "MYMISSISAHIPPIE";
  Alignment align(seq_1, seq_2);
  align.compute(3, -4, -1);
  return align.getScore() == 24;
}

bool test_align()
{
  string seq_1 = "PEPTIDE";
  string seq_2 = "EANANA";
  Alignment align(seq_1, seq_2);
  align.compute(3, -4, -1);
  // test if the input gets cleared
  string s1("<<seq1_garbage>>"), g("<<match_garbage>>"), s2("<<seq2_garbage>>");
  align.getAlignment(s1, g, s2);
  string m1 = "PE-----PTIDE";
  string m2 = "-EANANA-----";
  string gg = " |          ";
  std::cout << "Alignment test default:\n  got: \n" << s1 << "\n" << g << "\n" << s2 << "\n" << align.getScore() << "\n  expected: \n"
            << m1 << "\n" << gg << "\n" << m2 << "\n" << "-8\n";
	
  // intersperse order of execution for two instances, to discourage use of global variables
  string seq_3 = "IMISSMISSISSIPPI";
  string seq_4 = "MYMISSISAHIPPIE";
  Alignment align2(seq_3, seq_4);
	
  string seq_5 = "LEFT";
  string seq_6 = "XXXLEFT";
  Alignment align3(seq_5, seq_6);
  
  align2.compute(1, -4, -6);
  align3.compute(3, -4, -6);
  align2.compute(3, -4, -6); // call it again, to test clearing of old matrix


  // test if the input gets cleared
  string s3("<<seq1_garbage>>"), g2("<<match_garbage>>"), s4("<<seq2_garbage>>");
  align2.getAlignment(s3, g2, s4);
  string m3  = "IMISSMISSIS-SIPPI-";
  string m4  = "-M--YMISSISAHIPPIE";
  string gg2 = " |   ||||||  |||| ";
  std::cout << "Alignment test clear:\n  got: \n" << s3 << "\n" << g2 << "\n" << s4 << "\n" << align2.getScore() << "\n  expected: \n"
            << m3 << "\n" << gg2 << "\n" << m4 << "\n" << "-5\n";

  // test if the input gets cleared
  string s5("<<seq1_garbage>>"), g3("<<match_garbage>>"), s6("<<seq2_garbage>>");
  align3.getAlignment(s5, g3, s6);
  string m5  = "---LEFT";
  string m6  = "XXXLEFT";
  string gg3 = "   ||||";
  std::cout << "Alignment test leftgap:\n  got: \n" << s5 << "\n" << g3 << "\n" << s6 << "\n" << align3.getScore() << "\n  expected: \n"
            << m5 << "\n" << gg3 << "\n" << m6 << "\n" << "-6\n";
            
  // test corner cases:
  // empty alignment:
  string seq_7 = "";
  string seq_8 = "";
  Alignment align4(seq_7, seq_8);
  align4.compute(3, -4, -6);
  string e1, ge1, e2;
  align4.getAlignment(e1, ge1, e2);
  std::cout << "Alignment test empty:\n  got: \n" << e1 << "\n" << ge1 << "\n" << e2 << "\n" << align4.getScore() << "\n  expected: \n"
            << "''" << "\n" << "''" << "\n" << "''" << "\n" << "0\n";
  
  return (s1 == m1 && s2 == m2 && g == gg && align.getScore() == -8 &&
          s3 == m3 && s4 == m4 && g2 == gg2 && align2.getScore() == -5 &&
          s5 == m5 && s6 == m6 && g3 == gg3 && align3.getScore() == -6 &&
          e1 == "" && ge1 == "" && e2 == "" && align4.getScore() == 0);
}

bool test_repeat()
{
  string seq_1 = "IMISSMISSISSIPPI";
  string seq_2 = "MYMISSISAHIPPIE";
  Alignment align(seq_1, seq_2);

  align.compute(3, -4, -1);
  int score = align.getScore();
  string s1, g, s2;
  align.getAlignment(s1, g, s2);

  // mess up the matrix...
  align.compute(3, -40, 0);
  int score_mess = align.getScore();
  // test if its reset properly
  
  align.compute(3, -4, -1);
  string s11, g1, s21;
  align.getAlignment(s11, g1, s21);
 /*
IMISS-MISSISS--IPPI-
 |    ||||| |  ||||
-M---YMISSI-SAHIPPIE
score:24
    */
  return (s1 == s11 && s2 == s21 && g == g1 && align.getScore() == score && score != score_mess);
}

bool test_uninitialized()
{
  string seq_1 = "IMISSMISSISSIPPI";
  string seq_2 = "MYMISSISAHIPPIE";
  Alignment align(seq_1, seq_2);
  // calling getScore() and getAlignment() before compute() is not allowed
  
  int caught(0);
  
  try
  {
    align.getScore();
  }
  catch(...)
  { // caught! very good
    ++caught;
  }
  
  try
  { 
    string s1, g, s2;
    align.getAlignment(s1, g, s2);
  }
  catch (...)
  { // caught! very good
    ++caught;
  }
  return (caught == 2);
}

// additional points (Smith Waterman)
int test_Smith()
{
  string seq_1 = "XXXIMISSMISSISSIPPIXXX";
  string seq_2 = "YYYMYMISSISAHIPPIEYYY";
  int points = 0; // 4 max
  try
  {
    Alignment align(seq_1, seq_2);
    align.compute(3, -4, -1, true);
    /*
    MISSISS--IPPI
    ||||| |  ||||
    MISSI-SAHIPPI
    score:27
    */
    string m1 = "MISSIS--SIPPI";
    string m2 = "MISSISAH-IPPI";
    string gg = "||||||   ||||";
    string s1, g, s2;
    align.getAlignment(s1, g, s2);
    std::cout << "\nLOCAL Alignment test 1\n"
              << "     got: \n" << s1 << "\n" << g << "\n" << s2 << "\nscore: " << align.getScore() << "\n"
              << "expected: \n"
              << m1 << "\n" << gg << "\n" << m2 << "\nscore: 27\n";
    if (s1 == m1 && s2 == m2 && g == gg && align.getScore() == 27)
    {
      std::cout << "local alignment big test: success\n";
      points += 2;
    } else {
      std::cout << "local alignment big test: fail\n";
      return 0;
    }
    // corner cases
    Alignment align2("a", "b");
    align2.compute(2, -5, -1, false); // test if matrices are properly cleared on second call
    align2.compute(3, -4, -1, true);
    m1 = "";
    m2 = "";
    gg = "";
    s1 = "garbage", g = "garbage", s2 = "garbage";
    align2.getAlignment(s1, g, s2);
    std::cout << "\nLOCAL Alignment test 2 (corner case)\n"
              << "     got: \n" << s1 << "\n" << g << "\n" << s2 << "\nscore: " << align2.getScore() << "\n"
              << "expected: \n"
              << m1 << "\n" << gg << "\n" << m2 << "\nscore: 0\n";
    if (s1 == m1 && s2 == m2 && g == gg && align2.getScore() == 0)
    {
      std::cout << "local alignment corner case test: success\n";
      points += 2;
    } else {
      std::cout << "local alignment corner case test: fail\n";
      return 0;
    }
  } catch (...) {}
  return points;
}


int main()
{
    int points = 0;
    
    if (test_def_score()) points += 1;
    else std::cerr << "      o test_def_score failed (-1)!\n";
    std::cout << "current points: " << points << "\n";
    
    if (test_custom_score()) points += 1;
    else  std::cerr << "      o test_custom_score failed (-1)!\n";
    std::cout << "current points: " << points << "\n";

    if (test_align()) points += 6;
    else std::cerr << "      o test_align failed (-6)!\n";
    std::cout << "current points: " << points << "\n";

    if (test_repeat()) points += 1;
    else std::cerr << "      o test_repeat failed (-1)!\n";
    std::cout << "current points: " << points << "\n";

    if (test_uninitialized()) points += 1;
    else std::cerr << "      o test_uninitialized failed (-1)!\n";
    std::cout << "current points: " << points << "\n";

    // additional points
    int p = test_Smith(); //4 max
    std::cout << "      o test_Smith extra points: " << p << "!\n";
    points += p;

    std::cout << "Final score: " << points << " of 10.\n";
    std::cout << "(if code is correctly pushed into Git && aufgabe3_main.cpp is correct)\n";
    
    // returning the points as error code for easier evaluation
    return 100 + points;
}
