#include <algorithm>
#include "buffer.h"
template <typename T, typename Compare>
struct Head;

template <typename Compare>
struct HeadCompare {  // конвертация обычного компаратора в компаратор голов
private:
    Compare comp;

public:
    HeadCompare() = default;
    HeadCompare(const Compare &comp_) : comp(comp_) {
    }

    template <typename T1, typename Comp>
    bool operator()(Head<T1, Comp> &h1, Head<T1, Comp> &h2) {
        return comp(h1[0], h2[0]);
    }
};

template <typename T, typename Compare = std::less<T>>
struct Head {
    T data[3]{};          // данные в голове
    std::size_t id_tail;  // ссылка на хвост в памяти
    short size = 0;       // размер головы
    Compare comp;         //компаратор

    Head() = default;
    Head(const Compare &comp_) : comp(comp_) {
    }

    void add(const T &x) {
        data[size++] = x;
        std::sort(data, data + size, comp);
    }

    template <typename T1, std::size_t k, typename Comp>
    void from_buf(buffer<T1, k, Comp> &buf) {
        for (int i = 2; i > -1; --i) {
            data[i] = buf.getMax();
            buf.extractMax();
            size = 3;
        }
    }

    void extract() {
        data[0] = data[1];
        data[1] = data[2];
        --size;
    }

    [[nodiscard]] bool empty() const {
        return (size == 0);
    }

    T &operator[](std::size_t ind) {
        return data[ind];
    }
};