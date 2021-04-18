#include "ph_with_buffer.h"

inline const std::size_t B = 1024;

template <typename T, typename Compare = std::less<T>>
struct PairingHeap {
private:
    Compare comp;

    pairing_heap_with_buffer<T, B / sizeof(T), Compare>
        heap_of_elements;  //обычные
    pairing_heap_with_buffer<T, B / sizeof(T), Compare>
        heap_of_del_elements;  //удалённые

    void flush_dels() {  //убираем удаленные, пока он сравнимы с top
        while (!heap_of_del_elements.empty() && !heap_of_elements.empty() &&
               heap_of_del_elements.getMin() == heap_of_elements.getMin()) {
            heap_of_del_elements.extractMin();  //убрали
            heap_of_elements.extractMin();      //убрали
        }  //обе не пусты и равны минимумы
    }

public:
    PairingHeap() = default;

    explicit PairingHeap(const Compare &comp_)
        : comp(comp_), heap_of_elements(comp), heap_of_del_elements(comp) {
    }

    [[nodiscard]] bool empty() const {
        return (heap_of_elements.size() ==
                heap_of_del_elements.size());  //количество удаленных равно
                                               //количеству добавленных => пусто
    }

    [[nodiscard]] size_t size() const {
        return heap_of_elements.size() -
               heap_of_del_elements
                   .size();  //количество добавленных мину количество удаленных
    }

    void add(const T &x) {
        heap_of_elements.insert(x);
    }

    void del(const T &x) {
        heap_of_del_elements.insert(x);
    }

    void extract() {
        flush_dels();
        heap_of_elements.extractMin();
    }

    const T &top() {
        flush_dels();
        return heap_of_elements.getMin();
    }

    void decreaseKey(const T &x, const T &new_x) {
        heap_of_del_elements.insert(x);
        heap_of_elements.insert(new_x);
    }
};