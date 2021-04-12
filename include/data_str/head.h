template <typename T, Compare comp>
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

bool operator <(Head &h1, Head &h2){
	return (h1.data[0]<h2.data[0]);
}