/*
    Рекомендуется сначала почитать код pairing-heap, там подробнее расписано по комментариям
*/
#include <cassert>
#include "head.h"
#include "soft_heap.h"
inline std::size_t block_counter = 0;
namespace EMHS {
template <typename T, typename Compare = std::less<T>>
struct soft_heap_with_buffer {
private:
    Compare comp;

    soft_heap<Head<T, Compare>, HeadCompare<Compare>>
        heads_of_blocks;  //головы блоков на диске

    std::size_t size_ = 0;
    std::size_t max_size = 0;
    int max_head_size = 3;

    buffer<T, Compare> buf;  //буффер для добавленных

    void flush_buf() {  //по факту новый блок

        if (buf.size() >
            2 * B / sizeof(T) + max_head_size + 5) { 
            Block_t<T> /*<T, B/8>*/ new_bl;
            Head<T, Compare> new_h(max_head_size, comp);

            // наполняем блок
            while (!new_bl.full()) {
                new_bl.add(buf.getMax());
                buf.extractMax();
            }

            ++block_counter;
            WRITE(block_counter, new_bl);

            //наполняем голову
            new_h.from_buf(buf);
            new_h.id_tail = block_counter;
            heads_of_blocks.insert(new_h);
        }
    }

    void block_to_buf(const Block_t<T> &from) {  // скинуть блок в буфер

        for (std::size_t i = 0; i < B / sizeof(T); ++i) {
            buf.insert(from[i]);
        }
        flush_buf();  //если переполнен окажется
    }

public:
    soft_heap_with_buffer() : buf(3 * B / sizeof(T) + 4 * max_head_size, comp) {
    }

    soft_heap_with_buffer(std::size_t max_size_)
        : max_size(max_size_),
          max_head_size(std::min(static_cast<std::size_t>(100),
                                 (max_size * sizeof(T)) / (B * (m - 3)))),
          buf(3 * B / sizeof(T) + 3 * max_head_size, comp){};

    explicit soft_heap_with_buffer(const Compare &comp_)
        : comp(comp_),
          heads_of_blocks(HeadCompare<Compare>(comp)),
          buf(3 * B / sizeof(T) + 3 * max_head_size, comp) {
    }

    explicit soft_heap_with_buffer(std::size_t max_size_, const Compare &comp_)
        : comp(comp_),
          heads_of_blocks(HeadCompare<Compare>(comp)),
          max_size(max_size_),
          max_head_size(std::min(static_cast<std::size_t>(100),
                                 (max_size * sizeof(T)) / (B * (m - 3)))),
          buf(3 * B / sizeof(T) + 3 * max_head_size, comp) {
    }

    [[nodiscard]] bool empty() const {
        return (size_ == 0);
    }

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    void insert(const T &x) {
        ++size_;
        buf.insert(x);

        flush_buf();  //авось переполнило
    }

    const T &getMin() {
        if (buf.empty()) {
            return *(heads_of_blocks.getMin().data.begin());
        }

        if (heads_of_blocks.empty()) {
            return buf.getMin();
        }

        if (comp(buf.getMin(), *(heads_of_blocks.getMin().data.begin()))) {
            return buf.getMin();
        } else {
            return *(heads_of_blocks.getMin().data.begin());
        }
    }

    void extractMin() {
        --size_;

        if (heads_of_blocks.empty()) {
            buf.extractMin();
            return;
        }

        if (buf.empty()) {
            Head<T, Compare> h = heads_of_blocks.getMin();
            heads_of_blocks.extractMin();
            h.extract();
            if (h.empty()) {
                Block_t<T> new_bl;
                // new_bl.READ(folder_name, h.id_tail);
                READ(h.id_tail, new_bl);
                block_to_buf(new_bl);
            } else {
                heads_of_blocks.insert(h);
            }

            return;
        }

        if (comp(buf.getMin(), *(heads_of_blocks.getMin().data.begin()))) {
            buf.extractMin();
        } else {
            Head<T, Compare> h = heads_of_blocks.getMin();
            heads_of_blocks.extractMin();
            h.extract();

            if (h.empty()) {
                Block_t<T> new_bl;
                // new_bl.READ(folder_name, h.id_tail);
                READ(h.id_tail, new_bl);
                block_to_buf(new_bl);

            } else {
                if (!buf.empty() && comp(buf.getMin(), h[h.size() - 1])) {
                    h.add(buf.getMin());
                    buf.extractMin();
                }

                if (!buf.empty() && h.size() < max_head_size &&
                    comp(buf.getMin(), h[h.size() - 1])) {
                    h.add(buf.getMin());
                    buf.extractMin();
                }
                heads_of_blocks.insert(h);
            }
        }
    }
};

template <typename T, typename Compare = std::less<T>>
struct SoftHeap {
private:
    Compare comp;

    soft_heap_with_buffer<T, Compare> heap_of_elements;      //обычные
    soft_heap_with_buffer<T, Compare> heap_of_del_elements;  //удалённые

    void flush_dels() {  //убираем удаленные, пока он сравнимы с top
        while (!heap_of_del_elements.empty() && !heap_of_elements.empty() &&
               heap_of_del_elements.getMin() == heap_of_elements.getMin()) {
            heap_of_del_elements.extractMin();  //убрали
            heap_of_elements.extractMin();      //убрали
        }  //обе не пусты и равны минимумы
    }

public:
    SoftHeap() = default;
    explicit SoftHeap(const Compare &comp_)
        : comp(comp_), heap_of_elements(comp), heap_of_del_elements(comp) {
    }

    explicit SoftHeap(uint64_t max_size, const Compare &comp_)
        : comp(comp_),
          heap_of_elements(max_size, comp),
          heap_of_del_elements(comp) {
    }

    SoftHeap(uint64_t max_size)
        : heap_of_elements(max_size, comp), heap_of_del_elements(comp) {
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

    const T &
    top() {  // Внимание! Метод не константый! Мб произойдёт удаление минимумов
        flush_dels();
        return heap_of_elements.getMin();
    }

    void decreaseKey(const T &x, const T &new_x) {
        heap_of_del_elements.insert(x);
        heap_of_elements.insert(new_x);
    }
};
}  // namespace EMHS