// main.cpp
#include <iostream>
#include <expected>
#include <system_error>
#include <sys/stat.h>
#include <cerrno>

std::expected<void, std::error_code> ensure_directory(const char* path) {
    struct stat st{};
    if (stat(path, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            return {};
        }
        return std::unexpected{std::make_error_code(std::errc::file_exists)};
    }

    if (mkdir(path, 0755) == 0) {
        return {};
    }

    return std::unexpected{std::error_code{errno, std::generic_category()}};
}

int main() {
    if (auto result = ensure_directory("data"); !result) {
        std::cerr << "Failed to create directory: " << result.error().message() << '\n';
        return 1;
    }

    return 0;
}