#ifndef DISK_H
#define DISK_H

#include <functional>
#include <sstream>
#include <memory>

namespace EMHS {
    std::size_t B = 30'000;
    std::size_t m = 15;

template <typename T = uint64_t>
struct Block_t {
private:
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
    std::vector<T> data;
public:
    Block_t() : capacity_( B / sizeof(T) ), data(capacity_){
    }

    [[nodiscard]]  uint64_t capacity() const{
        return capacity_;
    }    

    [[nodiscard]] bool empty() const {
        return (size_ == 0);
    }

    [[nodiscard]] bool full() const {
        return (size_ == capacity_);
    }

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    void clear(){
        data.clear();
        size_ = 0;
    }

    void add(const T &x) {
        data[size_++] = x;
    }

    [[nodiscard]] T &operator[](std::size_t Index) {
            return data[Index];
        }

        [[nodiscard]] const T &operator[](std::size_t Index) const {
            return data[Index];
        }

};

//    void (* READ)(uint64_t NextWrite, Block_t &);


} // EMHS



    template<typename T>
    void READ(uint64_t NextWrite, EMHS::Block_t<T> & Block);/* {
        std::unique_ptr<FILE, decltype(& fclose)> File(fopen(NameMake(NextWrite).c_str(), "rb"), & fclose);
        fread(& Block[0], sizeof(T), Block.capacity(), File.get());
        I_COUNTER++;
    }*/
    
    template<typename T>
    void WRITE(uint64_t NextWrite, const EMHS::Block_t<T> & Block);/* {
        //std::cerr<<NameMake(NextWrite);
        std::unique_ptr<FILE, decltype(& fclose)> File(fopen(NameMake(NextWrite).c_str(), "wb"), & fclose);
        fwrite(& Block[0], sizeof(T), Block.capacity(), File.get());
        O_COUNTER++;
    }*/

#endif