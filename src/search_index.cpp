#include "grammar.hpp"
#include "util.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>
using namespace library;
int main(int argc, char* argv[]) {
    std::ifstream news_path_file(NEWS_PATH_FILE_PATH);
    news_path_file >> news_path;

    std::string word = argv[1];
    std::vector<index_t> arr;
    arr = query(word);
    for (const auto u : arr) {
        ptree data = get_news(u);
        std::cout << "idx : " << u << ". title : " << data.get<std::string>("title") << '\n';
    }

    if (arr.size() == 0) {
        printf("not found\n");
    }
}