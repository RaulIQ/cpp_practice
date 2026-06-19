#pragma once
#include "memory/memory.hpp"
#include <string>

class Document final {
    const int id;
    Memory title_mem;
    Memory content_mem;
public:
    Document(int id);
    std::string title() const;
    void title(std::string text);
    std::string content() const;
    void content(std::string text);
    bool equals(const Document& other) const;
    std::string to_string() const;
};
