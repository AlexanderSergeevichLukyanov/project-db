#include <algorithm>
#include "buffer.h"
template <typename T, typename Compare> struct Head;

template <typename Compare>
struct HeadCompare{
	Compare comp;
	HeadCompare() = default;
	HeadCompare(Compare &comp_): comp(comp_){
	}
	template <typename TT, typename Comp>
	bool operator()(Head<TT, Comp> &h1, Head<TT, Comp> &h2){
		return comp(h1[0], h2[0]);
	}
};



template <typename T, typename Compare=std::less<T>>
struct Head{
	T data[3]{};
	std::size_t id_tail;
	short size=0;
	Compare comp;
	
	Head() = default;
	Head(Compare &comp_): comp(comp_){
		
	}
	
	void add(T x){
		data[size]=x;
		std::sort(data, data+size, comp);
		++size;
	}
	
	template<typename TT, std::size_t k, typename Comp> 
	void from_buf(buffer<TT, k, Comp> &buf){
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
	
	T& operator[](std::size_t ind){
		return data[ind];
	}
	
};