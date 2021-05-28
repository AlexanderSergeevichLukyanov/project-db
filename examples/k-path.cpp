#include "EMHS.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <sys/stat.h>

std::size_t I_COUNTER{};
std::size_t O_COUNTER{};

std::string NameMake(uint64_t NextWrite) {
        std::stringstream StringStream;
        StringStream << "lib/DISK/" << NextWrite;
        std::string Name;
        StringStream >> Name;
        return Name;
    }

template<typename T>
    void READ(uint64_t NextWrite, EMHS::Block_t<T> & Block){
        std::unique_ptr<FILE, decltype(& fclose)> File(fopen(NameMake(NextWrite).c_str(), "rb"), & fclose);
        fread(& Block[0], sizeof(T), Block.capacity(), File.get());
        I_COUNTER++;
    }
    
    template<typename T>
    void WRITE(uint64_t NextWrite, const EMHS::Block_t<T> & Block){
        //std::cerr<<NameMake(NextWrite);
        std::unique_ptr<FILE, decltype(& fclose)> File(fopen(NameMake(NextWrite).c_str(), "wb"), & fclose);
        fwrite(& Block[0], sizeof(T), Block.capacity(), File.get());
        O_COUNTER++;
    }

uint64_t NextWrite = 3000000000;

struct Calculator {
    const std::size_t k;
    Calculator() = delete;
    explicit Calculator(std::size_t k) : k(k) {}
    [[nodiscard]] uint64_t operator()(EMHS::Package_t & Package) const {
        EMHS::Block_t ResultBlock;
        while (!Package.empty()) {
            EMHS::Block_t TmpBlock;
            uint64_t TmpBlockAddress = Package.top();
            Package.extract();
            READ(TmpBlockAddress, TmpBlock);
            ResultBlock[0] += 1;
            for (std::size_t Index = 0; Index < k - 1; Index++) {
                ResultBlock[Index + 1] += TmpBlock[Index];
            }
            ResultBlock[k - 1] += TmpBlock[k - 1];
        }
        WRITE(NextWrite, ResultBlock);
        NextWrite++;
        return NextWrite - 1;
    }
};

int main() {
    //remove("DISK");
    //mkdir("DISK", ACCESSPERMS);
    EMHS::m = 4;
    EMHS::B = 1024;
    //EMHS::READ = READ;
    //EMHS::WRITE = WRITE;
    std::size_t MaxSize = 3000;
    std::size_t Density = 8;
    std::size_t k = 14;
    for (std::size_t Test = 0; Test < 500; Test++) {
        std::vector<std::vector<std::size_t>> Chip(rand() % MaxSize + 1);
        for (std::size_t Index = 1; Index < Chip.size(); Index++) {
            std::size_t Count = (rand() % Index) % Density;
            std::set<std::size_t> TmpSet;
            while (TmpSet.size() < Count) {
                TmpSet.insert(rand() % Index);
            }
            for (auto SecondIndex : TmpSet) {
                Chip[Index].push_back(SecondIndex);
            }
        }
        std::vector<std::vector<uint64_t>> InCoreProgress(Chip.size(), std::vector<uint64_t>(k, 0));
        for (std::size_t Index = 0; Index < Chip.size(); Index++) {
            for (auto SecondIndex : Chip[Index]) {
                InCoreProgress[Index][0] += 1;
                for (std::size_t ThirdIndex = 0; ThirdIndex < k - 1; ThirdIndex++) {
                    InCoreProgress[Index][ThirdIndex + 1] += InCoreProgress[SecondIndex][ThirdIndex];
                }
                InCoreProgress[Index][k - 1] += InCoreProgress[SecondIndex][k - 1];
            }
        }
        std::vector<uint64_t> InCoreResult(Chip.size());
        for (std::size_t Index = 0; Index < Chip.size(); Index++) {
            InCoreResult[Index] = InCoreProgress[Index][k - 1];
        }
        std::vector<uint64_t> Input;
        Input.push_back(Chip.size());
        std::size_t NumberEdges = 0;
        for (auto & Index : Chip) {
            NumberEdges += Index.size();
        }
        Input.push_back(NumberEdges);
        std::vector<std::pair<uint64_t, uint64_t>> Pairs;
        for (std::size_t Index = 0; Index < Chip.size(); Index++) {
            for (auto SecondIndex : Chip[Index]) {
                Pairs.emplace_back(SecondIndex, Index);
            }
        }
        std::sort(Pairs.begin(), Pairs.end());
        for (auto Index : Pairs) {
            Input.push_back(Index.first);
            Input.push_back(Index.second);
        }
        while (Input.size() % (EMHS::B / 8) != 0) {
            Input.push_back(0);
        }
        for (std::size_t Index = 0; Index < Input.size() / (EMHS::B / 8); Index++) {
            std::stringstream StringStream;
            StringStream << "lib/DISK/" << 1000000000 + Index;
            std::string Name;
            StringStream >> Name;
            std::unique_ptr<FILE, decltype(& fclose)> File(fopen(Name.c_str(), "wb"), & fclose);
            for (std::size_t IndexFile = 0; IndexFile < EMHS::B / 8; IndexFile++) {
                fwrite(& Input[Index * (EMHS::B / 8) + IndexFile], 8, 1, File.get());
            }
        }
        EMHS::dp(1000000000, 2000000000, EMHS::PairingHeap<std::pair<uint64_t, uint64_t>>{}, Calculator{k});
        std::cout << "Process finished." << std::endl;
        std::cout << "Total input operations: " << I_COUNTER << "." << std::endl;
        std::cout << "Total output operations: " << O_COUNTER << "." << std::endl;
        std::size_t OutputSize = Chip.size() + 1;
        while (OutputSize % (EMHS::B / 8) != 0) {
            OutputSize++;
        }
        std::vector<uint64_t> Output(OutputSize);
        for (std::size_t Index = 0; Index < OutputSize / (EMHS::B / 8); Index++) {
            std::stringstream StringStream;
            StringStream << "lib/DISK/" << 2000000000 + Index;
            std::string Name;
            StringStream >> Name;
            std::unique_ptr<FILE, decltype(& fclose)> File(fopen(Name.c_str(), "rb"), & fclose);
            for (std::size_t IndexFile = 0; IndexFile < EMHS::B / 8; IndexFile++) {
                fread(& Output[Index * (EMHS::B / 8) + IndexFile], 8, 1, File.get());
            }
        }
        std::vector<uint64_t> OutCoreResult(Chip.size());
        for (std::size_t Index = 0; Index < Chip.size(); Index++) {
            EMHS::Block_t TmpBlock;
            READ(Output[Index + 1], TmpBlock);
            OutCoreResult[Index] = TmpBlock[k - 1];
        }
        if (Chip.size() == Output[0] && InCoreResult == OutCoreResult) {
            std::cout << "Ok." << std::endl;
            std::cout << Chip.size() << std::endl;
            for (auto Index : InCoreResult) {
                std::cout << Index << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "Pizdec. O_o" << std::endl;
            return 0;
        }
    }
    return 0;
}
