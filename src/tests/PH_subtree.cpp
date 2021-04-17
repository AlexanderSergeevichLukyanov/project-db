#include "mytest.h"
#include "PairingHeap.h"
#include "EMHS.h"

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

TEST_CASE("Subtree"){
	EMHS::m = 3;
    EMHS::B = 48;
    std::size_t MaxSize = 20;
    for (std::size_t Test = 0; Test < 100000; Test++) {
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
        remove("Input");
        mkdir("Input", ACCESSPERMS);
        for (std::size_t Index = 0; Index < Input.size() / (EMHS::B / 8); Index++) {
            std::stringstream StringStream;
            StringStream << "Input/" << Index;
            std::string Name;
            StringStream >> Name;
            std::unique_ptr<FILE, decltype(&fclose)> File(fopen(Name.c_str(), "wb"), &fclose);
            for (std::size_t IndexFile = 0; IndexFile < EMHS::B / 8; IndexFile++) {
                fwrite(&Input[Index * (EMHS::B / 8) + IndexFile], 8, 1, File.get());
            }
        }
        EMHS::dp("Input", "Output", PairingHeap<std::pair<uint64_t, uint64_t>>{}, Calculator{});
        std::size_t OutputSize = Tree.size() + 1;
        while (OutputSize % (EMHS::B / 8) != 0) {
            OutputSize++;
        }
        std::vector<uint64_t> Output(OutputSize);
        for (std::size_t Index = 0; Index < OutputSize / (EMHS::B / 8); Index++) {
            std::stringstream StringStream;
            StringStream << "Output/" << Index;
            std::string Name;
            StringStream >> Name;
            std::unique_ptr<FILE, decltype(&fclose)> File(fopen(Name.c_str(), "rb"), &fclose);
            for (std::size_t IndexFile = 0; IndexFile < EMHS::B / 8; IndexFile++) {
                fread(&Output[Index * (EMHS::B / 8) + IndexFile], 8, 1, File.get());
            }
        }
        std::vector<uint64_t> OutCoreResult(Tree.size());
        for (std::size_t Index = 0; Index < Tree.size(); Index++) {
            OutCoreResult[Index] = Output[Index + 1];
        }
		CHECK_MESSAGE(Tree.size() == Output[0], "sizes not equal!");
        if (Tree.size() == Output[0] && InCoreResult == OutCoreResult) {
            std::cerr << "Ok." << std::endl;
            std::cerr << Tree.size() << std::endl;
            for (std::size_t Index = 0; Index < Tree.size(); Index++) {
                std::cerr << InCoreResult[Index] << " ";
            }
            std::cerr << std::endl;
        } else {
            CHECK_MESSAGE(false, "Pizdec. O_o");
        }
    }
    //return 0;
}