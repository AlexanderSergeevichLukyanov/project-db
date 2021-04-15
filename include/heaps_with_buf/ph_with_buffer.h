#include "block.h"
#include "head.h"
#include "pairing_heap.h"
const std::string folder_name = "ph-data";//  заглушка
std::size_t block_counter = 0;//  заглушка
template <typename T, size_t BlockSize, typename Compare = std::less<T>>
struct pairing_heap_with_buffer {
private:
    Compare comp;

    pairing_heap<Head<T, Compare>, HeadCompare<Compare>>
        heads_of_blocks;  //головы блоков на диске
    buffer<T, 3 * BlockSize, Compare> buf;  //буффер для добавленных

    void flush_buf() {  //по факту новый блок

        if (buf.size() > BlockSize * 2) {  // TODO: нормальная ли константа?

            Block_t<T, BlockSize> new_bl;
            Head<T, Compare> new_h(comp);

            while (!new_bl.full()) {
                new_bl.add(buf.getMax());  // TODO: к 64
                buf.extractMax();
            }

            ++block_counter;

            new_bl.WRITE(folder_name, block_counter);  //
            // TODO: запись + связка -- вроде done
            new_h.from_buf(buf);
            new_h.id_tail = block_counter;
            // TODO: положить бошку в кучу -- вроде done
        }
    }
    template <typename C>
    void buf_to_block(Block_t<T, BlockSize> &to,
                      Head<T, C> &h) {  // наполняет блок, бошку и блок
                                        // связывает, блок пишет, бошку кладёт

        while (!to.full()) {  // buf точно не кончится, иначе не сливаем его в
                              // блок, в нем должно быть BS+3
            to.add(buf.getMax());  // TODO: к 64
            buf.extractMax();
        }

        ++block_counter;

        h.from_buf(buf);
        h.id_tail = block_counter;

        heads_of_blocks.insert(h);
    }

    template <size_t nn>
    void block_to_buf(Block_t<T, nn> &from) {  // TODO: ускорить в 2 раза, пока
                                               // даже логике не подчиняется

        std::size_t bl_size = from.size();

        for (std::size_t i = 0; i < bl_size; ++i) {
            buf.insert(from[i]);  // TODO: к норм
        }

        flush_buf();  //если переполнен окажется
    }

public:
    pairing_heap_with_buffer() = default;
    pairing_heap_with_buffer(const Compare &comp_)
        : comp(comp_),
          heads_of_blocks(HeadCompare<Compare>(comp_)),
		  buf(comp_)
    // TODO: buffer!
    {
    }

    [[nodiscard]] bool empty() const {
        return (heads_of_blocks.empty() && buf.empty());
    }

    [[nodiscard]] std::size_t size() const {
        return (heads_of_blocks.size() + buf.size());
    }

    void insert(const T &x) {
        buf.insert(x);
        flush_buf();  //авось переполнило
    }                 // вроде done

    const T &getMin() const {
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
    }  // вроде done

    void extractMin() {
        if (heads_of_blocks.empty()) {
            buf.extractMin();
            return;
        }

        if (buf.empty()) {
            Head h = heads_of_blocks.getMin();
            heads_of_blocks.extractMin();
            h.extract();
            if (h.empty()) {
                Block_t<T, BlockSize> new_bl;
                new_bl.READ(folder_name, h.id_tail, BlockSize);
                block_to_buf(new_bl);
            }
            return;
        }

        if (comp(buf.getMin(), heads_of_blocks.getMin().data[0])) {
            buf.extractMin();
        } else {
            Head h = heads_of_blocks.getMin();
            heads_of_blocks.extractMin();
            h.extract();
            if (h.empty()) {
                Block_t<T, BlockSize> new_bl;
                new_bl.READ(folder_name, h.id_tail);
                block_to_buf(new_bl);
            } else {
                if (comp(buf.getMin(), h[h.size - 1])) {
                    h.add(buf.getMin());
                    buf.extractMin();
                }
            }
        }
    }
};
