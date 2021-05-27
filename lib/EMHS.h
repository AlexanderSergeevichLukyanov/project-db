#ifndef EMHS_H
#define EMHS_H
#include <sstream>

//#include "DISK.h"
#include "disk.h"
#include "towerheap.h"
#include "flowerheap.h"
#include "PairingHeap.h"
//#include "SoftHeap.h"

namespace EMHS {

    namespace {
        class List_t {
        private:
            uint64_t DirectoryName;
            uint64_t NextWrite{};
            uint64_t N{};
            uint64_t M{};
            Block_t<> Block;
            std::size_t Current{};

        public:
            List_t(uint64_t Name, int) : DirectoryName(Name) {}

            explicit List_t(uint64_t Name) : DirectoryName(Name) {
                READ(DirectoryName + NextWrite, Block);
                Current = 2;
                N = Block[0];
                M = Block[1];
            }

            [[nodiscard]] uint64_t number() const {
                return N;
            }

            [[nodiscard]] bool empty() const {
                return M == 0;
            }

            [[nodiscard]] std::pair<uint64_t, uint64_t> top() const {
                return std::make_pair(Block[Current], Block[Current + 1]);
            }

            void extract() {
                M--;
                Current += 2;
                if (!empty() && Current == Block.capacity()) {
                    Current = 0;
                    NextWrite++;
                    READ(DirectoryName + NextWrite, Block);
                }
            }
        };

        class Output_t {
        private:
            uint64_t DirectoryName;
            std::optional<uint64_t> NextWrite = std::nullopt;
            Block_t<> FirstBlock;
            uint64_t Tail{};

        public:
            explicit Output_t(uint64_t Name) : DirectoryName(Name) {}

            void write(const Block_t<> & Block) {
                if (NextWrite == std::nullopt) {
                    FirstBlock[0] = Block.capacity();
                    for (std::size_t Index = 1; Index < Block.capacity(); Index++) {
                        FirstBlock[Index] = Block[Index - 1];
                    }
                    Tail = Block[Block.capacity() - 1];
                    NextWrite = 1;
                    return;
                }
                FirstBlock[0] += Block.capacity();
                Block_t<> TempBlock;
                TempBlock[0] = Tail;
                for (std::size_t Index = 1; Index < Block.capacity(); Index++) {
                    TempBlock[Index] = Block[Index - 1];
                }
                WRITE(DirectoryName + NextWrite.value(), TempBlock);
                Tail = Block[Block.capacity() - 1];
                NextWrite.value()++;
            }

            void finalize(const Block_t<> & Block) {
                if (NextWrite == std::nullopt) {
                    Block_t<> TempBlock;
                    TempBlock[0] = Block.size();
                    for (std::size_t Index = 0; Index < Block.size(); Index++) {
                        TempBlock[Index + 1] = Block[Index];
                    }
                    WRITE(DirectoryName, TempBlock);
                    return;
                }
                FirstBlock[0] += Block.size();
                WRITE(DirectoryName, FirstBlock);
                Block_t<> TempBlock;
                TempBlock[0] = Tail;
                for (std::size_t Index = 0; Index < Block.size(); Index++) {
                    TempBlock[Index + 1] = Block[Index];
                }
                WRITE(DirectoryName + NextWrite.value(), TempBlock);
            }
        };
    }

    class Package_t {
    private:
        uint64_t Used{};
        uint64_t UsedMax{};
        bool IsExternal = false;
        std::vector<uint64_t> Data;
        Output_t Output;
        bool FirstPart = true;
        Block_t<> Block;
        List_t List;

        void check() {
            if (FirstPart) {
                FirstPart = false;
                UsedMax = Used;
                if (IsExternal) {
                    Output.finalize(Block);
                    List = std::move(List_t{0});
                }
            }
        }

    public:
        Package_t() : Data(m * B / 8), Output(0), List(0, 1337) {}

        [[nodiscard]] bool empty() {
            check();
            return Used == 0;
        }

        [[nodiscard]] uint64_t top() {
            check();
            if (UsedMax - Used < 2) {
                return Data[UsedMax - Used];
            }
            if (IsExternal) {
                if ((UsedMax - Used) % 2 == 0) {
                    return List.top().first;
                }
                return List.top().second;
            } else {
                return Data[UsedMax - Used];
            }
        }

        void add(uint64_t Value) {
            if (IsExternal) {
                Block.add(Value);
                if (Block.full()) {
                    Output.write(Block);
                    Block.clear();
                }
                Used++;
            } else {
                Data[Used] = Value;
                Used++;
                if (Used == Data.size()) {
                    IsExternal = true;
                    for (std::size_t BlockIndex = 0; BlockIndex < m; BlockIndex++) {
                        Block_t<> TempBlock;
                        for (std::size_t Index = 0; Index < B; Index++) {
                            TempBlock[Index] = Data[BlockIndex * B + Index];
                        }
                        Output.write(TempBlock);
                    }
                }
            }
        }

        void extract() {
            check();
            if (IsExternal && (UsedMax - Used) % 2 == 1) {
                List.extract();
            }
            Used--;
        }
    };

    template<typename Heap_t, typename Calculator_t>
    void dp(uint64_t InputDirectory, uint64_t OutputDirectory, Heap_t Heap, Calculator_t Calculator) {
        List_t List(InputDirectory);
        Block_t<> Block;
        Output_t Output(OutputDirectory);
        for (uint64_t Current = 0; Current < List.number(); Current++) {
            Package_t Package;
            while (!Heap.empty() && Heap.top().first == Current) {
                Package.add(Heap.top().second);
                Heap.extract();
            }
            uint64_t ResultValue = Calculator(Package);
            Block.add(ResultValue);
            if (Block.full()) {
                Output.write(Block);
                Block.clear();
            }
            while (!List.empty() && List.top().first == Current) {
                Heap.add(std::make_pair(List.top().second, ResultValue));
                List.extract();
            }
        }
        Output.finalize(Block);
    }
} // EMHS

#endif