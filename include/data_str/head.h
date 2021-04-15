#include <algorithm>
template <typename T, typename Compare> struct Head;

template <typename Compare>
struct HeadCompare{
	Compare comp;
	HeadCompare() = default;
	HeadCompare(Compare &comp_): comp(comp_){
	}
	bool operator()(Head<auto, auto> &h1, Head<auto, auto> &h2){
		return comp(h1[0], h2[0]);
	}
};

template <typename T, typename Compare=HeadCompare<std::less<T>> >
struct Head{
	T data[3]{};
	std::size_t id_tail;
	short size=0;
	Compare comp;
	
	Head() = default;
	Head(Compare &comp_): comp(comp_){
		
	}
	
	void add(T &x){
		h[size]=x;
		std::sort(data, data+size, comp);
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
	
	T& operator[](std::size_t ind){
		return data[ind];
	}
	
};