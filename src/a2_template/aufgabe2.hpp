//
//   DO NOT CHANGE ANYTHING HERE!!!
//
#pragma once // include guard

#include <string>
#include <vector>

/// Build suffix array from text.
/// @param[out] sa A suffix array (sorted). The variable 'sa' is cleared before it's filled and returned.
/// @param text The text (might be empty)
void construct(std::vector<uint32_t>& sa, const std::string& text);

/// Search a query string in a text via suffix array, which was constructed before using 'construct()'. If the query is empty, return empty hits.
/// @param query The pattern we look for 
/// @param sa A suffix array previously build from 'text'
/// @param text The text that 'sa' is build from
/// @param[out] hits Indices of hits for 'query' in 'text' (sorted ascending by position in text!). The variable is cleared before it's filled and returned
void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits);