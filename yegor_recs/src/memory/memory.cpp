#include "memory.hpp"

Memory::Memory(): bytes{} {}

std::span<const std::byte> Memory::read() const {
    return bytes;
}

void Memory::write(std::span<const std::byte> data) {
    bytes.assign(data.begin(), data.end());
}
