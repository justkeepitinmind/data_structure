// #ifndef GRAMMAR_ANALYSIS_TWO
// #define GRAMMAR_ANALYSIS_TWO
// #include "map"
// #include "util.hpp"
// #include <iostream>
// #include <optional>
// #include <sstream>
// #include <stack>
// #include <vector>

// namespace library {
// using std::vector;
// struct minterms {
//     static std::map<std::string, int> word_idx;
//     static vector<std::string> words;
//     static int get_id(const std::string& s) {
//         if (word_idx.find(s) == word_idx.end()) {
//             word_idx[s] = words.size();
//             words.push_back(s);
//         }
//         return word_idx[s];
//     }
//     static void clear() {
//         word_idx.clear();
//         words.clear();
//     }
//     vector<int> idx;
//     vector<bool> f;
//     std::optional<minterms> operator*(const minterms& o) const {
//         int n1 = idx.size();
//         int n2 = o.idx.size();
//         minterms ans;
//         ans.idx.reserve(std::max(n1, n2));
//         ans.f.reserve(std::max(n1, n2));

//         int i = 0, j = 0;
//         for (; i < n1 && j < n2;) {
//             if (idx[i] < o.idx[j]) {
//                 ans.f.push_back(f[i]);
//                 ans.idx.push_back(idx[i++]);
//             } else if (idx[i] > o.idx[j]) {
//                 ans.f.push_back(o.f[j]);
//                 ans.idx.push_back(idx[j++]);
//             } else {
//                 if (f[i] != o.f[j]) return std::nullopt;
//                 ans.f.push_back(f[i]);
//                 ans.idx.push_back(idx[i]);
//                 i++, j++;
//             }
//         }
//         for (; i < n1;) {
//             ans.f.push_back(f[i]);
//             ans.idx.push_back(idx[i++]);
//         }
//         for (; j < n2;) {
//             ans.f.push_back(o.f[j]);
//             ans.idx.push_back(idx[j++]);
//         }
//         return ans;
//     };

//     struct poly {
//         vector<minterms> p;
//         poly opeartor*(const minterms& o) const {

//         }
//     };
// }
// } // namespace library

// #endif