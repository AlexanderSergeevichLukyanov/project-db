#define _LARGE_FILE_API
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
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


namespace EMHS{
/*    class DISK{
    private:
        std::unordered_map<int, int> open_descriptors; // по NextWrite пользователя
        std::unordered_set<int> utilized;
    public:
        DISK() = default;

    void WRITE_DISK(char * filename, char * data, std::size_t len){
        if(!utilized.empty()){

        }
        int file_descriptor = open64(filename, O_RDWR|O_CREAT, 0777);
        if(file_descriptor == -1){
            std::cerr << "Ошибка при открытии/создании файла\n";
            exit(1);
        }    
        int ret_code = pwrite64(file_descriptor, data, len, 0);
        if(ret_code == -1){
            std::cerr << "Ошибка при записи на диск\n";
            exit(1);
        }
        fdatasync(file_descriptor);
        
    }
    void READ_DISK(char * filename, char * data, std::size_t len){

        int file_descriptor = open64(filename, O_RDWR|O_CREAT, 0777);
        if(file_descriptor == -1){
            std::cerr << "Ошибка при открытии/создании файла\n";
            exit(1);
        }    
        int ret_code = pread64(file_descriptor, data, len, 0);
        if(ret_code == -1){
            std::cerr << "Ошибка при чтении с диска\n";
            exit(1);
        }
        fdatasync(file_descriptor);
        // unlink(filename); // он вроде как больше не нужен после чтения
    }


        ~DISK(){
            for (auto it : open_descriptors){
                fclose(it.second);
            }

            for (auto it : utilized){
                fclose(*it);
            }
        }
    }
    /*
    // TODO класс, запоминающий файловый дескриптор
    конструктор --
    деструктор - закрыть
// TODO: класть на стек все файлы, а в конце main удалить=
    void WRITE_DISK(char * filename, char * data, std::size_t len){
        int file_descriptor = open64(filename, O_RDWR|O_CREAT, 0777);
        if(file_descriptor == -1){
            std::cerr << "Ошибка при открытии/создании файла\n";
            exit(1);
        }    
        int ret_code = pwrite64(file_descriptor, data, len, 0);
        if(ret_code == -1){
            std::cerr << "Ошибка при записи на диск\n";
            exit(1);
        }
        fdatasync(file_descriptor);
        
    }
    void READ_DISK(char * filename, char * data, std::size_t len){
        int file_descriptor = open64(filename, O_RDWR|O_CREAT, 0777);
        if(file_descriptor == -1){
            std::cerr << "Ошибка при открытии/создании файла\n";
            exit(1);
        }    
        int ret_code = pread64(file_descriptor, data, len, 0);
        if(ret_code == -1){
            std::cerr << "Ошибка при чтении с диска\n";
            exit(1);
        }
        fdatasync(file_descriptor);
        // unlink(filename); // он вроде как больше не нужен после чтения
    }
    */
}