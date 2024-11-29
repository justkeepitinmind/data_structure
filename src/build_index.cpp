#include "index_builder.hpp"
#include "util.hpp"
#include <filesystem>
#include <iostream>
#include <string>
using namespace library;
int main(int argc, char* argv[]) {

    fs::path directory = argv[1];
    index_builder builder;

    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "path not exists or is not directory " << std::endl;
        return 1;
    }

    if (!fs::exists(DATA_BASE_PATH)) fs ::create_directory(DATA_BASE_PATH);

    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        // std::cout << entry.path() << '\n';

        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            builder.add_book(entry.path());
        }
    }
    std::cout << "books are added" << '\n';
    builder.to_file();

    fs::path news_json_file = "";
    return 0;
}
