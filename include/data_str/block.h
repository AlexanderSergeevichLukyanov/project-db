template <typename T, std::size_t n>
struct Block_t{
private:
	T data[n];
	std::size_t size_=0;
public:
	[[nodiscard]] bool empty() const{
		return (size_==0);
	}
	
	[[nodiscard]] bool full() const{
		return (size_==n);
	}
	
	[[nodiscard]] std::size_t size() const{
		return size_;
	}
	
	void add(T &x){
		data[size_++]=x;
	}
	
	void WRITE(std::string DirectoryName, int NextWrite) {
		FILE *f;
		f=fopen((DirectoryName+"/"+std::to_string(NextWrite)).c_str(), "wb+");
        fwrite(data, sizeof(T), size_, f);
	//	++I_COUNTER;
    }

    void READ(std::string DirectoryName, int NextWrite, std::size_t k) {
        FILE *f;
		size_=k;
		f=fopen((DirectoryName+"/"+std::to_string(NextWrite)).c_str(), "wb+");
        fread(data, sizeof(T), size_, f);
    //    ++O_COUNTER;
    }
	
	T& operator [](const size_t ind){
		return data[ind];
	}
};
