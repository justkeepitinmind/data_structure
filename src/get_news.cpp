#include "util.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
using namespace library;
void print_news(const ptree& data) {
    std::cout << "Title : " << data.get<std::string>("title") << '\n';
    std::cout << "time : " << data.get<std::string>("time") << '\n';
    std::cout << "desc : " << data.get<std::string>("desc") << '\n';
    std::cout << data.get<std::string>("content") << '\n';
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "please input the idx of the news like\n ./get_news 4\n";
        return 0;
    }
    std::ifstream news_path_file(NEWS_PATH_FILE_PATH);
    news_path_file >> news_path;
    std::stringstream ss;
    int idx;
    ss << argv[1];
    ss >> idx;
    ptree data = get_news(idx);
    print_news(data);
}