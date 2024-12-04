#ifndef GRAMMAR_ANALYSIS
#define GRAMMAR_ANALYSIS
#include "util.hpp"
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
namespace library {
using std::vector, std::stack;
// 求两个集合的交
template <class T>
vector<T> SET_AND(const vector<T>& a, const vector<T>& b) {
    vector<T> c;
    int i = 0, j = 0;
    for (; i < a.size() && j < b.size();) {
        if (a[i] < b[j])
            i++;
        else if (a[i] > b[j])
            j++;
        else
            c.push_back(a[i]), i++, j++;
    }
    return c;
}

// 求两个集合的并
template <class T>
vector<T> SET_OR(const vector<T>& a, const vector<T>& b) {
    vector<T> c;
    c.reserve(std::max(a.size(), b.size()));
    int i = 0, j = 0;
    for (; i < a.size() && j < b.size();) {
        if (a[i] < b[j])
            c.push_back(a[i++]);
        else if (a[i] > b[j])
            c.push_back(b[j++]);
        else
            c.push_back(a[i]), i++, j++;
    }
    for (; i < a.size();) c.push_back(a[i++]);
    for (; j < b.size();) c.push_back(b[j++]);
    return c;
}

// 集合作差
template <class T>
vector<T> SET_SUB(const vector<T>& a, const vector<T>& b) {
    vector<T> c;
    for (int i = 0, j = 0; i < a.size(); ++i) {
        while (j < b.size() && b[j] < a[i]) j++;
        if (j == b.size() || b[j] != a[i]) c.push_back(a[i]);
    }
    return c;
}

enum OP { AND,
          OR,
          LEFT_BRACKET,
          RIGHT_BRACKET,
          SUB };

template <class T>
vector<T> op_work(const vector<T>& a, const vector<T>& b, const OP op) {
    if (op == AND)
        return SET_AND(a, b);
    else if (op == OR)
        return SET_OR(a, b);
    else if (op == SUB)
        return SET_SUB(a, b);
    std::cerr << "unknown operator" << '\n';
    return {};
}

vector<index_t> query(const string& s) {
    stack<vector<index_t>> st;
    stack<OP> op_st;
    // 检查 s 接下来几位是否为o.即 s[i:i+o.size()] == o
    auto check_nxt = [&s](int idx, const string& o) -> bool {
        if (idx + o.size() >= s.size()) return 0;
        for (int i = 0; i < o.size(); ++i)
            if (o[i] != s[i + idx]) return 0;
        // if (idx + o.size() != s.size() && s[idx + o.size()] != ' ') return 0;
        return 1;
    };

    auto pop_and_func = [&]() -> bool {
        if (op_st.size() == 0 || st.size() < 2) {
            std::cerr << "wrong expressions." << '\n';
            return 0;
        };
        vector<index_t> t2(std::move(st.top()));
        st.pop();
        vector<index_t> t1(std::move(st.top()));
        st.pop();
        OP op = op_st.top();
        op_st.pop();
        st.push(op_work(t1, t2, op));
        return 1;
    };

    for (int i = 0; i < s.size();) {
        if (s[i] == ' ') {
            i++;
        } else if (s[i] == '(') {
            i++;
            op_st.push(LEFT_BRACKET);
        } else if (s[i] == ')') {
            i++;
            while (op_st.size() && op_st.top() != LEFT_BRACKET) {
                if (!pop_and_func()) {
                    return {};
                };
            }
            if (op_st.size() == 0) {
                std::cerr << "unmatched right backet." << '\n';
                return {};
            };
            op_st.pop();
        } else if (check_nxt(i, "and") || check_nxt(i, "AND") || check_nxt(i, "&&") || check_nxt(i, "&") || check_nxt(i, "*")) {
            if (check_nxt(i, "and") || check_nxt(i, "AND"))
                i += 3;
            else if (check_nxt(i, "&&"))
                i += 2;
            else
                i += 1;
            op_st.push(AND);
        } else if (check_nxt(i, "or") || check_nxt(i, "OR") || check_nxt(i, "||") || check_nxt(i, "|") || check_nxt(i, "+")) {
            if (check_nxt(i, "or") || check_nxt(i, "OR") || check_nxt(i, "||"))
                i += 2;
            else
                i += 1;
            while (op_st.size() && op_st.top() == AND) {
                if (!pop_and_func()) {
                    return {};
                };
            }
            op_st.push(OR);
        } else if (check_nxt(i, "sub") || check_nxt(i, "SUB") || check_nxt(i, "-")) {
            if (check_nxt(i, "sub") || check_nxt(i, "SUB"))
                i += 3;
            else
                i += 1;
            while (op_st.size() && op_st.top() == AND) {
                if (!pop_and_func()) {
                    return {};
                };
            }
            op_st.push(SUB);
        } else {
            int j = i + 1;
            while (j < s.size() && s[j] != ' ') j++;
            string word = s.substr(i, j - i);
            st.push(query_word(word));
            i = j;
        }
    }

    while (op_st.size()) {
        if (!pop_and_func()) {
            return {};
        };
    }
    if (st.size() != 1) {
        std::cerr << "wrong expressions." << '\n';
        return {};
    };
    return st.top();
}

} // namespace library
#endif