#include "pairing_heap.h"
#include "buffer.h"
using namespace std;

template <typename T, size_t BlockSize, typename Compare>
struct pairing_heap_with_buffer{
private:
	pairing_heap<head<T>, Compare> heads_of_blocks; //головы блоков на диске
	buffer<T, 2*BlockSize, Compare> buf; //буффер для добавленных
	
	void flush_buf(){ //по факту новый блок
		
	}
	
	void buf_to_block(buffer<T, BlockSize, Compare> &to, Head &h){ // ссылки не привязывает(
		while(to.size()<BlockSize){ //buf точно не кончится, иначе не сливаем его в блок, в нем должно быть BS+3
			to.insert(buf.getMax());
			buf.extractMax();
		}
		h.e3=buf.getMax();
		buf.extractMax();
		h.e2=buf.getMax();
		buf.extractMax();
		h.e1=buf.getMax();
		buf.extractMax();
		//TODO!!! HEAD! Не здесь, правда
	}
	
	void block_to_buf(buffer<T, BlockSize, Compare> &from){
		while(!from.empty()){
			buf.insert(from.getMin());
			from.extractMin();
		}
		flush_buf();
	}
	
	void file_to_block(const std::string &filename, buffer<T, BlockSize, Compare> &to){
		T* data;
		read(filename, data);
		to.make(data); 
	}
	
	void block_to_file(const std::string &filename, buffer<T, BlockSize, Compare> &from){
		//todo
	}
	
	
	
public:

	[[nodiscard]] bool empty() const{
		return (heads_of_blocks.empty() && buf.empty());
	}
	
	[[nodiscard]] size_t size() const{
		return (heads_of_blocks.size() + buf.size());
	}
	
	void insert(T &x){
		buf.insert(x);
		if(buf.size()>..){
			//TODO: псевдокод
			new block new_bl;
			new Head h;
			buf_to_block(new_bl, h){
			}
			link(h, new_bl);
			WRITE("dn", "..", new_bl);
		}
	}
	
	const T& getMin() const{
		//TODO: бросить исключение на пустые
		if(buf.empty()){
			return heads_of_blocks.getMin().n1;
		}
		
		if(heads_of_blocks.empty()){
			return buf.n1;
		}
		
		if(comp(buf.getMin(), heads_of_blocks.getMin().n1){
			return buf.getMin();
		} else{
			return heads_of_blocks.getMin().n1;
		}
	}
	
	void extractMin(){
		
		if(buf.empty()){
			Head h = heads_of_blocks.getMin();
			heads_of_blocks.extractMin();
			h.n1=h.n2;
			h.n2=h.n3;
			h.n3=NULL;
			--h.size;
			if(h.empty()){
				//psevdo
				READ;
				block_to_buf();
			} else{ 
				//todo: пополнение из буфера
				heads_of_blocks.insert(h);
			}
		//	return heads_of_blocks.getMin().n1;
		}
		
		if(heads_of_blocks.empty()){
			buf.extractMin();
			//return buf.n1;
		}
		
		if(comp(buf.getMin(), heads_of_blocks.getMin().n1){
			buf.extractMin();
		} else{
			//todo: что и в первом случае
			//return heads_of_blocks.getMin().n1;
		}
	}
	/** TODO: тут еще поколдовать надо над методами, они тривиальны, просто куча + буффер и немного алгоритмической магии) */
}


template <typename T, size_t BlockSize, typename Compare =  std::less<T>> /** TODO: мб BlockSize считать автоматом от T здесь?! */
struct PairingHeap{
private:
	Compare comp;

	pairing_heap_with_buffer<T, Compare> heap_of_elements; //обычные
	pairing_heap_with_buffer<T, Compare> heap_of_del_elements; //удалённые
	
	void flush_dels(){ //убираем удаленные, пока он сравнимы с максимальными
		if(!heap_of_del_elements().empty() && !heap_of_del_elements.empty() && heap_of_del_elements.getMin() == heap_of_elements.getMin()){ 
			heap_of_del_elements.extractMin(); //убрали
			heap_of_elements.extractMin(); //убрали
			flush_dels(); // рекусивно продолжаем убиарть всё это
		} //обе не пусты и равны минимумы
	}

public:

	/** TODO: конструктор*/
	
	[[nodiscard]] bool empty() const{ //готово
		return (heap_of_elements.size() == heap_of_del_elements.size()); //количество удаленных равно количеству добавленных => пусто
	}
	
	[[nodiscard]] size_t size() const{ //готово
		return heap_of_del_elements.size() - heap_of_del_elements.size(); //количество добавленных мину количество удаленных
	}
	
	void insert(const T& x){
		heap_of_elements.insert(x);
	}
	
	void del(const T& x){
		heap_of_del_elements.insert(x);
	}
	
	void extractMin(){ // pop
		flush_dels();
		heap_of_elements.extractMin();
	}
	
	const T& getMin() const{ // top
		flush_dels();
		return heap_of_elements.getMin();
	}
	
	void decreaseKey(T& x, T& new_x){
		heap_of_del_elements.insert(x);
		heap_of_elements.insert(new_x);
	}
}