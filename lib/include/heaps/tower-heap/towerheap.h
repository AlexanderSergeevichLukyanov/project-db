#ifndef TOWERHEAP_H
#define TOWERHEAP_H

namespace EMHS {
template <typename Element_t = uint64_t>
class TowerHeap_t {
private:
    class Tower_t {
    private:
        uint64_t MaxHeight{};
        uint64_t Size{};
        uint64_t HeadSize{};
        Block_t<> Head;

    public:
        Tower_t() = delete;

        explicit Tower_t(uint64_t MaxHeight) : MaxHeight(MaxHeight) {
        }

        [[nodiscard]] static uint64_t BlockCapacity() {
            return B / sizeof(Element_t) - 1;
        }

        [[nodiscard]] bool empty() const {
            return Size == 0;
        }

        [[nodiscard]] uint64_t size() const {
            return Size;
        }

        [[nodiscard]] uint64_t capacity() const {
            return MaxHeight * BlockCapacity();
        }

        [[nodiscard]] Element_t top() const {
            Element_t Result{};
            Element_t *Pointer = &Result;
            auto MovablePointer = reinterpret_cast<uint64_t *>(Pointer);
            for (std::size_t Index = sizeof(Element_t) / 8;
                 Index < 2 * sizeof(Element_t) / 8; Index++) {
                *MovablePointer = Head[Index];
                MovablePointer++;
            }
            return Result;
        }

        void clear() {
            Size = 0;
            HeadSize = 0;
            Head.clear();
        }

        void extract() {
            Size--;
            HeadSize--;
            if (Size == 0) {
                clear();
                return;
            }
            if (HeadSize == 0) {
                HeadSize = BlockCapacity();
                READ(Head[0], Head);
                return;
            }
            for (std::size_t Index = sizeof(Element_t) / 8;
                 Index < B / 8 - sizeof(Element_t) / 8; Index++) {
                Head[Index] = Head[Index + sizeof(Element_t) / 8];
            }
        }

        [[nodiscard]] bool full() const {
            return Size == BlockCapacity();
        }

        void ReHead(const Block_t<> &NewHead,
                    uint64_t NewSize,
                    uint64_t NewHeadSize) {
            Head = NewHead;
            Size = NewSize;
            HeadSize = NewHeadSize;
        }

        void add(Element_t Value) {
            HeadSize++;
            Block_t<> NewHead;
            std::size_t Current = sizeof(Element_t) / 8;
            while (!empty() && top() < Value) {
                for (std::size_t Index = Current;
                     Index < Current + sizeof(Element_t) / 8; Index++) {
                    NewHead[Index] =
                        Head[Index - Current + sizeof(Element_t) / 8];
                    if (Index + sizeof(Element_t) / 8 < B / 8) {
                        Head[Index - Current + sizeof(Element_t) / 8] =
                            Head[Index + sizeof(Element_t) / 8];
                    }
                }
                Current += sizeof(Element_t) / 8;
                Size--;
            }
            std::vector<uint64_t> Parsed = Parse(Value);
            for (std::size_t Index = Current;
                 Index < Current + sizeof(Element_t) / 8; Index++) {
                NewHead[Index] = Parsed[Index - Current];
            }
            Current += sizeof(Element_t) / 8;
            while (!empty()) {
                for (std::size_t Index = Current;
                     Index < Current + sizeof(Element_t) / 8; Index++) {
                    NewHead[Index] =
                        Head[Index - Current + sizeof(Element_t) / 8];
                    if (Index + sizeof(Element_t) / 8 < B / 8) {
                        Head[Index - Current + sizeof(Element_t) / 8] =
                            Head[Index];
                    }
                }
                Current += sizeof(Element_t) / 8;
                Size--;
            }
            ReHead(NewHead, HeadSize, HeadSize);
        }
    };

    uint64_t NextWrite;
    uint64_t Size{};
    std::vector<Tower_t> Data;

    [[nodiscard]] static std::vector<uint64_t> Parse(Element_t Value) {
        Element_t *Pointer = &Value;
        auto MovablePointer = reinterpret_cast<uint64_t *>(Pointer);
        std::vector<uint64_t> Result;
        for (std::size_t Index = 0; Index < sizeof(Element_t) / 8; Index++) {
            Result.push_back(*MovablePointer);
            MovablePointer++;
        }
        return Result;
    }

    [[nodiscard]] uint64_t Height(std::size_t X) const {
        if (X < 1 + 2 + 4) {
            return 1 * 1 + 1 * 2;
        }
        if (X < 1 + 2 + 4 + 8) {
            return 1 * 1 + 1 * 2 + 3 * 4;
        }
        if (X < 1 + 2 + 4 + 8 + 16) {
            return 1 * 1 + 1 * 2 + 3 * 4 + 15 * 8;
        }
        if (X < 1 + 2 + 4 + 8 + 16 + 32) {
            return 1 * 1 + 1 * 2 + 3 * 4 + 15 * 8 + 135 * 16;
        }
        if (X < 1 + 2 + 4 + 8 + 16 + 32 + 128) {
            return 1 * 1 + 1 * 2 + 3 * 4 + 15 * 8 + 135 * 16 + 2295 * 32;
        }
        if (X < 1 + 2 + 4 + 8 + 16 + 32 + 128 + 512) {
            return 1 * 1 + 1 * 2 + 3 * 4 + 15 * 8 + 135 * 16 + 2295 * 32 +
                   75735 * 128;
        }
        if (X < 1 + 2 + 4 + 8 + 16 + 32 + 128 + 512 + 4096) {
            return 1LL * 1LL + 1LL * 2LL + 3LL * 4LL + 15LL * 8LL +
                   135LL * 16LL + 2295LL * 32LL + 75735LL * 128LL +
                   9769815LL * 512LL;
        }
        return 1LL * 1LL + 1LL * 2LL + 3LL * 4LL + 15LL * 8LL + 135LL * 16LL +
               2295LL * 32LL + 75735LL * 128LL + 9769815LL * 512LL +
               5011915095LL * 4096LL;
    }

    [[nodiscard]] bool IsHead(std::size_t X) const {
        return X == 2 || X == 6 || X == 14 || X == 30 || X == 62 || X == 190 ||
               X == 702 || X == 4798;
    }

    void Merge(TowerHeap_t<Element_t> Tmp, std::size_t Number) {
        uint64_t TmpSize = Tmp.Size;
        Block_t<> NewHead;
        if (TmpSize == 0) {
            Data[Number].ReHead(NewHead, 0, 0);
            return;
        }
        std::size_t TmpHeadSize = TmpSize % Tower_t::BlockCapacity();
        if (TmpHeadSize == 0) {
            TmpHeadSize = Tower_t::BlockCapacity();
        }
        for (std::size_t Index = 1; Index <= TmpHeadSize; Index++) {
            std::vector<uint64_t> Parsed = Parse(Tmp.top());
            Tmp.extract();
            for (std::size_t SecondIndex = 0;
                 SecondIndex < sizeof(Element_t) / 8; SecondIndex++) {
                NewHead[Index * sizeof(Element_t) / 8 + SecondIndex] =
                    Parsed[SecondIndex];
            }
        }
        uint64_t Hole{};
        if (TmpSize != TmpHeadSize) {
            NewHead[0] = NextWrite;
            Hole = NextWrite;
            NextWrite++;
        }
        uint64_t NewTowerDiskHeight =
            (TmpSize - TmpHeadSize) / Tower_t::BlockCapacity();
        while (NewTowerDiskHeight > 0) {
            NewTowerDiskHeight--;
            Block_t<> ToDisk;
            for (std::size_t Index = 1; Index <= Tower_t::BlockCapacity();
                 Index++) {
                std::vector<uint64_t> Parsed = Parse(Tmp.top());
                Tmp.extract();
                for (std::size_t SecondIndex = 0;
                     SecondIndex < sizeof(Element_t) / 8; SecondIndex++) {
                    ToDisk[Index * sizeof(Element_t) / 8 + SecondIndex] =
                        Parsed[SecondIndex];
                }
            }
            if (NewTowerDiskHeight > 0) {
                ToDisk[0] = NextWrite;
                WRITE(Hole, ToDisk);
                Hole = NextWrite;
                NextWrite++;
            } else {
                WRITE(Hole, ToDisk);
            }
        }
        Data[Number].ReHead(NewHead, TmpSize, TmpHeadSize);
    }

public:
    TowerHeap_t() = delete;

    explicit TowerHeap_t(uint64_t DirectoryName) : NextWrite(DirectoryName){};
    TowerHeap_t &operator=(const TowerHeap_t &) = default;

    [[nodiscard]] bool empty() const {
        return Size == 0;
    }

    [[nodiscard]] uint64_t size() const {
        return Size;
    }

    [[nodiscard]] Element_t top() const {
        Element_t Minimum{};
        for (auto Value : Data) {
            if (!Value.empty()) {
                Minimum = Value.top();
                break;
            }
        }
        for (auto Value : Data) {
            if (!Value.empty() && Value.top() < Minimum) {
                Minimum = Value.top();
            }
        }
        return Minimum;
    }

    void extract() {
        Size--;
        Element_t Top = top();
        std::size_t Index = 0;
        while (Data[Index].empty() || Data[Index].top() != Top) {
            Index++;
        }
        Data[Index].extract();
    }

    void add(Element_t Value) {
        Size++;
        for (std::size_t Index = 0; Index < 3; Index++) {
            if (Data.size() == Index) {
                Data.push_back(Tower_t{1});
            }
            if (!Data[Index].full()) {
                Data[Index].add(Value);
                return;
            }
        }
        std::size_t FullHead = 0;
        uint64_t Count = 0;
        uint64_t Capacity = 0;
        while (!IsHead(FullHead) || 2 * Count >= Capacity) {
            if (Data.size() == FullHead) {
                Data.push_back(Tower_t{Height(FullHead)});
            }
            Count += Data[FullHead].size();
            Capacity += Data[FullHead].capacity();
            FullHead++;
        }
        FullHead--;
        while (!IsHead(FullHead)) {
            FullHead--;
        }
        std::size_t First = FullHead + 1;
        std::size_t Second = FullHead + 2;
        if (Data[Second].size() < Data[First].size()) {
            First++;
            Second--;
        }
        std::size_t Current = FullHead + 2;
        while (!IsHead(Current)) {
            Current++;
            if (Data.size() == Current) {
                Data.push_back(Tower_t{Height(Current)});
            }
            if (Data[First].size() > Data[Current].size()) {
                Second = First;
                First = Current;
                continue;
            }
            if (Data[Second].size() > Data[Current].size()) {
                Second = Current;
            }
        }
        TowerHeap_t<Element_t> TmpOne(NextWrite);
        while (TmpOne.Data.size() <= First || TmpOne.Data.size() <= Second) {
            TmpOne.Data.push_back(Tower_t{Height(TmpOne.Data.size())});
        }
        TmpOne.Data[First] = Data[First];
        TmpOne.Data[Second] = Data[Second];
        TmpOne.Size += Data[First].size();
        TmpOne.Size += Data[Second].size();
        Data[First].clear();
        Data[Second].clear();
        Merge(TmpOne, First);
        TowerHeap_t<Element_t> TmpTwo(NextWrite);
        for (std::size_t Index = 0; Index <= FullHead; Index++) {
            TmpTwo.Data.push_back(Tower_t{Height(Index)});
            TmpTwo.Data[Index] = Data[Index];
            TmpTwo.Size += Data[Index].size();
            Data[Index].clear();
        }
        Merge(TmpTwo, Second);
        Size--;
        add(Value);
    }
};
}  // namespace EMHS

#endif