#pragma once
#include "document/document.hpp"
#include <algorithm>
#include <string>

class TextInDocument final {
    const Document& document;
public:
    TextInDocument(const Document& document);
    std::string read() const;
};

template <typename T>
class AllCapsText final {
    T origin;
public:
    AllCapsText(T origin) : origin(origin) {}

    std::string read() {
        std::string text = this->origin.read();
        std::ranges::transform(text, text.begin(),
            [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
        return text;
    }
};
