#ifndef GLOBAL_H
#define GLOBAL_H
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

extern unordered_map<int, vector<string>> person_ids;
extern unordered_map<string, vector<int>> area_map;
extern unordered_map<int, string> img_map;

#endif // GLOBAL_H
