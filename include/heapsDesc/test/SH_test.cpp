#include <algorithm>
#include <type_traits>
#include <vector>
#include <set>
#include "mytest.h"
#include "SoftHeap.h"

namespace {
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


TEST_CASE("soft-heap: constructors without Compare") {
    SoftHeap<int> h1;
    SoftHeap<double> h2;
    SoftHeap<std::string> h3;
    // only compile
}



TEST_CASE("soft-heap: 30000 add") {
    SoftHeap<int> h1;
    for (int i = 0; i < 30000; ++i) {
        h1.add(rand());
    }
    CHECK(h1.size() == 30000);
}

TEST_CASE("soft-heap: 300000 add") {
    SoftHeap<int> h1;
    for (int i = 0; i < 300000; ++i) {
        h1.add(rand());
    }
    CHECK(h1.size() == 300000);
}



TEST_CASE("soft-heap: top()") {
    SoftHeap<int> h1;
    int min = 300000;
    for (int i = 0; i < 300000; ++i) {
        int x = rand() % 300000;
        if (x < min)
            min = x;
        h1.add(x);
    }
    CHECK_MESSAGE(h1.top() == min, "generate min is other");
}

TEST_CASE("soft-heap: 300000 top()") {
    SoftHeap<int> h1;
    int min = 300000;
    for (int i = 0; i < 300000; ++i) {
        int x = rand() % 300000;
        if (x < min)
            min = x;
        h1.add(x);
        CHECK_MESSAGE(h1.top() == min, "#"+std::to_string(i)+": generate min is other: "+std::to_string(h1.top())+" != "+std::to_string(min));
        if (h1.top() != min)
            break;
    }
}

TEST_CASE("soft-heap: 3000 add and 3000 extract_min") {
    SoftHeap<int> h1;
    std::vector<int> res;
    for (int i = 0; i < 3000; ++i) {
        int x = rand() % 3000;
        h1.add(x);
        res.push_back(x);
    }
    std::sort(res.begin(), res.end());
    CHECK_TIME(
        "After 3000 add random  and sort random x, then start checking "
        "extract_min");
    for (int i = 0; i < 3000; ++i) {
        CHECK(h1.top() == res[i]);
        h1.extract();
    }
}

TEST_CASE("soft-heap: 10000 add and 10000 extract_min") {
    SoftHeap<int> h1;
    std::vector<int> res;
    for (int i = 0; i < 10000; ++i) {
        int x = rand() % 10000;
        h1.add(x);
        res.push_back(x);
    }
    std::sort(res.begin(), res.end());
    CHECK_TIME(
        "After 10000 add random  and sort random x, then start checking "
        "extract_min");
    for (int i = 0; i < 10000; ++i) {
        CHECK(h1.top() == res[i]);
        h1.extract();
    }
}


/*
TEST_CASE("soft-heap: make (30000 elements)") {
    SoftHeap<int> h1;
    int arr[30000];
    int min = 30000;
    for (int i = 0; i < 30000; ++i) {
        int x = rand() % 30000;
        arr[i] = x;
        if (min > x)
            min = x;
    }
    h1.make(arr, 30000);
    CHECK_MESSAGE(h1.size() == 30000,
                  "size heap must be equal to the array size(30000), but no " +
                      std::to_string(h1.size()));
    CHECK_MESSAGE(h1.top() == min,
                  "min heap must be equal to the array min");
}

TEST_CASE("soft-heap: make (300000 elements)") {
    SoftHeap<int> h1;
    int arr[300000];
    int min = 300000;
    for (int i = 0; i < 300000; ++i) {
        int x = rand() % 300000;
        arr[i] = x;
        if (min > x)
            min = x;
    }
    h1.make(arr, 300000);
    CHECK_MESSAGE(h1.size() == 300000,
                  "size heap must be equal to the array size(300000), but no " +
                      std::to_string(h1.size()));
    CHECK_MESSAGE(h1.top() == min,
                  "min heap must be equal to the array min");
}
*/


TEST_CASE("soft-heap: random test#1") {
    SoftHeap<int> h1;
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    h1.add(1);
    CHECK(h1.size() == 1);
    CHECK(!h1.empty());
    h1.extract();
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    for (int i = 0; i < 5000; ++i) {
        h1.add(i);
    }
    CHECK(h1.top() == 0);
    h1.extract();
    CHECK(h1.size() == 4999);
    CHECK(h1.top() == 1);
}

TEST_CASE("soft-heap: random test#2") {
    SoftHeap<double> h1;
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    h1.add(1.0);
    CHECK(h1.size() == 1);
    CHECK(!h1.empty());
    h1.extract();
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    for (double i = 0; i < 5000; ++i) {
        h1.add(i);
    }
    CHECK(h1.top() == 0);
    h1.extract();
    CHECK(h1.size() == 4999);
    CHECK(h1.top() == 1.0);
}

TEST_CASE("soft-heap: check const, reference methods") {
    SoftHeap<int> h1;
    h1.add(6);
    h1.add(1);
    CHECK_MESSAGE(std::is_reference<decltype(h1.top())>::value,
                  "method top() must be &top()");
    CHECK_MESSAGE(
        std::is_const_v<std::remove_reference_t<decltype(h1.top())>>,
        "method top() must be const");
}

TEST_CASE("soft-heap: voids must be voids...") {
    SoftHeap<int> h1;
    h1.add(6);
    h1.add(1);
    int p[10]{};
    std::string str1(typeid(h1.extract()).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.extract())>,
                  "method extract() must be void! not " + str1 + " !");

    std::string str3(typeid(h1.extract()).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.add(10))>,
                  "method add() must be void! not " + str3 + " !");
}

TEST_CASE("soft-heap: SoftHeap(other &&)") {
    SoftHeap<int> h2;
    h2.add(1);
    h2.add(2);
    SoftHeap<int> h1(std::move(h2));
    CHECK(h1.size() == 2);
    CHECK(h1.top() == 1);
    h1.extract();
    CHECK(h1.size() == 1);
    CHECK(h1.top() == 2);
}

TEST_CASE("soft-heap: operator=(other &&)") {
    SoftHeap<int> h2;
    h2.add(1);
    h2.add(2);
    SoftHeap<int> h1 = std::move(h2);
    CHECK(h1.size() == 2);
    CHECK(h1.top() == 1);
    h1.extract();
    CHECK(h1.size() == 1);
    CHECK(h1.top() == 2);
}

TEST_CASE("soft-heap: default contructor must be implicit") {
    [[maybe_unused]] SoftHeap<int> h1 = {};
    // only compile
}




TEST_CASE("soft-heap: constructors with Compare") {
    SoftHeap<int, CloserTo> heap(CloserTo(10));
    // only compile
}



TEST_CASE("soft-heap-with-compare: 30000 add") {
    SoftHeap<int, CloserTo> h1(CloserTo(10));
    for (int i = 0; i < 30000; ++i) {
        h1.add(rand());
    }
    CHECK(h1.size() == 30000);
}

TEST_CASE("soft-heap-with-compare: 300000 add") {
    SoftHeap<int, CloserTo> h1(CloserTo(10));
    for (int i = 0; i < 300000; ++i) {
        h1.add(rand());
    }
    CHECK(h1.size() == 300000);
}



TEST_CASE("soft-heap-with-compare: 3000 add and 3000 extract_min") {
    SoftHeap<int, CloserTo> h1(CloserTo(10));
    std::vector<int> res;
    for (int i = 0; i < 3000; ++i) {
        int x = rand() % 3000;
        h1.add(x);
        res.push_back(std::abs(x - 10));
    }
    std::sort(res.begin(), res.end());
    CHECK_TIME(
        "After 3000 add random  and sort random x, then start checking "
        "extract_min");
    for (int i = 0; i < 3000; ++i) {
        CHECK(std::abs(h1.top() - 10) == res[i]);
        h1.extract();
    }
}

TEST_CASE("soft-heap-with-compare: 10000 add and 10000 extract_min") {
    SoftHeap<int, CloserTo> h1(CloserTo(10));
    std::vector<int> res;
    for (int i = 0; i < 10000; ++i) {
        int x = rand() % 10000;
        h1.add(x);
        res.push_back(std::abs(x - 10));
    }
    std::sort(res.begin(), res.end());
    CHECK_TIME(
        "After 10000 add random  and sort random x, then start checking "
        "extract_min");
    for (int i = 0; i < 10000; ++i) {
        CHECK_MESSAGE(std::abs(h1.top() - 10) == res[i], "#"+std::to_string(i)+": "+std::to_string(std::abs(h1.top() - 10))+" != "+std::to_string(res[i]));
        h1.extract();
    }
}



TEST_CASE("soft-heap-with-compare: 3000 add and 3000 extract_min") {
    SoftHeap<int, CloserTo> h1(CloserTo(10));
    std::vector<int> res;
    for (int i = 0; i < 3000; ++i) {
        int x = rand() % 3000;
        h1.add(x);
        res.push_back(std::abs(x - 10));
    }
    std::sort(res.begin(), res.end());
    CHECK_TIME(
        "After 3000 add random  and sort random x, then start checking "
        "extract_min");
    for (int i = 0; i < 3000; ++i) {
        CHECK(std::abs(h1.top() - 10) == res[i]);
        h1.extract();
    }
}

TEST_CASE("soft-heap-with-compare: 10000 add and 10000 extract_min") {
    SoftHeap<int, CloserTo> h1(CloserTo(10));
    std::vector<int> res;
    for (int i = 0; i < 10000; ++i) {
        int x = rand() % 10000;
        h1.add(x);
        res.push_back(std::abs(x - 10));
    }
    std::sort(res.begin(), res.end());
    CHECK_TIME(
        "After 10000 add random  and sort random x, then start checking "
        "extract_min");
    for (int i = 0; i < 10000; ++i) {
        CHECK_MESSAGE(std::abs(h1.top() - 10) == res[i], std::to_string(std::abs(h1.top() - 10))+" != "+std::to_string(res[i]));
        h1.extract();
    }
}


/*
TEST_CASE("soft-heap: make (30000 elements)") {
    SoftHeap<int, CloserTo> h1(CloserTo(10));
    int arr[30000];
    int min = 30000;
    for (int i = 0; i < 30000; ++i) {
        int x = rand() % 30000;
        arr[i] = x;
        if (min > std::abs(x - 10))
            min = std::abs(x - 10);
    }
    h1.make(arr, 30000);
    CHECK_MESSAGE(h1.size() == 30000,
                  "size heap must be equal to the array size(30000), but no " +
                      std::to_string(h1.size()));
    CHECK_MESSAGE(std::abs(h1.top() - 10) == min,
                  "min heap must be equal to the array min");
}

TEST_CASE("soft-heap: make (300000 elements)") {
    SoftHeap<int, CloserTo> h1(CloserTo(10));
    int arr[300000];
    int min = 300000;
    for (int i = 0; i < 300000; ++i) {
        int x = rand() % 300000;
        arr[i] = x;
        if (min > std::abs(x - 10))
            min = std::abs(x - 10);
    }
    h1.make(arr, 300000);
    CHECK_MESSAGE(h1.size() == 300000,
                  "size heap must be equal to the array size(300000), but no " +
                      std::to_string(h1.size()));
    CHECK_MESSAGE(std::abs(h1.top() - 10) == min,
                  "min heap must be equal to the array min");
}
*/



TEST_CASE("soft-heap-with-compare: random test") {
    SoftHeap<int, CloserTo> h1(CloserTo(10));
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    h1.add(1);
    CHECK(h1.size() == 1);
    CHECK(!h1.empty());
    h1.extract();
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    for (int i = 0; i < 5000; ++i) {
        h1.add(i);
    }
    CHECK(h1.top() == 10);
    h1.extract();
    CHECK(h1.size() == 4999);
    CHECK(h1.top() == 9 or h1.top() == 11);
}

TEST_CASE("soft-heap-with-compare: check const, reference methods") {
    SoftHeap<int, CloserTo> h1(CloserTo(10));
    h1.add(7);
    h1.add(1);
    CHECK_MESSAGE(std::is_reference<decltype(h1.top())>::value,
                  "method top() must be &top()");
    CHECK_MESSAGE(
        std::is_const_v<std::remove_reference_t<decltype(h1.top())>>,
        "method top() must be const");
}

TEST_CASE("soft-heap-with-compare: voids must be voids...") {
    SoftHeap<int, CloserTo> h1(CloserTo(10));
    h1.add(7);
    h1.add(1);
    int p[10]{};
    std::string str1(typeid(h1.extract()).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.extract())>,
                  "method extract() must be void! not " + str1 + " !");

    std::string str3(typeid(h1.extract()).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.add(10))>,
                  "method add() must be void! not " + str3 + " !");
}

TEST_CASE("soft-heap-with-compare: SoftHeap(other &&)") {
    SoftHeap<int, CloserTo> h2(CloserTo(10));
    h2.add(1);
    h2.add(2);
    SoftHeap<int, CloserTo> h1(std::move(h2));
    CHECK(h1.size() == 2);
    CHECK(h1.top() == 2);
    h1.extract();
    CHECK(h1.size() == 1);
    CHECK(h1.top() == 1);
}

TEST_CASE("soft-heap-with-compare: operator=(other &&)") {
    SoftHeap<int, CloserTo> h2(CloserTo(10));
    h2.add(1);
    h2.add(2);
    SoftHeap<int, CloserTo> h1 = std::move(h2);
    CHECK(h1.size() == 2);
    CHECK(h1.top() == 2);
    h1.extract();
    CHECK(h1.size() == 1);
    CHECK(h1.top() == 1);
}

TEST_CASE(
    "soft-heap-with-compare: constructor from comparator is explicit") {
    CHECK((std::is_constructible_v<SoftHeap<int>, std::less<int>>));
    CHECK((!std::is_convertible_v<std::less<int>, SoftHeap<int>>));
}

TEST_CASE("soft-heap-with-compare: with std::greater(on max)") {
    SoftHeap<int, std::greater<>> heap;
    auto &ch = heap;

    heap.add(5);
    CHECK(!ch.empty());
    CHECK(ch.top() == 5);

    heap.add(15);
    CHECK(!ch.empty());
    CHECK(ch.top() == 15);

    heap.add(10);
    CHECK(!ch.empty());
    CHECK(ch.top() == 15);

    heap.extract();
    CHECK(!ch.empty());
    CHECK(ch.top() == 10);

    heap.extract();
    CHECK(!ch.empty());
    CHECK(ch.top() == 5);

    heap.extract();
    CHECK(ch.empty());
}

TEST_CASE("soft-heap: delete"){
	SoftHeap<int> h;
	for(int i=0; i<10; ++i){
		h.add(i);
	}
	h.del(0);
	CHECK(h.top()==1);
	h.extract();
	CHECK(h.top()==2);
	h.del(5);
	CHECK(h.size()==7);
	h.del(3);
	h.extract();
	h.extract();
	CHECK(h.top()==6);
}

TEST_CASE("soft-heap: delete"){
	SoftHeap<int, CloserTo> h(CloserTo(10));
	for(int i=0; i<10; ++i){
		h.add(i);
	}
	h.del(9);
	CHECK(h.top()==8);
	h.extract();
	CHECK(h.top()==7);
	h.del(5);
	CHECK(h.size()==7);
	h.del(6);
	h.extract();
	CHECK(h.top()==4);
}

TEST_CASE("soft-heap: decreaseKey"){
	SoftHeap<int> h;
	for(int i=0; i<10; ++i){
		h.add(i);
	}
	h.decreaseKey(5,11);
	CHECK(h.size()==10);
	h.extract();
	h.extract();
	h.extract();
	h.extract();
	h.extract();
	CHECK(h.top()==6);
}
void add(std::multiset<int> &pq, SoftHeap<int> &ph){
	int x = rand();
	pq.insert(x);
	ph.add(x);
}

void extract(std::multiset<int> &pq, SoftHeap<int> &ph, int k){
	int x = *(pq.begin());
	int y = ph.top();
	pq.erase(pq.begin());
	ph.extract();
	REQUIRE_MESSAGE(x==y, "#"+std::to_string(k)+": minimums are not equal: "+std::to_string(x)+" != "+std::to_string(y));
}

void delet(std::multiset<int> &pq, SoftHeap<int> &ph, int k){
	std::size_t sz = pq.size();
	std::size_t sz_2 = rand() % sz;
	std::multiset<int>::iterator it = pq.begin();
	advance(it, sz_2);
	ph.del(*it);
	pq.erase(it);
	int x = *(pq.begin());
	int y = ph.top();
	REQUIRE_MESSAGE(x==y, "#"+std::to_string(k)+": minimums are not equal: "+std::to_string(x)+" != "+std::to_string(y));
}

TEST_CASE("stress test with multiset: 10^5 operations"){
	std::multiset<int> pq;
	SoftHeap<int> ph;
	for(int i = 0; i<100'000; ++i){
		int r = rand() % 3;
		if(r<2 or pq.empty()){ //с большей вероятностью add
			add(pq, ph);
			REQUIRE(pq.size()==ph.size());
		} else{
			extract(pq, ph, i);
			REQUIRE(pq.size()==ph.size());
		}
	}
}

TEST_CASE("stress test with multiset: 10^5 operations"){
	std::multiset<int> pq;
	SoftHeap<int> ph;
	for(int i = 0; i<100'000; ++i){
		int r = rand() % 7;
		if(r<7 or pq.empty()){ //с большей вероятностью add
			add(pq, ph);
			REQUIRE(pq.size()==ph.size());
		} else{
			extract(pq, ph, i);
			REQUIRE(pq.size()==ph.size());
		}
	}
}

TEST_CASE("stress test with multiset: 10^5 operations"){
	std::multiset<int> pq;
	SoftHeap<int> ph;
	for(int i = 0; i<100'000; ++i){
		int r = rand() % 7;
		if(r<6 or pq.empty()){ //с большей вероятностью add
			add(pq, ph);
			REQUIRE(pq.size()==ph.size());
		} else if(r==6){
			delet(pq, ph, i);
			REQUIRE(pq.size()==ph.size());
		} else{
			extract(pq, ph, i);
			REQUIRE(pq.size()==ph.size());
		}
	}
}

}  // namespace
