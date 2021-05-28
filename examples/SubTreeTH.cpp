#include "EMHS.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <sys/stat.h>

struct Calculator {
    [[nodiscard]] uint64_t operator()(EMHS::Package_t & Package) {
        uint64_t Result = 1;
        while (!Package.empty()) {
            Result += Package.top();
            Package.extract();
        }
        return Result;
    }
};

int main() {
    EMHS::m = 15;
    EMHS::B = 512;
    std::size_t MaxSize = 10000;
    for (std::size_t Test = 0; Test < 10; Test++) {
        std::vector<std::size_t> Tree(rand() % MaxSize + 1);
        for (std::size_t Index = 0; Index < Tree.size() - 1; Index++) {
            Tree[Index] = Index + 1 + (rand() % (Tree.size() - Index - 1));
        }
        std::vector<uint64_t> InCoreResult(Tree.size(), 1);
        for (std::size_t Index = 0; Index < Tree.size() - 1; Index++) {
            InCoreResult[Tree[Index]] += InCoreResult[Index];
        }
        std::vector<uint64_t> Input;
        Input.push_back(Tree.size());
        Input.push_back(Tree.size() - 1);
        for (std::size_t Index = 0; Index < Tree.size() - 1; Index++) {
            Input.push_back(Index);
            Input.push_back(Tree[Index]);
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
        EMHS::dp(1000000000, 2000000000, EMHS::TowerHeap_t<std::pair<uint64_t, uint64_t>>{0}, Calculator{}); //{0, 1024}
        std::cout << "Process finished." << std::endl;
        std::cout << "Total input operations: " << I_COUNTER << "." << std::endl;
        std::cout << "Total output operations: " << O_COUNTER << "." << std::endl;
        std::size_t OutputSize = Tree.size() + 1;
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
        std::vector<uint64_t> OutCoreResult(Tree.size());
        for (std::size_t Index = 0; Index < Tree.size(); Index++) {
            OutCoreResult[Index] = Output[Index + 1];
        }
        if (Tree.size() == Output[0] && InCoreResult == OutCoreResult) {
            std::cout << "Ok." << std::endl;
            std::cout << Tree.size() << std::endl;
            /*for (std::size_t Index = 0; Index < Tree.size(); Index++) {
                std::cout << InCoreResult[Index] << " ";
            }*/
            std::cout << std::endl;
        } else {
            std::cout << "Pizdec. O_o" << std::endl;
            exit(1);
            return 0;
        }
    }
    return 0;
}