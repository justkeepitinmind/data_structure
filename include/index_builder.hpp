#ifndef LIBRARY_BUILDER
#define LIBRARY_BUILDER

#include "util.hpp"
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>
namespace library {
using dict = std::unordered_map<std::string, std::vector<index_t>>;

class index_builder {
  private:
    cppjieba::MixSegment seg;
    cppjieba::KeywordExtractor extractor;
    dict dic;
    std::vector<std::string> books;

  public:
    index_t get_book_idx(const std::string& file_path);
    index_builder();
    static std::vector<std::string> tokenize(const std::string& s);

    void add(const std::string& word, index_t idx);

    void add_book(const std::string& file_path);

    void index_to_file() const;

    void news_offset_to_file(const std::string& news_path) const;

    void books_to_file() const;

    void to_file() const;
};
} // namespace library
#endif