#ifndef FLOWERHEAP_H
#define FLOWERHEAP_H

#include <set>

namespace EMHS {
    template<typename Element_t = uint64_t>
    class FlowerHeap_t {
    private:
        template<typename HelminthElement_t>
        class Helminth_t {
        private:
            Block_t Head;

            [[nodiscard]] static uint64_t BlockCapacity() {
                return B / sizeof(HelminthElement_t) - 1;
            }

        public:
            Helminth_t() = default;

            explicit Helminth_t(uint64_t Address) {
                (* READ)(Address, Head);
            }

            [[nodiscard]] uint64_t size() const {
                return Head[sizeof(HelminthElement_t) / 8];
            }

            [[nodiscard]] bool empty() const {
                return size() == 0;
            }

            [[nodiscard]] HelminthElement_t upchuck() {
                if (size() % BlockCapacity() == 0) {
                    uint64_t OldSize = size();
                    (* READ)(Head[0], Head);
                    HelminthElement_t Result{};
                    HelminthElement_t * Pointer = & Result;
                    auto MovablePointer = reinterpret_cast<uint64_t *>(Pointer);
                    for (std::size_t Index = sizeof(HelminthElement_t) / 8; Index < 2 * sizeof(HelminthElement_t) / 8; Index++) {
                        * MovablePointer = Head[Index];
                        MovablePointer++;
                    }
                    Head[sizeof(HelminthElement_t) / 8] = OldSize - 1;
                    return Result;
                }
                HelminthElement_t Result{};
                HelminthElement_t * Pointer = & Result;
                auto MovablePointer = reinterpret_cast<uint64_t *>(Pointer);
                for (std::size_t Index = (size() % BlockCapacity() + 1) * sizeof(HelminthElement_t) / 8; Index < (size() % BlockCapacity() + 2) * sizeof(HelminthElement_t) / 8; Index++) {
                    * MovablePointer = Head[Index];
                    MovablePointer++;
                }
                Head[sizeof(HelminthElement_t) / 8]--;
                return Result;
            }

            [[nodiscard]] std::size_t nibble(HelminthElement_t Value, uint64_t NextWriteLocale) {
                HelminthElement_t * Pointer = & Value;
                auto MovablePointer = reinterpret_cast<uint64_t *>(Pointer);
                std::vector<uint64_t> Result;
                for (std::size_t Index = 0; Index < sizeof(HelminthElement_t) / 8; Index++) {
                    Result.push_back(* MovablePointer);
                    MovablePointer++;
                }
                Head[sizeof(HelminthElement_t) / 8]++;
                if (size() % BlockCapacity() == 0) {
                    uint64_t OldSize = size();
                    for (std::size_t Index = sizeof(HelminthElement_t) / 8; Index < 2 * sizeof(HelminthElement_t) / 8; Index++) {
                        Head[Index] = Result[Index - sizeof(HelminthElement_t) / 8];
                    }
                    (* WRITE)(NextWriteLocale, Head);
                    Head[0] = NextWriteLocale;
                    Head[sizeof(HelminthElement_t) / 8] = OldSize;
                    return 1;
                }
                for (std::size_t Index = (size() % BlockCapacity() + 1) * sizeof(HelminthElement_t) / 8; Index < (size() % BlockCapacity() + 2) * sizeof(HelminthElement_t) / 8; Index++) {
                    Head[Index] = Result[Index - (size() % BlockCapacity() + 1) * sizeof(HelminthElement_t) / 8];
                }
                return 0;
            }

            void egg(uint64_t Address) const {
                (* WRITE)(Address, Head);
            }
        };

        class Flower_t {
        private:
            Element_t Border{};
            uint64_t Level{};
            uint64_t Children{};
            std::size_t DataElementsCount{};
            Helminth_t<Element_t> Helminth;

        public:
            Flower_t() = delete;

            Flower_t(Element_t Border, uint64_t Level, uint64_t Children, std::size_t DataElementsCount) : Border(Border), Level(Level), Children(Children), DataElementsCount(DataElementsCount) {};

            Flower_t(uint64_t Address, std::size_t DataElementsCount) : DataElementsCount(DataElementsCount) {
                Block_t Block;
                (* READ)(Address, Block);
                Level = Block[0];
                Children = Block[1];
                Helminth = Helminth_t<Element_t>{Block[2]};
                Element_t * Pointer = & Border;
                auto MovablePointer = reinterpret_cast<uint64_t *>(Pointer);
                for (std::size_t Index = 3; Index < sizeof(Element_t) / 8 + 3; Index++) {
                    * MovablePointer = Block[Index];
                    MovablePointer++;
                }
            }

            [[nodiscard]] bool full() const {
                return Level == 0 && Helminth.size() == 2 * DataElementsCount;
            }

            [[nodiscard]] uint64_t children() const {
                return Children;
            }

            [[nodiscard]] uint64_t level() const {
                return Level;
            }

            [[nodiscard]] Element_t border() const {
                return Border;
            }

            [[nodiscard]] std::size_t add(Element_t Value, uint64_t NextWriteLocale) {
                return Helminth.nibble(Value, NextWriteLocale);
            }

            [[nodiscard]] Helminth_t<Element_t> helminth() const {
                return Helminth;
            }

            void seed(uint64_t Address) {
                Helminth.egg(Address + 1);
                Block_t Block;
                Block[0] = Level;
                Block[1] = Children;
                Block[2] = Address + 1;
                Helminth.egg(Address + 1);
                Element_t * Pointer = & Border;
                auto MovablePointer = reinterpret_cast<uint64_t *>(Pointer);
                for (std::size_t Index = 3; Index < sizeof(Element_t) / 8 + 3; Index++) {
                    Block[Index] = * MovablePointer;
                    MovablePointer++;
                }
                (* WRITE)(Address, Block);
            }
        };

        uint64_t NextWrite;
        uint64_t Size{};
        std::multiset<Element_t> DataElements;
        const std::size_t DataElementsCount;
        const std::size_t DataBlocksCount;
        std::vector<Flower_t *> DataFlowers;

        void Unfroze(uint64_t Address, std::vector<Flower_t *> & Input) {
            Helminth_t<uint64_t> TmpHelminth(Address);
            while (!TmpHelminth.empty()) {
                Input.push_back(new Flower_t{TmpHelminth.upchuck(), DataElementsCount});
            }
            std::sort(Input.begin(), Input.end(), [](Flower_t * A, Flower_t * B) {
                return A->border() < B->border();
            });
        }

        uint64_t Froze(const std::vector<Flower_t *> & Input) {
            Helminth_t<uint64_t> TmpHelminth;
            for (auto Flower : Input) {
                Flower->seed(NextWrite);
                NextWrite += 2;
                NextWrite += TmpHelminth.nibble(NextWrite - 2, NextWrite);
                delete Flower;
            }
            TmpHelminth.egg(NextWrite);
            NextWrite++;
            return NextWrite - 1;
        }

        void Cut(Flower_t * Input, Helminth_t<Element_t> * & ResultHelminth, std::vector<Flower_t *> & ResultVector) {
            ResultHelminth = new Helminth_t<Element_t>{};
            * ResultHelminth = Input->helminth();
            Unfroze(Input->children(), ResultVector);
        }

        void Cut(Flower_t * Input, std::vector<Flower_t *> & Result) {
            Helminth_t<Element_t> * TmpHelminth;
            std::vector<Flower_t *> TmpVector;
            Cut(Input, TmpHelminth, TmpVector);
            delete Input;
            std::multiset<Element_t> TmpElements;
            while (!TmpHelminth->empty()) {
                TmpElements.insert(TmpHelminth->upchuck());
            }
            for (std::size_t Index = 0; Index < 2; Index++) {
                Result.push_back(new Flower_t{* TmpElements.begin(), 0, 0, DataElementsCount});
                for (uint64_t SecondIndex = 0; SecondIndex < DataElementsCount; SecondIndex++) {
                    NextWrite += Result.back()->add(* TmpElements.begin(), NextWrite);
                    TmpElements.erase(TmpElements.begin());
                }
            }
            delete TmpHelminth;
        }

        void ReCut(const std::vector<Flower_t *> & Input, std::vector<Flower_t *> & Result) {
            Result.push_back(new Flower_t{Input[0]->border(), Input[0]->level() + 1, Froze(Input), DataElementsCount});
        }

        void ReCut(const std::multiset<Element_t> & Input, Flower_t * & Result) {
            Result = new Flower_t{* Input.begin(), 0, 0, DataElementsCount};
            for (auto Element : Input) {
                NextWrite += Result->add(Element, NextWrite);
            }
        }

        void Normalize() {
            std::size_t CurrentCount = 1;
            for (std::size_t Index = 1; Index < DataFlowers.size(); Index++) {
                CurrentCount++;
                if (DataFlowers[Index]->level() != DataFlowers[Index - 1]->level()) {
                    CurrentCount = 1;
                }
                if (CurrentCount == 2 * DataBlocksCount) {
                    while (Index + 1 < DataFlowers.size() && DataFlowers[Index + 1]->level() == DataFlowers[Index]->level()) {
                        Index++;
                    }
                    std::vector<Flower_t *> TmpDataFlowers;
                    for (std::size_t SecondIndex = 0; SecondIndex <= Index - DataBlocksCount; SecondIndex++) {
                        TmpDataFlowers.push_back(DataFlowers[SecondIndex]);
                    }
                    std::vector<Flower_t *> ForMergeDataFlowers;
                    for (std::size_t SecondIndex = Index - DataBlocksCount + 1; SecondIndex <= Index; SecondIndex++) {
                        ForMergeDataFlowers.push_back(DataFlowers[SecondIndex]);
                    }
                    ReCut(ForMergeDataFlowers, TmpDataFlowers);
                    for (std::size_t SecondIndex = Index + 1; SecondIndex < DataFlowers.size(); SecondIndex++) {
                        TmpDataFlowers.push_back(DataFlowers[SecondIndex]);
                    }
                    DataFlowers = TmpDataFlowers;
                    Normalize();
                    return;
                }
            }
        }

    public:
        FlowerHeap_t() = delete;

        FlowerHeap_t(uint64_t DirectoryName, std::size_t DataElementsBytes) : NextWrite(DirectoryName), DataElementsCount(DataElementsBytes / sizeof(Element_t)), DataBlocksCount(DataElementsBytes / B) {};

        [[nodiscard]] bool empty() const {
            return Size == 0;
        }

        [[nodiscard]] Element_t top() const {
            return * DataElements.begin();
        }

        void extract() {
            Size--;
            DataElements.erase(DataElements.begin());
            while (DataElements.empty() && !DataFlowers.empty()) {
                Helminth_t<Element_t> * TmpHelminth;
                std::vector<Flower_t *> TmpDataFlowers;
                uint64_t Level = DataFlowers[0]->level();
                Cut(DataFlowers[0], TmpHelminth, TmpDataFlowers);
                delete DataFlowers[0];
                for (std::size_t Index = 1; Index < DataFlowers.size(); Index++) {
                    TmpDataFlowers.push_back(DataFlowers[Index]);
                }
                DataFlowers = TmpDataFlowers;
                Normalize();
                if (Level == 0) {
                    std::multiset<Element_t> TmpDataElements;
                    while (!TmpHelminth->empty()) {
                        TmpDataElements.insert(TmpHelminth->upchuck());
                    }
                    DataElements = TmpDataElements;
                } else {
                    while (!TmpHelminth->empty()) {
                        add(TmpHelminth->upchuck());
                    }
                }
                delete TmpHelminth;
            }
        }

        void add(Element_t Value) {
            Size++;
            int Number = 0;
            while (Number < DataFlowers.size() && DataFlowers[Number]->border() <= Value) {
                Number++;
            }
            Number--;
            if (Number != -1) {
                NextWrite += DataFlowers[Number]->add(Value, NextWrite);
                if (DataFlowers[Number]->full()) {
                    std::vector<Flower_t *> TmpDataFlowers;
                    for (std::size_t Index = 0; Index < Number; Index++) {
                        TmpDataFlowers.push_back(DataFlowers[Index]);
                    }
                    Cut(DataFlowers[Number], TmpDataFlowers);
                    for (std::size_t Index = Number + 1; Index < DataFlowers.size(); Index++) {
                        TmpDataFlowers.push_back(DataFlowers[Index]);
                    }
                    DataFlowers = TmpDataFlowers;
                    Normalize();
                }
                return;
            }
            DataElements.insert(Value);
            if (DataElements.size() == 3 * DataElementsCount) {
                std::multiset<Element_t> TmpDataElements;
                for (std::size_t Index = 0; Index < 2 * DataElementsCount; Index++) {
                    TmpDataElements.insert(* DataElements.begin());
                    DataElements.erase(DataElements.begin());
                }
                DataFlowers.emplace_back();
                for (std::size_t Index = DataFlowers.size() - 1; Index >= 1; Index--) {
                    DataFlowers[Index] = DataFlowers[Index - 1];
                }
                ReCut(DataElements, DataFlowers[0]);
                DataElements = TmpDataElements;
                Normalize();
            }
        }
    };
} // EMHS

#endif