#include "pairing_heap.h"
#include "buffer.h"
using namespace std;

const std::string folder_name = "ph-data";
std::size_t block_counter = 0;


template <typename T>
struct Head{
	T data[3]{};
	std::size_t id_tail;
	short size=0;
	
	void add(T &x){
		h[size]=x;
		std::sort(data, data+size);
		++size;
	}
	
	void from_buf(buffer &b){
		for(int i=2; i>-1; --i){
			data[i]=buf.getMax();
			buf.extractMax();
			size=3;
		}
	}
	
	void extract(){
		data[0]=data[1];
		data[1]=data[2];
		--size;
	}
	
	[[nodiscard]] bool empty() const{
		return (size==0);
	}
	
};

template <typename T, size_t BlockSize, typename Compare>
struct pairing_heap_with_buffer{
private:
	pairing_heap<head<T>, Compare> heads_of_blocks; //головы блоков на диске
	buffer<T, 2*BlockSize, Compare> buf; //буффер для добавленных
	
	void flush_buf() { //по факту новый блок
		
		if(buf.size()>BlockSize*3/2){ //TODO: нормальная ли константа?
		
			Block_t new_bl;
			Head new_h;
			
			while(!new_bl.full()){ 
				new_bl.add(buf.getMax()); //TODO: к 64
				buf.extractMax();
			}
			
			++block_counter;
			
			WRITE(folder_name, block_counter, new_bl);//
			//TODO: запись + связка -- вроде done
			new_h.from_buf(buf);
			h.href_to_tail=block_counter;
			//TODO: положить бошку в кучу -- вроде done
		}
	
	}
	
	void buf_to_block(Block_t &to, Head &h) { // наполняет блок, бошку и блок связывает, блок пишет, бошку кладёт
		
		while(!to.full()){ //buf точно не кончится, иначе не сливаем его в блок, в нем должно быть BS+3
			to.add(buf.getMax()); //TODO: к 64
			buf.extractMax();
		}
		
		++block_counter;
			
		WRITE(folder_name, block_counter, new_bl);

		h.from_buf(buf);
		h.href_to_tail=block_counter;

		heads_of_blocks.insert(h);
	}
	
	void block_to_buf(Block_t &from){ //TODO: ускорить в 2 раза, пока даже логике не подчиняется
		
		std::size_t bl_size = from.size();
		
		for(std::size_t i = 0; i < bl_size; ++i){
			buf.insert(from[i]); //TODO: к норм
		}
		
		flush_buf(); //если переполнен окажется
	
	}
	
	/*void file_to_block(const std::string &filename, buffer<T, BlockSize, Compare> &to){
		T* data;
		read(filename, data);
		to.make(data); 
	}
	
	void block_to_file(const std::string &filename, buffer<T, BlockSize, Compare> &from){
		//todo
	}*/
	
	
	
public:

	[[nodiscard]] bool empty() const{
		return (heads_of_blocks.empty() && buf.empty());
	}
	
	[[nodiscard]] std::size_t size() const{
		return (heads_of_blocks.size() + buf.size());
	}
	
	void insert(T &x){
		buf.insert(x);
		flush_buf(); //авось переполнило
	} // вроде done
	
	const T& getMin() const{
		//TODO: бросить исключение на пустые
		if(buf.empty()){
			return heads_of_blocks.getMin().data[0];
		}
		
		if(heads_of_blocks.empty()){
			return buf.getMin();
		}
		
		if(comp(buf.getMin(), heads_of_blocks.getMin().data[0]){
			return buf.getMin();
		} else{
			return heads_of_blocks.getMin().data[0];
		}
	} // вроде done
	
	void extractMin(){
		
		if(heads_of_blocks.empty()){
			buf.extractMin();
			return;
		}
		
		if(buf.empty()){
			Head h = heads_of_blocks.getMin();
			heads_of_blocks.extractMin();
			h.extract();
			if(h.empty()){
				Block_t new_bl;
				READ(folder_name, h.id_tail, new_bl);
				block_to_buf(new_bl);
			} 
			return;
		} 
		//	return heads_of_blocks.getMin().n1;
		
		if(comp(buf.getMin(), heads_of_blocks.getMin().data[0]){
			buf.extractMin();
		} else{
			Head h = heads_of_blocks.getMin();
			heads_of_blocks.extractMin();
			h.extract();
			if(h.empty()){
				Block_t new_bl;
				READ(folder_name, h.id_tail, new_bl);
				block_to_buf(new_bl);
			} else {
				if(comp(buf.getMin(), h[h.size-1])){
					h.add(buf.getMin());
					buf.extractMin();
				} 
			}
		}
	} //вроде done
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
	
	void pop(){ // pop
		flush_dels();
		heap_of_elements.extractMin();
	}
	
	const T& top() const{ // top
		flush_dels();
		return heap_of_elements.getMin();
	}
	
	void decreaseKey(T& x, T& new_x){
		heap_of_del_elements.insert(x);
		heap_of_elements.insert(new_x);
	}
}