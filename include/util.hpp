#ifndef LYC_UTIL
#define LYC_UTIL
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include "cppjieba/HMMSegment.hpp"
#include "cppjieba/KeywordExtractor.hpp"
#include "cppjieba/MPSegment.hpp"
#include "cppjieba/MixSegment.hpp"
#include "limonp/Colors.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace library {
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using std::vector, std::string;
using index_t = unsigned;
using u64 = unsigned long long;
using u32 = unsigned;
namespace fs = std::filesystem;
const std::string DATA_BASE_PATH = "./data";
const std::string OFFSET_FILE_NAME = DATA_BASE_PATH + "/offset";
const std::string INDEX_FILE_NAME = DATA_BASE_PATH + "/index";
const std::string NEWS_OFFSET_PATH = DATA_BASE_PATH + "/news_offset";
const std::string TO_STRIP = " \n\r\t";
const std::string NEWS_PATH_FILE_PATH = DATA_BASE_PATH + "/news_path";
const u64 NOT_FOUND = 0xffffffffffffffff;
const int BUFF_SIZE = 255;
extern std::string news_path;
// extern std::ifstream news_file;

std::string to_lower(const std::string& str);

bool is_valid_utf8(const std::string& subject);

u64 get_hash(const std::string& str);

std::string get_book_path(int i);

std::vector<index_t> query_word(const std::string& word);

void strip(std::string& s);

ptree get_news(u32 idx);

template <typename T>
void write_integer(std::ofstream& file, T number) {
    file.write(reinterpret_cast<const char*>(&number), sizeof(T));
};
} // namespace library
#endif