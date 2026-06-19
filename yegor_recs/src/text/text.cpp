#include "text.hpp"

TextInDocument::TextInDocument(const Document& document) :
    document(document) {}

std::string TextInDocument::read() const {
    return this->document.content();
}
