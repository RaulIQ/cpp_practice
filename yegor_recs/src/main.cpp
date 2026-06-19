#include <print>
#include "document/document.hpp"
#include "text/text.hpp"

int main() {
    Document first = Document{1};
    first.title("How to grill a sandwich");
    first.content("Bread, cheese, butter");
    
    std::println("{}", first.title());
    std::println("{}", first.content());

    auto content = first.content();
    first.content(content + " and ham");
    std::println("{}", first.content());

    auto text = AllCapsText{
        TextInDocument{first}
    };

    std::println("{}", text.read());

    return 0;
}
