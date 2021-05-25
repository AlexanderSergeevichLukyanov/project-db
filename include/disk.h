#ifndef DISK_H
#define DISK_H

#include <functional>

namespace EMHS {
    std::size_t B = 48;
    std::size_t m = 5;

    class Block_t {
    private:
        std::vector<uint64_t> Data;
        std::size_t Used{};

    public:
        Block_t() : Data(B / 8) {}

        [[nodiscard]] static uint64_t capacity() {
            return B / 8;
        }

        [[nodiscard]] uint64_t size() const {
            return Used;
        }

        [[nodiscard]] bool full() const {
            return size() == capacity();
        }

        void clear() {
            Used = 0;
            for (uint64_t & Index : Data) {
                Index = 0;
            }
        }

        void add(uint64_t Value) {
            Data[Used] = Value;
            Used++;
        }

        [[nodiscard]] uint64_t &operator[](std::size_t Index) {
            return Data[Index];
        }

        [[nodiscard]] const uint64_t &operator[](std::size_t Index) const {
            return Data[Index];
        }
    };

    void (* READ)(uint64_t, Block_t &);

    void (* WRITE)(uint64_t, const Block_t &);
} // EMHS

#endif