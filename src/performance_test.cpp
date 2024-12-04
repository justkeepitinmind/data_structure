#include "grammar.hpp"
#include "util.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>
using namespace library;
void __attribute__((optimize("O0"))) test(int n, const std::string expression) {
    for (int i = 0; i < n; i++) {
        query(expression);
    }
}

int main(int argc, char* argv[]) {
    std::ifstream news_path_file(NEWS_PATH_FILE_PATH);
    news_path_file >> news_path;

    std::string expreesion = argv[1];
    std::string tmp = argv[2];
    int n;
    std::stringstream ss;
    ss << tmp;
    ss >> n;
    test(n, expreesion);
}
