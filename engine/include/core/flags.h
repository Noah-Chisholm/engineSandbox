#pragma once

#include <cstddef>
#include <initializer_list>
#include <type_traits>

template <typename TEnum>
class FlagSet {
    static_assert(std::is_enum_v<TEnum>, "TEnum must be an enum type");

public:
    using BaseType = std::underlying_type_t<TEnum>;

    constexpr FlagSet() = default;

    constexpr FlagSet(std::initializer_list<TEnum> flags) {
        addFlags(flags);
    }

    static constexpr BaseType getNumericValue(TEnum value) {
        return static_cast<BaseType>(value);
    }

    constexpr BaseType getRawValue() const {
        return bits;
    }

    constexpr void setRawValue(BaseType value) {
        bits = value;
    }

    constexpr bool hasFlag(TEnum flag) const {
        return (bits & getNumericValue(flag)) != 0;
    }

    constexpr bool hasFlagsOr(std::initializer_list<TEnum> flags) const {
        for (TEnum flag : flags) {
            if (hasFlag(flag)) {
                return true;
            }
        }

        return false;
    }

    constexpr bool hasFlagsAnd(std::initializer_list<TEnum> flags) const {
        for (TEnum flag : flags) {
            if (!hasFlag(flag)) {
                return false;
            }
        }

        return true;
    }

    constexpr bool hasExactlyOneFlagOf(std::initializer_list<TEnum> flags) const {
        std::size_t count = 0;

        for (TEnum flag : flags) {
            if (hasFlag(flag)) {
                ++count;
            }
        }

        return count == 1;
    }

    constexpr bool hasOnlyFlagsFrom(std::initializer_list<TEnum> flags) const {
        BaseType allowedMask = 0;

        for (TEnum flag : flags) {
            allowedMask |= getNumericValue(flag);
        }

        const BaseType outsideMask = static_cast<BaseType>(~allowedMask);

        return (bits & outsideMask) == 0;
    }

    constexpr void addFlag(TEnum flag) {
        bits |= getNumericValue(flag);
    }

    constexpr void addFlags(std::initializer_list<TEnum> flags) {
        for (TEnum flag : flags) {
            addFlag(flag);
        }
    }

    constexpr void removeFlag(TEnum flag) {
        bits &= static_cast<BaseType>(~getNumericValue(flag));
    }

    constexpr void removeFlags(std::initializer_list<TEnum> flags) {
        for (TEnum flag : flags) {
            removeFlag(flag);
        }
    }

    constexpr void toggleFlag(TEnum flag) {
        bits ^= getNumericValue(flag);
    }

    constexpr void toggleFlags(std::initializer_list<TEnum> flags) {
        for (TEnum flag : flags) {
            toggleFlag(flag);
        }
    }

    constexpr void clear() {
        bits = 0;
    }

private:
    BaseType bits = 0;
};