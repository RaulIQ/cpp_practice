#pragma once
#include <span>
#include <vector>

class Memory final {
    std::vector<std::byte> bytes;
public:
    Memory();
    std::span<const std::byte> read() const;
    void write(std::span<const std::byte> data);
};
