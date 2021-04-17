#ifndef EMHS_LIBRARY
#define EMHS_LIBRARY

#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <sys/stat.h>
#include <utility>
#include <vector>

namespace EMHS {
    std::size_t B = 32768;

    namespace {
        std::size_t I_COUNTER{};
        std::size_t O_COUNTER{};

        class Block_t {
        private:
            std::vector <uint64_t> Data;
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

        std::string NameMake(const std::string & DirectoryName, uint64_t NextWrite) {
            std::stringstream StringStream;
            StringStream << DirectoryName << "/" << NextWrite;
            std::string Name;
            StringStream >> Name;
            return Name;
        }

        void READ(const char * DirectoryName, uint64_t NextWrite, Block_t & Block) {\
            std::unique_ptr<FILE, decltype(&fclose)> File(fopen(NameMake(DirectoryName, NextWrite).c_str(), "rb"), &fclose);
            for (std::size_t Index = 0; Index < Block_t::capacity(); Index++) {
                fread(&Block[Index], 8, 1, File.get());
            }
            I_COUNTER++;
        }

        void WRITE(const char * DirectoryName, uint64_t NextWrite, const Block_t & Block) {
            std::unique_ptr<FILE, decltype(&fclose)> File(fopen(NameMake(DirectoryName, NextWrite).c_str(), "wb"), &fclose);
            for (std::size_t Index = 0; Index < Block_t::capacity(); Index++) {
                fwrite(&Block[Index], 8, 1, File.get());
            }
            O_COUNTER++;
        }

        class List_t {
        private:
            const char * DirectoryName;
            uint64_t NextWrite{};
            uint64_t N{};
            uint64_t M{};
            Block_t Block;
            std::size_t Current{};

        public:
            List_t(const char * Name, int) : DirectoryName(Name) {}

            explicit List_t(const char * Name) : DirectoryName(Name) {
                READ(DirectoryName, NextWrite, Block);
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

            [[nodiscard]] std::pair <uint64_t, uint64_t> top() const {
                return std::make_pair(Block[Current], Block[Current + 1]);
            }

            void extract() {
                M--;
                Current += 2;
                if (!empty() && Current == Block_t::capacity()) {
                    Current = 0;
                    NextWrite++;
                    READ(DirectoryName, NextWrite, Block);
                }
            }
        };

        class Output_t {
        private:
            const char * DirectoryName;
            std::optional <uint64_t> NextWrite = std::nullopt;
            Block_t FirstBlock;
            uint64_t Tail{};

        public:
            explicit Output_t(const char * Name) : DirectoryName(Name) {
                remove(DirectoryName);
                mkdir(DirectoryName, ACCESSPERMS);
            }

            void write(const Block_t & Block) {
                if (NextWrite == std::nullopt) {
                    FirstBlock[0] = Block_t::capacity();
                    for (std::size_t Index = 1; Index < Block_t::capacity(); Index++) {
                        FirstBlock[Index] = Block[Index - 1];
                    }
                    Tail = Block[Block_t::capacity() - 1];
                    NextWrite = 1;
                    return;
                }
                FirstBlock[0] += Block_t::capacity();
                Block_t TempBlock;
                TempBlock[0] = Tail;
                for (std::size_t Index = 1; Index < Block_t::capacity(); Index++) {
                    TempBlock[Index] = Block[Index - 1];
                }
                WRITE(DirectoryName, NextWrite.value(), TempBlock);
                Tail = Block[Block_t::capacity() - 1];
                NextWrite.value()++;
            }

            void finalize(const Block_t & Block) {
                if (NextWrite == std::nullopt) {
                    Block_t TempBlock;
                    TempBlock[0] = Block.size();
                    for (std::size_t Index = 0; Index < Block.size(); Index++) {
                        TempBlock[Index + 1] = Block[Index];
                    }
                    WRITE(DirectoryName, 0, TempBlock);
                    return;
                }
                FirstBlock[0] += Block.size();
                WRITE(DirectoryName, 0, FirstBlock);
                Block_t TempBlock;
                TempBlock[0] = Tail;
                for (std::size_t Index = 0; Index < Block.size(); Index++) {
                    TempBlock[Index + 1] = Block[Index];
                }
                WRITE(DirectoryName, NextWrite.value(), TempBlock);
            }
        };
    }

    std::size_t m = 1024;

    class Package_t {
    private:
        template<typename Heap_t, typename Calculator_t>
        friend void dp(const char * InputDirectory, const char * OutputDirectory, Heap_t Heap, Calculator_t Calculator);

        uint64_t Used{};
        uint64_t UsedMax{};
        bool IsExternal = false;
        std::vector <uint64_t> Data;
        Output_t Output;
        bool FirstPart = true;
        Block_t Block;
        List_t List;

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
                        Block_t TempBlock;
                        for (std::size_t Index = 0; Index < B; Index++) {
                            TempBlock[Index] = Data[BlockIndex * B + Index];
                        }
                        Output.write(TempBlock);
                    }
                }
            }
        }

        void check() {
            if (FirstPart) {
                FirstPart = false;
                UsedMax = Used;
                if (IsExternal) {
                    Output.finalize(Block);
                    List = std::move(List_t{"PACKAGE"});
                }
            }
        }

    public:
        Package_t() : Data(m * B / 8), Output("PACKAGE"), List("PACKAGE", 1337) {}

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

        void extract() {
            check();
            if (IsExternal && (UsedMax - Used) % 2 == 1) {
                List.extract();
            }
            Used--;
        }
    };

    template<typename Heap_t, typename Calculator_t>
    void dp(const char * InputDirectory, const char * OutputDirectory, Heap_t Heap, Calculator_t Calculator) {
        List_t List(InputDirectory);
        Block_t Block;
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
        std::cout << "Process finished." << std::endl;
        std::cout << "Total input operations: " << I_COUNTER << "." << std::endl;
        std::cout << "Total output operations: " << O_COUNTER << "." << std::endl;
    }
} // EMHS

#endif // EMHS_LIBRARY
