#pragma once
#include <cstdint>
#include <cassert>

enum class EOverflowResolutions {
    DROP_NEW,
    OVERWRITE_OLDEST,
    OVERWRITE_NEWEST,
    ASSERT_FAIL
};

template<typename T, std::uint32_t MaxEvents, EOverflowResolutions OverflowPolicy>
class queue {
    static_assert(MaxEvents > 0, "MaxEvents must be positive");
    static_assert((MaxEvents& (MaxEvents - 1)) == 0, "MaxEvents must be a power of two");

private:
    T buffer[MaxEvents];
    std::uint32_t write = 0; // next slot to write
    std::uint32_t read = 0; // next slot to read

    static constexpr std::uint32_t mask() { return MaxEvents - 1; }

    bool full_impl(std::uint32_t nextWrite) const {
        return nextWrite == read; // one-empty-slot ring buffer
    }

public:
    bool isEmpty() const { return read == write; }

    bool isFull() const {
        std::uint32_t nextWrite = (write + 1) & mask();
        return full_impl(nextWrite);
    }

    // Return value:
    //   true  -> enqueued without overflow
    //   false -> overflow occurred (and policy was applied, or enqueue rejected)
    bool enqueue(const T& value) {
        std::uint32_t nextWrite = (write + 1) & mask();

        if (full_impl(nextWrite)) {
            if constexpr (OverflowPolicy == EOverflowResolutions::DROP_NEW) {
                // Keep existing items, reject new one
                return false;
            }
            else if constexpr (OverflowPolicy == EOverflowResolutions::OVERWRITE_OLDEST) {
                // Make room by dropping oldest (advance read)
                read = (read + 1) & mask();
                buffer[write] = value;
                write = nextWrite;
                return false;
            }
            else if constexpr (OverflowPolicy == EOverflowResolutions::OVERWRITE_NEWEST) {
                // Replace the most recently written item (do not move write/read)
                // "Newest" is the item just before 'write' (wrapping)
                std::uint32_t newestIndex = (write - 1) & mask();
                buffer[newestIndex] = value;
                return false;
            }
            else if constexpr (OverflowPolicy == EOverflowResolutions::ASSERT_FAIL) {
                // Catch bugs early: queue capacity too small or consumer too slow
                assert(false && "queue overflow");
                return false;
            }
            else {
                // If someone adds a new enum value and forgets to implement it:
                static_assert(OverflowPolicy != OverflowPolicy, "Unhandled overflow policy");
            }
        }

        buffer[write] = value;
        write = nextWrite;
        return true;
    }

    bool dequeue(T& out) {
        if (isEmpty()) return false;
        out = buffer[read];
        read = (read + 1) & mask();
        return true;
    }

    bool peek(T& out) const {
        if (isEmpty()) return false;
        out = buffer[read];
        return true;
    }
};