#include "list_heap.h"
#include "mytest.h"
#include <algorithm>
#include <type_traits>
#include <vector>
//#define list_wrong_test
#define list_construct
#define list_insert
#define list_get_min
//#define list_make
#define list_extract_min
#define list_solyanka
#define list_construct_comp
#define list_insert_comp
#define list_get_min_comp
//#define list_make_comp
#define list_extract_min_comp
//#define list_solyanka_comp
namespace{
struct CloserTo {
    private:
        int center_;

    public:
        explicit CloserTo(int center) : center_(center) {
        }
        CloserTo(const CloserTo &) = default;
        CloserTo(CloserTo &&) = default;
        CloserTo &operator=(const CloserTo &) = delete;
        CloserTo &operator=(CloserTo &&) = delete;
        ~CloserTo() = default;
        bool operator()(int a, int b) const {
            return std::abs(a - center_) < std::abs(b - center_);
        }
};

#ifdef list_wrong_test	
TEST_CASE("list-heap: demo-test"){
	CHECK(2*2==5);
}
#endif

#ifdef list_construct
TEST_CASE("list-heap: constructors without Compare"){
	list_heap<int> h1;
	list_heap<double> h2;
	list_heap<std::string> h3;
	//only compile
}
#endif

#ifdef list_insert
TEST_CASE("list-heap: 30000 insert"){
	list_heap<int> h1;
	for(int i=0; i<30000; ++i){
		h1.insert(rand());
	}
	CHECK(h1.size()==30000);
}

TEST_CASE("list-heap: 300000 insert"){
	list_heap<int> h1;
	for(int i=0; i<300000; ++i){
		h1.insert(rand());
	}
	CHECK(h1.size()==300000);
}
#endif

#ifdef list_get_min
TEST_CASE("list-heap: GetMin()"){
	list_heap<int> h1;
	int min=300000;
	for(int i=0; i<300000; ++i){
		int x = rand()%300000;
		if(x<min) min=x;
		h1.insert(x);
	}
	CHECK_MESSAGE(h1.getMin()==min, "generate min is other");
}

TEST_CASE("list-heap: 300000 GetMin()"){
	list_heap<int> h1;
	int min=300000;
	for(int i=0; i<300000; ++i){
		int x = rand()%300000;
		if(x<min) min=x;
		h1.insert(x);
		CHECK_MESSAGE(h1.getMin()==min, "generate min is other");
		if(h1.getMin()!=min) break;
	}
}
#endif

#ifdef list_extract_min
TEST_CASE("list-heap: 3000 insert and 3000 extract_min"){
	list_heap<int> h1;
	std::vector<int> res;
	for(int i=0; i<3000; ++i){
		int x = rand()%3000;
		h1.insert(x);
		res.push_back(x);
	}
	std::sort(res.begin(), res.end());
	CHECK_TIME("After 3000 insert random  and sort random x, then start checking extract_min");
	for(int i=0; i<3000; ++i){
		int x = rand()%3000;
		CHECK(h1.getMin()==res[i]);
		h1.extractMin();
	}
}

TEST_CASE("list-heap: 10000 insert and 10000 extract_min"){
	list_heap<int> h1;
	std::vector<int> res;
	for(int i=0; i<10000; ++i){
		int x = rand()%10000;
		h1.insert(x);
		res.push_back(x);
	}
	std::sort(res.begin(), res.end());
	CHECK_TIME("After 10000 insert random  and sort random x, then start checking extract_min");
	for(int i=0; i<10000; ++i){
		int x = rand()%10000;
		CHECK(h1.getMin()==res[i]);
		h1.extractMin();
	}
}
#endif

#ifdef list_make
TEST_CASE("list-heap: make (30000 elements)"){
	int arr[30000];
	int min=30000;
	for(int i=0; i<30000; ++i){
		int x = rand()%30000;
		if(min>x) min=x;
	}
	h1.make(arr);
	CHECK_MESSAGE(h1.size()==30000, "size heap must be equal to the array size");
	CHECK_MESSAGE(h1.getMin()==min, "min heap must be equal to the array min");
}
TEST_CASE("list-heap: make (300000 elements)"){
	int arr[300000];
	int min=300000;
	for(int i=0; i<300000; ++i){
		int x = rand()%300000;
		if(min>x) min=x;
	}
	h1.make(arr);
	CHECK_MESSAGE(h1.size()==300000, "size heap must be equal to the array size");
	CHECK_MESSAGE(h1.getMin()==min, "min heap must be equal to the array min");
}
#endif

#ifdef list_solyanka
TEST_CASE("list-heap: random test#1"){
	list_heap<int> h1;
	CHECK(h1.empty());
	CHECK(h1.size()==0);
	h1.insert(1);
	CHECK(h1.size()==1);
	CHECK(!h1.empty());
	h1.extractMin();
	CHECK(h1.empty());
	CHECK(h1.size()==0);
	for(int i=0; i<5000; ++i){
		h1.insert(i);
	}
	CHECK(h1.getMin()==0);
	h1.extractMin();
	CHECK(h1.size()==4999);
	CHECK(h1.getMin()==1);
}

TEST_CASE("list-heap: random test#2(check const, reference)"){
	list_heap<int> h1;
	h1.insert(6);
	h1.insert(1);
	CHECK_MESSAGE(std::is_reference<decltype(h1.getMin())>, "method getMin() must be &getMin()");
	CHECK_MESSAGE(std::is_const_v<decltype(h1.getMin())>, "method getMin() must be const");
	CHECK_MESSAGE(std::is_const_v<decltype(h1.size())>, "method size() must be const");
	CHECK_MESSAGE(std::is_const_v<decltype(h1.empty())>, "method empty() must be const");
}

#endif

#ifdef list_construct_comp
TEST_CASE("list-heap: constructors with Compare"){
    list_heap<int, CloserTo> heap(CloserTo(10));
	//only compile
}
#endif

#ifdef list_insert_comp
TEST_CASE("list-heap-with-compare: 30000 insert"){
	list_heap<int, CloserTo> h1(CloserTo(10));
	for(int i=0; i<30000; ++i){
		h1.insert(rand());
	}
	CHECK(h1.size()==30000);
}

TEST_CASE("list-heap: 300000 insert"){
	list_heap<int, CloserTo> h1(CloserTo(10));
	for(int i=0; i<300000; ++i){
		h1.insert(rand());
	}
	CHECK(h1.size()==300000);
}
#endif

#ifdef list_extract_min_comp
TEST_CASE("list-heap: 3000 insert and 3000 extract_min"){
	list_heap<int, CloserTo> h1(CloserTo(10));
	std::vector<int> res;
	for(int i=0; i<3000; ++i){
		int x = rand()%3000;
		h1.insert(x);
		res.push_back(std::abs(x-10));
	}
	std::sort(res.begin(), res.end());
	CHECK_TIME("After 3000 insert random  and sort random x, then start checking extract_min");
	for(int i=0; i<3000; ++i){
		int x = rand()%3000;
		CHECK(std::abs(h1.getMin()-10)==res[i]);
		h1.extractMin();
	}
}

TEST_CASE("list-heap: 10000 insert and 10000 extract_min"){
	list_heap<int, CloserTo> h1(CloserTo(10));
	std::vector<int> res;
	for(int i=0; i<10000; ++i){
		int x = rand()%10000;
		h1.insert(x);
		res.push_back(std::abs(x-10));
	}
	std::sort(res.begin(), res.end());
	CHECK_TIME("After 10000 insert random  and sort random x, then start checking extract_min");
	for(int i=0; i<10000; ++i){
		int x = rand()%10000;
		CHECK(std::abs(h1.getMin()-10)==res[i]);
		h1.extractMin();
	}
}
#endif

#ifdef list_extract_min_comp
TEST_CASE("list-heap: 3000 insert and 3000 extract_min"){
	list_heap<int, CloserTo> h1(CloserTo(10));
	std::vector<int> res;
	for(int i=0; i<3000; ++i){
		int x = rand()%3000;
		h1.insert(x);
		res.push_back(std::abs(x-10));
	}
	std::sort(res.begin(), res.end());
	CHECK_TIME("After 3000 insert random  and sort random x, then start checking extract_min");
	for(int i=0; i<3000; ++i){
		int x = rand()%3000;
		CHECK(std::abs(h1.getMin()-10)==res[i]);
		h1.extractMin();
	}
}

TEST_CASE("list-heap: 10000 insert and 10000 extract_min"){
	list_heap<int, CloserTo> h1(CloserTo(10));
	std::vector<int> res;
	for(int i=0; i<10000; ++i){
		int x = rand()%10000;
		h1.insert(x);
		res.push_back(std::abs(x-10));
	}
	std::sort(res.begin(), res.end());
	CHECK_TIME("After 10000 insert random  and sort random x, then start checking extract_min");
	for(int i=0; i<10000; ++i){
		int x = rand()%10000;
		CHECK(std::abs(h1.getMin()-10)==res[i]);
		h1.extractMin();
	}
}
#endif
}
