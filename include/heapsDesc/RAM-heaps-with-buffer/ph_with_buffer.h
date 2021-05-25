#include <cassert>
#include "block.h"
#include "head.h"
#include "pairing_heap.h"

inline const std::string folder_name = "ph-data";
inline std::size_t block_counter = 0;

const int B = 32'000;
const int m = 10;

template <typename T, typename Compare = std::less<T>>
struct pairing_heap_with_buffer {
private:
    Compare comp;

    pairing_heap<Head<T, Compare>, HeadCompare<Compare>>
        heads_of_blocks;  //головы блоков на диске

    std::size_t size_ = 0;
    std::size_t max_size = 0;
    int max_head_size = 3;

    buffer<T, Compare> buf;  //буффер для добавленных

    void flush_buf() {  //по факту новый блок

        if (buf.size()>B/sizeof(T)+max_head_size+5) {  // TODO: к норм виду
            Block_t<T, B/sizeof(T)> new_bl;
            Head<T, Compare> new_h(max_head_size, comp);

            // наполняем блок
            while (!new_bl.full()) {
                new_bl.add(buf.getMax());
                buf.extractMax();
            }
            ++block_counter;
            new_bl.WRITE(folder_name, block_counter);
            // WRITE(fn, new_bl);

            //наполняем голову
            new_h.from_buf(buf);
            new_h.id_tail = block_counter;
            heads_of_blocks.insert(new_h);
        }
    }

    template <std::size_t N>
    void block_to_buf(Block_t<T, N> &from) {  // скинуть блок в буфер

        for (std::size_t i = 0; i < B/sizeof(T); ++i) {
            buf.insert(from[i]);
        }
        flush_buf();  //если переполнен окажется
    }

public:

    pairing_heap_with_buffer(): buf( 2 * B / sizeof(T) + 3 * max_head_size, comp) {
    }

    pairing_heap_with_buffer(std::size_t max_size_) :
    max_size(max_size_),
    max_head_size( std::min(static_cast<std::size_t>(100), ( max_size*sizeof(T) ) / (B * (m - 3)) ) ),
    buf(2*B+max_head_size, comp) {
    };

    explicit pairing_heap_with_buffer(const Compare &comp_)
        : comp(comp_), 
        heads_of_blocks(HeadCompare<Compare>(comp)), 
        buf(2 * B / sizeof(T) + 3 * max_head_size, comp){
    }

    explicit pairing_heap_with_buffer(std::size_t max_size_, const Compare &comp_)
        : comp(comp_), 
        heads_of_blocks(HeadCompare<Compare>(comp)), 
        max_size(max_size_), 
        max_head_size(std::max(static_cast<std::size_t>(100), (B*(m-3)) / (max_size*sizeof(T)) ) ),
        buf(2 * B / sizeof(T) + 3 * max_head_size, comp) {
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

    const T &getMin() const {
        // TODO: бросить исключение на пустые
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
                Block_t<T, B/sizeof(T)> new_bl;
                new_bl.READ(folder_name, h.id_tail);
                block_to_buf(new_bl);
            } else {
                heads_of_blocks.insert(h);
            }

            return;
        }

        if (comp(buf.getMin(), *(heads_of_blocks.getMin().data.begin()) )) {
            buf.extractMin();
        } else {

            Head<T, Compare> h = heads_of_blocks.getMin();
            heads_of_blocks.extractMin();
            h.extract();
            
            if (h.empty()) {

                Block_t<T, B/sizeof(T)> new_bl;
                new_bl.READ(folder_name, h.id_tail);
                block_to_buf(new_bl);

            } else {
				
                if (!buf.empty() && comp(buf.getMin(), h[h.size() - 1])) {
                    h.add(buf.getMin());
                    buf.extractMin();
                }
				
				if (!buf.empty() && h.size()<max_head_size && comp(buf.getMin(), h[h.size() - 1])) {
                    h.add(buf.getMin());
                    buf.extractMin();
                }
                heads_of_blocks.insert(h);
            }
        }
    }
};
