#include "block.h"
#include "head.h"
#include "pairing_heap.h"
#include <cassert>
//#include <set>
inline const std::string folder_name = "ph-data";//  заглушка
inline std::size_t block_counter = 0;//  заглушка
template <typename T, size_t BlockSize, typename Compare = std::less<T>>
struct pairing_heap_with_buffer {
private:
    Compare comp;
	//std::multiset<int, Compare> pr;
    pairing_heap<Head<T, Compare>, HeadCompare<Compare>>
        heads_of_blocks;  //головы блоков на диске
    buffer<T, 3 * BlockSize + 5, Compare> buf;  //буффер для добавленных
	
	std::size_t size_=0;
	
    void flush_buf() {  //по факту новый блок

        if (buf.size() > BlockSize * 2) {  // TODO: нормальная ли константа?
            Block_t<T, BlockSize> new_bl;
            Head<T, Compare> new_h(comp);

            while (!new_bl.full()) {
                new_bl.add(buf.getMax());
                buf.extractMax();
            }
			
            ++block_counter;

            new_bl.WRITE(folder_name, block_counter);  //
            // TODO: запись + связка -- вроде done
			assert(buf.size()>=3);
            new_h.from_buf(buf);
            new_h.id_tail = block_counter;
			heads_of_blocks.insert(new_h);
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

        //std::size_t bl_size = from.size();

        for (std::size_t i = 0; i < BlockSize; ++i) {
            buf.insert(from[i]);  // TODO: к норм
        }

        flush_buf();  //если переполнен окажется
    }

public:
    pairing_heap_with_buffer() = default;
    explicit pairing_heap_with_buffer(const Compare &comp_)
        : comp(comp_),
          heads_of_blocks(HeadCompare<Compare>(comp)),
		  buf(comp)
    // TODO: buffer!
    {
    }

    [[nodiscard]] bool empty() const {
        return (size_==0);
    }

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    void insert(const T &x) {
		//pr.insert(x);
		++size_;
        buf.insert(x);
		//CHECK(*(pr.begin())==buf.getMin());
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
		--size_;
		//CHECK(*(pr.begin())==buf.getMin());
        if (heads_of_blocks.empty()) {
		//	REQUIRE(!pr.empty());
            buf.extractMin();
//			pr.erase(pr.begin());
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
            } else{
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
                if (comp(buf.getMin(), h[h.size - 1])) {
                    h.add(buf.getMin());
                    buf.extractMin();
                }
				heads_of_blocks.insert(h);
            }
        }
    }
};
