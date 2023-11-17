#include <vector>
#include <string>
#include <algorithm>
#include "aufgabe2.hpp"
using namespace std;

void construct(vector<uint32_t>& sa, const string& text){
if(text.empty()){return;}
sa.clear();
sa.resize(text.length());
vector<int> suffix_rank(text.length()), temp_rank(text.length()); 

for(int i = 0; i < text.length(); i++){
    sa[i] = i; //заполняем вектор индексами текста
    suffix_rank[i] = text[i]; //присваиваем ранг каждому индексу текста
}

for(int k = 1; k < text.length(); k *= 2){ //самый главный цикл для сортировки индексов на основе их рангов
    auto ranks_cmp = [&](int i, int j){
        if(suffix_rank[i] != suffix_rank[j]){//сравниваем начальные ранги суффиксов
            return suffix_rank[i] < suffix_rank[j];
        }
        int rank_first, rank_second;
        if(i + k < text.length()){//если индекс i + k не выходит за пределы строки
           rank_first = suffix_rank[i + k];//присваиваем ri ранг суффикса
        }
        else{rank_first = -1;}//если выходит, то ранг = -1

        if(j + k < text.length()){//если индекс j + k не выходит за пределы строки
            rank_second = suffix_rank[j + k];//присваиваем rj ранг суффикса
        }
        else{rank_second = -1;}////если выходит, то ранг = -1
        return rank_first < rank_second;
    };
    sort(sa.begin(), sa.end(), ranks_cmp);

    for(int i = 1; i < text.length(); i++){// Вычисляем новые ранги, увеличивая на 1, если текущий суффикс отличается от предыдущего
        temp_rank[sa[i]] = temp_rank[sa[i - 1]] + ranks_cmp(sa[i - 1], sa[i]);
    }
    suffix_rank = temp_rank;
    if(suffix_rank[sa[text.length() - 1]] == text.length() - 1) {
        break;//все ранги уникальны
    } 
}
}

void find(const string& query, const vector<uint32_t>& sa, const string& text, vector<uint32_t>& hits) {
hits.clear();
if(query.empty()){return;}

auto compare = [&](uint32_t suftab_i, uint32_t start_idx) {
    for (size_t i = start_idx; i < query.size(); i++) {
        bool text_less_query = (suftab_i + i >= text.size() || text[suftab_i + i] < query[i]);
        if (text_less_query) {
            return -1; // text substring is less than query
        }
        bool text_greater_query = (text[suftab_i + i] > query[i]);
        if (text_greater_query) {
            return 1; // text substring is greater than query
        }
    }
    return 0; // text substring is equal to query
};

    uint32_t L = 0, R = sa.size(), l = 0, r = 0;

    // Binary search for lower bound with MLR heuristic
    while (L < R) {
        int mid = L + (R - L) / 2;
        uint32_t mlr = min(l, r);

        if (compare(sa[mid], mlr) < 0) {
            L = mid + 1;
            l = mlr; // Update l since the left side is matched up to mlr
        } else {
            R = mid;
            r = (compare(sa[mid], mlr) == 0) ? query.size() : mlr; // Update r based on comparison result
        }
    }

    uint32_t lower_bound = L;
    // Reset prefix lengths for upper bound search
    l = r = 0;
    R = sa.size();

    // Binary search for upper bound with MLR heuristic
    while (L < R) {
        int mid = L + (R - L) / 2;
        uint32_t mlr = min(l, r);
        if (compare(sa[mid], mlr) <= 0) {
            L = mid + 1;
            l = (compare(sa[mid], mlr) == 0) ? query.size() : mlr; // Update l based on comparison result
        } else {
            R = mid;
            r = mlr; // Update r since the right side is matched up to mlr
        }
    }

    uint32_t upper_bound = L;
    for (int i = lower_bound; i < upper_bound; i++) {
        hits.push_back(sa[i]);
    }

    sort(hits.begin(), hits.end());
}

