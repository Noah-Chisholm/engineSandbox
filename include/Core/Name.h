#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace Core {
    class Name {
    public:
        explicit Name(const std::string& inString);

        std::uint64_t getHash() const;
        const std::string& getString() const;
        bool operator==(const Name& other) const;

        struct NameHasher {
            std::size_t operator()(const Name& inName) const noexcept {
                return inName.getHash();
            }
        };
    private:
        std::uint64_t hash = 0;

        static constexpr std::uint64_t hasherM = 18446744073709551557ull;
        static constexpr std::uint64_t hasherP = 131ull;

        static std::unordered_map<std::uint64_t, std::string> hashTable;

        static std::uint64_t computeHash(const std::string& inString);
        static std::uint64_t charToValue(char c, const std::string& fullString, std::size_t index);
    };
}