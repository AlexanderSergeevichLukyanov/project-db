#include <cstddef>
#include <string>
#include <type_traits>

inline std::size_t I_COUNTER = 0;
inline std::size_t O_COUNTER = 0;

template <typename T = uint64_t>
struct Block_t {
private:
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
    std::unique_ptr<T[]> data;
public:
    Block_t() : capacity_( B / sizeof(T) ), data(new T[capacity]){
    }

    [[nodiscard]]  uint64_t capacity() const{
        return capacity_;
    }    

    [[nodiscard]] bool empty() const {
        return (size_ == 0);
    }

    [[nodiscard]] bool full() const {
        return (size_ == n);
    }

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    void add(const T &x) {
        data[size_++] = x;
    }

    T &operator[](const size_t ind) {
        return data[ind];
    }

};
