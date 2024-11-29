#include "index_builder.hpp"
#include "util.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <unordered_map>
#include <vector>
namespace library {
const string ROOT_PATH = "../cppjieba/";
const string DICT_PATH = ROOT_PATH + "dict/jieba.dict.utf8";
const string HMM_MODEL_PATH = ROOT_PATH + "dict/hmm_model.utf8";
const string IDF_PATH = ROOT_PATH + "dict/idf.utf8";
const string STOP_WORD_PATH = ROOT_PATH + "dict/stop_words.utf8";

index_builder::index_builder() : seg(DICT_PATH, HMM_MODEL_PATH), extractor(DICT_PATH, HMM_MODEL_PATH, IDF_PATH, STOP_WORD_PATH) {}

void index_builder::add(const std::string& word, index_t idx) {
    if (dic.find(word) == dic.end()) {
        dic[word] = {idx};
    } else {
        if (dic[word].back() != idx) dic[word].push_back(idx);
    }
}

void index_builder::add_book(const std::string& file_path) {
    index_t idx = get_book_idx(file_path);
    std::ifstream ifs(file_path);
    if (!ifs.is_open()) {
        std::cerr << "Error opening file: " << std::strerror(errno) << std::endl;
        return;
    };
    string doc;
    doc << ifs;

    std::vector<std::string> words;
    extractor.Extract(doc, words, 40);
    for (const std::string& word : words) {
        add(word, idx);
    }
}

index_t index_builder::get_book_idx(const std::string& file_path) {
    books.push_back(file_path);
    return books.size() - 1;
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
        std::cout << word << '\n';
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

void index_builder::books_to_file() const {
    std::ofstream offset_file(OFFSET_BOOK_NAME, std::ios::binary);
    std::ofstream book_names(BOOK_NAMES);

    for (int i = 0; i < books.size(); ++i) {
        write_integer<u64>(offset_file, book_names.tellp());
        book_names << books.at(i) << " ";
    }
    offset_file.close();
    book_names.close();
}

void index_builder::to_file() const {
    index_to_file();
    books_to_file();
}
} // namespace library