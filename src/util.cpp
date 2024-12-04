#include "util.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

namespace library {
std::string news_path;
using namespace boost::property_tree;

std::string to_lower(const std::string& str) {
    std::string result;
    result.reserve(str.size());
    std::transform(str.begin(), str.end(), std::back_inserter(result), ::tolower);
    return result;
}

void strip(std::string& s) {
    while (s.size() && TO_STRIP.find(s.back()) != std::string::npos) {
        s.pop_back();
    }
    return;
}

const u64 BASE = 1331;

u64 get_hash(const std::string& str) {
    u64 ans = 0;
    for (const char ch : str) {
        ans = ans * BASE + ch;
    }
    return ans;
}

// file 是由无数 [val(u64),offset(u64)] 这样结构单元构成
u64 search_file(const std::string& filename, u64 val) {
    std::ifstream file(filename, std::ios::binary);
    auto get_val = [&file](int i) {
        u64 val;
        file.seekg(16 * i, std::ios::beg);
        file.read(reinterpret_cast<char*>(&val), sizeof(val));
        return val;
    };
    int sz = fs::file_size(filename) / 16;
    int l = 0, r = sz - 1;

    while (l < r) {
        int mid = (l + r) >> 1;
        u64 t = get_val(mid);
        if (t >= val) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }

    if (get_val(r) != val) return NOT_FOUND;
    u64 ans;
    file.seekg(16 * r + 8, std::ios::beg);
    file.read(reinterpret_cast<char*>(&ans), sizeof(ans));
    return ans;
}

std::vector<index_t> get_index_by_offset(u64 offset) {
    std::ifstream index(INDEX_FILE_NAME);
    index.seekg(offset, std::ios::beg);
    auto comsume = [&index]() -> index_t {
        index_t val;
        index.read(reinterpret_cast<char*>(&val), sizeof(val));
        return val;
    };
    index_t sz = comsume();
    std::vector<index_t> result;
    result.reserve(sz);
    for (int i = 0; i < sz; ++i) {
        result.push_back(comsume());
    }
    return result;
}

std::vector<index_t> query_word(const std::string& word) {
    u64 h = get_hash(word);
    u64 offset = search_file(OFFSET_FILE_NAME, h);
    if (offset == NOT_FOUND) return {};
    std::ifstream index(INDEX_FILE_NAME);
    u32 length;
    index.seekg(offset);
    index.read(reinterpret_cast<char*>(&length), sizeof(length));

    std::string word_in_file;
    word_in_file.resize(length);
    index.read(&word_in_file[0], length);
    if (word_in_file != word) return {};
    index.close();
    offset += sizeof(u32) + length;
    return get_index_by_offset(offset);
}

ptree get_news(u32 idx) {
    std::ifstream offset_file(NEWS_OFFSET_PATH, std::ios::binary);
    offset_file.seekg(8 * idx, std::ios::beg);
    u64 offset;
    offset_file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
    std::ifstream news_file(news_path);
    if (!news_file.is_open()) {
        std::cerr << "unable to open file " << news_path << '\n';
        exit(0);
    }
    news_file.seekg(offset);
    std::string line;
    std::getline(news_file, line);
    ptree pt;
    std::stringstream ss;
    ss << line;
    read_json(ss, pt);
    return pt;
}
} // namespace library