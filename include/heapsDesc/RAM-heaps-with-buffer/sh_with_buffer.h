#include <cassert>
#include "block.h"
#include "head.h"
#include "soft_heap.h"

inline const std::string folder_name = "sh-data";
inline std::size_t block_counter = 0;

template <typename T, size_t BlockSize, typename Compare = std::less<T>>
struct soft_heap_with_buffer {
private:
    Compare comp;

    soft_heap<Head<T, Compare>, HeadCompare<Compare>>
        heads_of_blocks;  //головы блоков на диске

    buffer<T, 3 * BlockSize + 5, Compare> buf;  //буффер для добавленных

    std::size_t size_ = 0;

    void flush_buf() {  //по факту новый блок

        if (buf.size() > BlockSize * 2) {  // TODO: нормальная ли константа?
            Block_t<T, BlockSize> new_bl;
            Head<T, Compare> new_h(comp);

            // наполняем блок
            while (!new_bl.full()) {
                new_bl.add(buf.getMax());
                buf.extractMax();
            }
            ++block_counter;
            new_bl.WRITE(folder_name, block_counter);

            //наполняем голову
            new_h.from_buf(buf);
            new_h.id_tail = block_counter;
            heads_of_blocks.insert(new_h);
        }
    }

    template <typename Comp>
    void buf_to_block(Block_t<T, BlockSize> &to,
                      Head<T, Comp> &h) {  // наполняет блок, бошку и блок
        // связывает, блок пишет, бошку кладёт

        //разбираемся с блоком
        while (!to.full()) {  // buf точно не кончится, иначе не сливаем его в
                              // блок, в нем должно быть BS+3
            to.add(buf.getMax());
            buf.extractMax();
        }
        ++block_counter;

        //а тут голова
        h.from_buf(buf);
        h.id_tail = block_counter;
        heads_of_blocks.insert(h);
    }

    template <std::size_t N>
    void block_to_buf(Block_t<T, N> &from) {  // скинуть блок в буфер

        for (std::size_t i = 0; i < BlockSize; ++i) {
            buf.insert(from[i]);
        }

        flush_buf();  //если переполнен окажется
    }

public:

    soft_heap_with_buffer() = default;
    explicit soft_heap_with_buffer(const Compare &comp_)
        : comp(comp_), heads_of_blocks(HeadCompare<Compare>(comp)), buf(comp) {
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
        // TODO: бросить исключение на пустые
        if (buf.empty()) {
            return heads_of_blocks.getMin().data[0];
        }

        if (heads_of_blocks.empty()) {
            return buf.getMin();
        }

        if (comp(buf.getMin(), heads_of_blocks.getMin().data[0])) {
            return buf.getMin();
        } else {
            return heads_of_blocks.getMin().data[0];
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
                Block_t<T, BlockSize> new_bl;
                new_bl.READ(folder_name, h.id_tail);
                block_to_buf(new_bl);
            } else {
                heads_of_blocks.insert(h);
            }

            return;
        }

        if (comp(buf.getMin(), heads_of_blocks.getMin().data[0])) {
            buf.extractMin();
        } else {
            Head<T, Compare> h = heads_of_blocks.getMin();
            heads_of_blocks.extractMin();
            h.extract();
            if (h.empty()) {
                Block_t<T, BlockSize> new_bl;
                new_bl.READ(folder_name, h.id_tail);
                block_to_buf(new_bl);
            } else {
				
                if (!buf.empty() && comp(buf.getMin(), h[h.size - 1])) {
                    h.add(buf.getMin());
                    buf.extractMin();
                }
				
				if (!buf.empty() && h.size<3 && comp(buf.getMin(), h[h.size - 1])) {
                    h.add(buf.getMin());
                    buf.extractMin();
                }
				
                heads_of_blocks.insert(h);
            }
        }
    }
};
