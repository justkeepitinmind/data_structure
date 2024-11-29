#include "grammar.hpp"
#include "util.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>
using namespace library;
int main(int argc, char* argv[]) {

    std::string word = argv[1];
    std::vector<index_t> arr;
    arr = query(word);
    for (const auto u : arr) {
        std::cout << get_book_path(u) << "\n";
    }

    if (arr.size() == 0) {
        printf("not found\n");
    }
}