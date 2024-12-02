#include "index_builder.hpp"
#include "util.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace library {

const string ROOT_PATH = "../cppjieba/";
const string DICT_PATH = ROOT_PATH + "dict/jieba.dict.utf8";
const string HMM_MODEL_PATH = ROOT_PATH + "dict/hmm_model.utf8";
const string IDF_PATH = ROOT_PATH + "dict/idf.utf8";
const string STOP_WORD_PATH = ROOT_PATH + "dict/stop_words.utf8";

index_builder::index_builder() : seg(DICT_PATH, HMM_MODEL_PATH), extractor(DICT_PATH, HMM_MODEL_PATH, IDF_PATH, STOP_WORD_PATH) {}

void index_builder::init() {
    std::ofstream news_path_file(NEWS_PATH_FILE_PATH);
    news_path_file << news_path;
    news_offset_to_file(news_path);
    std::ifstream news_file(news_path);
    if (!news_file.is_open()) {
        std::cerr << "unable to open the file" << news_path << '\n';
        return;
    }
    std::string line;
    index_t idx = 0;
    while (news_file.good()) {
        std::getline(news_file, line);
        if (line.empty()) continue;
        add_news(line, idx++);
    }

    news_file.close();
}

void index_builder::add(const std::string& word, index_t idx) {
    if (dic.find(word) == dic.end()) {
        dic[word] = {idx};
    } else {
        if (dic[word].back() != idx) dic[word].push_back(idx);
    }
}

void index_builder::add_news(const std::string& news_line, index_t idx) {
    std::vector<std::string> words;
    ptree pt;
    std::stringstream ss;
    ss << news_line;
    read_json(ss, pt);
    string doc{pt.get<std::string>("content")};
    extractor.Extract(news_line, words, 40);
    for (const std::string& word : words) {
        add(word, idx);
    }
}

void index_builder::index_to_file() const {
    std::vector<std::pair<u64, std::string>> f;
    for (const auto& item : dic) {
        f.emplace_back(get_hash(item.first), item.first);
    }

    sort(f.begin(), f.end());
    std::cout << "the number of word is " << f.size() << ".\n";
    for (int i = 1; i < f.size(); ++i) {
        if (f[i].first == f[i - 1].first) {
            std::cout << "hash collision: " << f[i].second << " " << f[i - 1].second << "\n";
        }
    }

    std::ofstream offset_file(OFFSET_FILE_NAME, std::ios::binary);
    std::ofstream index_file(INDEX_FILE_NAME, std::ios::binary);

    for (const auto& [hash_val, word] : f) {
        // std::cout << word << '\n';
        const auto& list = dic.at(word);
        write_integer<u64>(offset_file, hash_val);
        write_integer<u64>(offset_file, index_file.tellp());

        write_integer<u32>(index_file, word.size());
        index_file.write(word.c_str(), word.size());

        write_integer<index_t>(index_file, list.size());

        for (const index_t idx : list) {
            write_integer<index_t>(index_file, idx);
        }
    }
    offset_file.close();
    index_file.close();
}

void index_builder::news_offset_to_file(const std::string& news_path) const {
    std::ofstream offset_file(NEWS_OFFSET_PATH, std::ios::binary);
    std::ifstream news_file(news_path);
    if (!news_file.is_open()) {
        std::cerr << "unable to open the file" << news_path << '\n';
        return;
    }
    std::string line;
    while (news_file.good()) {
        write_integer<u32>(offset_file, news_file.tellg());
        std::getline(news_file, line);
    }

    news_file.close();
    offset_file.close();
}

void index_builder::to_file() const {
    index_to_file();
}
} // namespace library