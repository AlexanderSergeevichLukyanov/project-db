#define _LARGE_FILE_API
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <unordered_set>
#include <iterator>

namespace EMHS {
class DISK {
private:
    std::unordered_map<std::string, int>
        open_descriptors;  
    std::unordered_map<uint64_t, int> open_smart_descriptors;
    std::unordered_set<int> utilized;

    std::string NameMake(uint64_t NextWrite) {
        std::stringstream StringStream;
        StringStream << "lib/DISK/" << NextWrite;
        std::string Name;
        StringStream >> Name;
        return Name;
    }

public:
    DISK() = default;

    void SMART_WRITE(uint64_t NextWrite, char *data, std::size_t len) {
        int file_descriptor = -1;
        if (!utilized.empty()) {
            file_descriptor = *(utilized.begin());
            utilized.erase(file_descriptor);
        } else {
            file_descriptor =
                open64(NameMake(NextWrite).c_str(), O_RDWR | O_CREAT, 0777);
        }
        open_smart_descriptors[NextWrite] = file_descriptor;
        if (file_descriptor == -1) {
            std::cerr << "Ошибка при открытии/создании файла\n";
            exit(1);
        }
        int ret_code = pwrite64(file_descriptor, data, len, 0);
        if (ret_code == -1) {
            std::cerr << "Ошибка при записи на диск\n";
            exit(1);
        }
        fdatasync(file_descriptor);
    }

    void SMART_READ(uint64_t NextWrite, char *data, std::size_t len) {
        assert(open_smart_descriptors.count(NextWrite));
        int file_descriptor = open_smart_descriptors[NextWrite];
        if (file_descriptor == -1) {
            std::cerr << "Ошибка при открытии/создании файла\n";
            exit(1);
        }
        int ret_code = pread64(file_descriptor, data, len, 0);
        if (ret_code == -1) {
            std::cerr << "Ошибка при чтении с диска\n";
            exit(1);
        }
        fdatasync(file_descriptor);
        open_smart_descriptors.erase(NextWrite);
        utilized.insert(file_descriptor);
    }

    void WRITE_DISK(const char *filename, char *data, std::size_t len) {
        int file_descriptor = open64(filename, O_RDWR | O_CREAT, 0777);
        open_descriptors[std::string(filename)] = file_descriptor;
        if (file_descriptor == -1) {
            std::cerr << "Ошибка при открытии/создании файла\n";
            exit(1);
        }
        int ret_code = pwrite64(file_descriptor, data, len, 0);
        if (ret_code == -1) {
            std::cerr << "Ошибка при записи на диск\n";
            exit(1);
        }
        fdatasync(file_descriptor);
    }
    void READ_DISK(const char *filename, char *data, std::size_t len) {
        int file_descriptor = open_descriptors[std::string(filename)];
        if (file_descriptor == -1) {
            std::cerr << "Ошибка при открытии/создании файла\n";
            exit(1);
        }
        int ret_code = pread64(file_descriptor, data, len, 0);
        if (ret_code == -1) {
            std::cerr << "Ошибка при чтении с диска\n";
            exit(1);
        }
        fdatasync(file_descriptor);

        // unlink(filename); // он вроде как больше не нужен после чтения
    }

    ~DISK() {
        for (auto it : open_descriptors) {
            close(it.second);
        }

        std::unordered_set<int> :: iterator itr;
        for (itr = utilized.begin(); itr != utilized.end(); itr++){
            close(*itr);
        }
    }
} d;
}  // namespace EMHS
std::size_t I_COUNTER{};
std::size_t O_COUNTER{};

std::string NameMake(uint64_t NextWrite) {
    std::stringstream StringStream;
    StringStream << "lib/DISK/" << NextWrite;
    std::string Name;
    StringStream >> Name;
    return Name;
}

template <typename T>
void READ(uint64_t NextWrite, EMHS::Block_t<T> &Block) {
    std::unique_ptr<FILE, decltype(&fclose)> File(
        fopen(NameMake(NextWrite).c_str(), "rb"), &fclose);
    fread(&Block[0], sizeof(T), Block.capacity(), File.get());
    // EMHS::d.SMART_READ(NextWrite, (char*)&Block[0],
    // sizeof(T)*Block.capacity()); -- это более быстрое чтение
    // EMHS::d.READ_DISK(NameMake(NextWrite).c_str(), (char*)&Block[0],
    // sizeof(T)*Block.capacity());
    I_COUNTER++;
}

template <typename T>
void WRITE(uint64_t NextWrite, const EMHS::Block_t<T> &Block) {
    std::unique_ptr<FILE, decltype(&fclose)> File(
        fopen(NameMake(NextWrite).c_str(), "wb"), &fclose);
    fwrite(&Block[0], sizeof(T), Block.capacity(), File.get());
    // EMHS::d.WRITE_DISK(NameMake(NextWrite).c_str(), (char*)&Block[0],
    // sizeof(T)*Block.capacity()); EMHS::d.SMART_WRITE(NextWrite,
    // (char*)&Block[0], sizeof(T)*Block.capacity());
    O_COUNTER++;
}
