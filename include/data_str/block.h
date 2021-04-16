#include <cstddef>
#include <string>
#include <type_traits>

inline std::size_t I_COUNTER = 0;
inline std::size_t O_COUNTER = 0;

template <typename T, std::size_t n>
struct Block_t {
private:
    T data[n];
    std::size_t size_ = 0;

public:
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

    void WRITE(std::string DirectoryName, int NextWrite) {
        FILE *f = fopen(
            (DirectoryName + "/" + std::to_string(NextWrite) + ".bin").c_str(),
            "wb");
        fwrite(data, sizeof(T), size_, f);
        fclose(f);
        ++I_COUNTER;
    }

    void READ(std::string DirectoryName, int NextWrite, std::size_t k = n) {
        FILE *f;
        size_ = k;
        f = fopen(
            (DirectoryName + "/" + std::to_string(NextWrite) + ".bin").c_str(),
            "rb");
        fread(data, sizeof(T), size_, f);
        fclose(f);
        ++O_COUNTER;
    }

    T &operator[](const size_t ind) {
        return data[ind];
    }
};
