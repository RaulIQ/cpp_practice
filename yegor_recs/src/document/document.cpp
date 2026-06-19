#include "document.hpp"
#include <format>

Document::Document(int id) :
    id(id),
    title_mem{},
    content_mem{} {}

std::string Document::title() const {
    const std::span<const std::byte> data = this->title_mem.read();
    return std::string(
        reinterpret_cast<const char*>(data.data()),
        data.size()
    );
}

void Document::title(const std::string text) {
    this->title_mem.write(std::span<const std::byte>(
        reinterpret_cast<const std::byte*>(text.data()),
        text.size()
    ));
}

std::string Document::content() const {
    const std::span<const std::byte> data = this->content_mem.read();
    return std::string(
        reinterpret_cast<const char*>(data.data()),
        data.size()
    );
}

void Document::content(const std::string text) {
    this->content_mem.write(std::span<const std::byte>(
        reinterpret_cast<const std::byte*>(text.data()),
        text.size()
    ));
}

bool Document::equals(const Document& other) const {
    return this->id == other.id
        && this->title() == other.title()
        && this->content() == other.content();
}

std::string Document::to_string() const {
    return std::format("doc #{} about '{}' with '{}'", this->id, this->title(), this->content());
}
