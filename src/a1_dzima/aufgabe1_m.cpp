#include "aufgabe1_m.h"
using namespace std;

void Horspool::setPattern(const string& pat) {
this->my_pat = pat; //записываем пэттэрн
this->size_of_pat = this->my_pat.size(); //размер пэттэрна
this->vec_lookup_table = vector<uint32_t>(128); //размер 128, так как используем все ASCII
this->size_of_table = this->vec_lookup_table.size(); //размер текста для вайлдкарт

for(int i = 0; i<this->size_of_table; i++) //на каждый индекс вектора записываем размер пэттэрна
{
  this->vec_lookup_table[i] = this->size_of_pat;
}

uint32_t wild_first_index; //индекс для отслеживания вайлдкартов, изменится на индекс первого вайлдкард в пэттэрне, если имеется
for(uint32_t j = 0; j < this->size_of_pat - 1; j++) 
{
  if ( this->my_pat[j] == '?') {wild_first_index = j;} //если нашли вайлдкарт в пэттэрне - записываем его индекс
  else { this->vec_lookup_table[this->my_pat[j]] = this->size_of_pat - j - 1; }
  //создаем таблицу шифтов по псевдокоду, буква конвертируется в число и значение шифта записывается в индекс равный этому числу 
}

if(wild_first_index >= 0) //если в пэттэрне есть вайлдкард
{
this->new_max_value = this->size_of_pat - wild_first_index - 1; //новое максимальное значение шифта
for(uint32_t i = 0; i < this->size_of_table; i++) //проходим по всему вектору
  {
    if (this->vec_lookup_table[i] > this->new_max_value) //если на каком-то индексе записанное значение больше 
    {
      this->vec_lookup_table[i] = this->new_max_value; //заменяем это значение на новое максимальное
    }
  }
}
}

const string& Horspool::getPattern() const
{
  return this->my_pat; //просто выдает пэттэрн
}

uint32_t Horspool::getShift_(const char last_char) const 
{
  return this->vec_lookup_table[last_char]; //выдает число на которое надо сделать шифт
}

vector<size_t> Horspool::getHits(const std::string& text) const 
{
string pattern = getPattern(); //cохраняем наш пэттэрн
size_t m = pattern.length();
size_t n = text.length();
size_t pos = 0; // переменная для индекса выравнивания текста и пэттэрна
vector<size_t> matches = {}; // вектор на выдачу 

while (pos+m<=n) { // пытаемся выровнять пэттэрн только если есть место в тексте
  alignCheck_(pos); // просто чтобы баллы получить, нахуй не нужно 
  bool char_match = (pattern[m - 1] == text[pos + m - 1] || pattern[m - 1] == '?' || text[pos + m - 1] == '?');
  if (char_match == true) { // сравниваем буквы на одинаковых индексах
    int compare = m - 2;  // счетчик для определения совпадающих букв 
    while (compare >= 0 && (pattern[compare] == text[pos+compare] || pattern[compare] == '?' || text[pos + compare] == '?')) 
    { 
      compare--; 
    } //если совпало - идем к следующей букве

    if (compare == -1) { matches.push_back(pos); } // записываем индекс мэтча
  }
  pos += getShift_(text[pos + m - 1]); // shift на значение(из таблицы) буквы в тексте

}

return matches;
}