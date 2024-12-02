#include "index_builder.hpp"
#include "util.hpp"
#include <filesystem>
#include <iostream>
#include <string>
using namespace library;
int main(int argc, char* argv[]) {

    index_builder builder;
    library::news_path = argv[1];
    if (!fs::exists(news_path) || !fs::is_regular_file(news_path)) {
        std::cerr << "path not exists or is not regular file " << std::endl;
        return 1;
    }

    if (!fs::exists(DATA_BASE_PATH)) fs ::create_directory(DATA_BASE_PATH);

    builder.init();
    std::cout << "books are added" << '\n';
    builder.to_file();

    fs::path news_json_file = "";

    return 0;
}
