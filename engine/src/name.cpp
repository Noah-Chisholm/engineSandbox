#include "name.h"

#include <stdexcept>

std::unordered_map<std::uint64_t, std::string> name::hashTable{};

name::name(const std::string& inString) {
    hash = computeHash(inString);

    auto found = hashTable.find(hash);

    if (found == hashTable.end()) {
        hashTable.emplace(hash, inString);
        return;
    }

    if (found->second != inString) {
        throw std::runtime_error(
            "Hash collision while creating name. Existing string: '" +
            found->second +
            "', new string: '" +
            inString +
            "'"
        );
    }
}

std::uint64_t name::getHash() const {
    return hash;
}

std::uint64_t name::computeHash(const std::string& inString) {
    std::uint64_t hashValue = 0;
    std::uint64_t pPow = 1;

    for (std::size_t i = 0; i < inString.length(); ++i) {
        std::uint64_t value = charToValue(inString[i], inString, i);

        hashValue = (hashValue + value * pPow) % hasherM;
        pPow = (pPow * hasherP) % hasherM;
    }

    return hashValue;
}

std::uint64_t name::charToValue(char c, const std::string& fullString, std::size_t index) {
    if (c >= 'a' && c <= 'z') {
        return static_cast<std::uint64_t>(c - 'a' + 1);
    }

    if (c >= 'A' && c <= 'Z') {
        return static_cast<std::uint64_t>(c - 'A' + 27);
    }

    if (c >= '0' && c <= '9') {
        return static_cast<std::uint64_t>(c - '0' + 53);
    }

    if (c == '_') {
        return 63;
    }

    if (c == '-') {
        return 64;
    }

    throw std::runtime_error(
        "Error creating name: '" +
        fullString +
        "' because it contains an invalid character: '" +
        std::string(1, c) +
        "' at index: " +
        std::to_string(index)
    );
}

const std::string& name::getString() const {
    auto found = hashTable.find(hash);

    if (found == hashTable.end()) {
        throw std::logic_error("name hash was not found in the name table");
    }

    return found->second;
}

bool name::operator==(const name& other) const {
    return hash == other.hash;
}