#define _LARGE_FILE_API
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace EMHS{
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
        close(file_descriptor);
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
        close(file_descriptor);
        // unlink(filename); // он вроде как больше не нужен после чтения
    }
}