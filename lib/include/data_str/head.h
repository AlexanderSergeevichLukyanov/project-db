#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <iterator>
#include <list>
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
    int capacity = 0;
    std::list<T> data;// данные в голове
    std::size_t id_tail;  // ссылка на хвост в памяти
    Compare comp;         //компаратор

    Head() = delete;
    Head(int size_head) : capacity(size_head) {
    }
    Head(int size_head, const Compare &comp_) : capacity(size_head), comp(comp_) {
    }

    void add(const T &x) { //TODO
        data.push_back(x);
        data.sort(comp);
    }

    template <typename T1, typename Comp>
    void from_buf(buffer<T1, Comp> &buf) {
        for (int _ = 0; _ < capacity; ++_) {
            data.push_front(buf.getMax());
            buf.extractMax();
        }
        //data.sort(comp);
    } 

    void extract() {
        data.pop_front();
    }

    [[nodiscard]] bool empty() const {
        return data.empty();
    }

    [[nodiscard]] std::size_t size() const {
        return data.size();
    }

    T &operator[](std::size_t ind) {
        auto it = data.begin();
        std::advance(it, ind);
        return *it;
    }
};