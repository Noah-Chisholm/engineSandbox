#pragma once

#include <cstdint>
#include <cassert>

namespace Core {
    enum class EOverflowResolution {
        DROP_NEW,
        OVERWRITE_OLDEST,
        OVERWRITE_NEWEST,
        ASSERT_FAIL
    };

    template<
        typename T,
        std::uint32_t MaxQueue,
        EOverflowResolution OverflowPolicy
    >
    class CircularQueue {
        static_assert(MaxQueue > 0, "MaxQueue must be positive");
        static_assert((MaxQueue& (MaxQueue - 1)) == 0, "MaxQueue must be a power of two");

    private:
        T buffer[MaxQueue];
        std::uint32_t write = 0;
        std::uint32_t read = 0;

        static constexpr std::uint32_t mask() {
            return MaxQueue - 1;
        }

        bool full_impl(std::uint32_t nextWrite) const {
            return nextWrite == read;
        }

    public:
        bool isEmpty() const {
            return read == write;
        }

        bool isFull() const {
            std::uint32_t nextWrite = (write + 1) & mask();
            return full_impl(nextWrite);
        }

        bool enqueue(const T& value) {
            std::uint32_t nextWrite = (write + 1) & mask();

            if (full_impl(nextWrite)) {
                if constexpr (OverflowPolicy == EOverflowResolution::DROP_NEW) {
                    return false;
                }
                else if constexpr (OverflowPolicy == EOverflowResolution::OVERWRITE_OLDEST) {
                    read = (read + 1) & mask();
                    buffer[write] = value;
                    write = nextWrite;
                    return false;
                }
                else if constexpr (OverflowPolicy == EOverflowResolution::OVERWRITE_NEWEST) {
                    std::uint32_t newestIndex = (write - 1) & mask();
                    buffer[newestIndex] = value;
                    return false;
                }
                else if constexpr (OverflowPolicy == EOverflowResolution::ASSERT_FAIL) {
                    assert(false && "queue overflow");
                    return false;
                }
                else {
                    static_assert(OverflowPolicy != OverflowPolicy, "Unhandled overflow policy");
                }
            }

            buffer[write] = value;
            write = nextWrite;
            return true;
        }

        bool dequeue(T& out) {
            if (isEmpty()) {
                return false;
            }

            out = buffer[read];
            read = (read + 1) & mask();
            return true;
        }

        bool peek(T& out) const {
            if (isEmpty()) {
                return false;
            }

            out = buffer[read];
            return true;
        }
    };
}