#include <algorithm>
#include <type_traits>
#include <vector>
#include <set>
#include "mytest.h"
#include "EMHS.h"

using namespace EMHS;


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


TEST_CASE("pairing-heap: constructors without Compare") {
    pairing_heap_with_buffer<int> h1;
    pairing_heap_with_buffer<double> h2;
    //pairing_heap_with_buffer<std::string> h3;
    // only compile
}



TEST_CASE("pairing-heap: 30000 insert") {
    pairing_heap_with_buffer<int> h1;
    for (int i = 0; i < 30000; ++i) {
        h1.insert(rand());
    }
    CHECK(h1.size() == 30000);
}

TEST_CASE("pairing-heap: 300000 insert") {
    pairing_heap_with_buffer<int> h1;
    for (int i = 0; i < 300000; ++i) {
        h1.insert(rand());
    }
    CHECK(h1.size() == 300000);
}



TEST_CASE("pairing-heap: GetMin()") {
    pairing_heap_with_buffer<int> h1;
    int min = 300000;
    for (int i = 0; i < 300000; ++i) {
        int x = rand() % 300000;
        if (x < min)
            min = x;
        h1.insert(x);
    }
    CHECK_MESSAGE(h1.getMin() == min, "generate min is other");
}

TEST_CASE("pairing-heap: 300000 GetMin()") {
    pairing_heap_with_buffer<int> h1;
    int min = 300000;
    for (int i = 0; i < 300000; ++i) {
        int x = rand() % 300000;
        if (x < min)
            min = x;
        h1.insert(x);
        CHECK_MESSAGE(h1.getMin() == min, "#"+std::to_string(i)+": generate min is other: "+std::to_string(h1.getMin())+" != "+std::to_string(min));
        if (h1.getMin() != min)
            break;
    }
}

TEST_CASE("pairing-heap: 3000 insert and 3000 extract_min") {
    pairing_heap_with_buffer<int> h1;
    std::vector<int> res;
    for (int i = 0; i < 3000; ++i) {
        int x = rand() % 3000;
        h1.insert(x);
        res.push_back(x);
    }
    std::sort(res.begin(), res.end());
    CHECK_TIME(
        "After 3000 insert random  and sort random x, then start checking "
        "extract_min");
    for (int i = 0; i < 3000; ++i) {
        CHECK(h1.getMin() == res[i]);
        h1.extractMin();
    }
}

TEST_CASE("pairing-heap: 10000 insert and 10000 extract_min") {
    pairing_heap_with_buffer<int> h1;
    std::vector<int> res;
    for (int i = 0; i < 10000; ++i) {
        int x = rand() % 10000;
        h1.insert(x);
        res.push_back(x);
    }
    std::sort(res.begin(), res.end());
    CHECK_TIME(
        "After 10000 insert random  and sort random x, then start checking "
        "extract_min");
    for (int i = 0; i < 10000; ++i) {
        CHECK(h1.getMin() == res[i]);
        h1.extractMin();
    }
}


/*
TEST_CASE("pairing-heap: make (30000 elements)") {
    pairing_heap_with_buffer<int, 7000> h1;
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
    CHECK_MESSAGE(h1.getMin() == min,
                  "min heap must be equal to the array min");
}

TEST_CASE("pairing-heap: make (300000 elements)") {
    pairing_heap_with_buffer<int, 7000> h1;
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
    CHECK_MESSAGE(h1.getMin() == min,
                  "min heap must be equal to the array min");
}
*/


TEST_CASE("pairing-heap: random test#1") {
    pairing_heap_with_buffer<int> h1;
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    h1.insert(1);
    CHECK(h1.size() == 1);
    CHECK(!h1.empty());
    h1.extractMin();
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    for (int i = 0; i < 5000; ++i) {
        h1.insert(i);
    }
    CHECK(h1.getMin() == 0);
    h1.extractMin();
    CHECK(h1.size() == 4999);
    CHECK(h1.getMin() == 1);
}

TEST_CASE("pairing-heap: random test#2") {
    pairing_heap_with_buffer<double> h1;
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    h1.insert(1.0);
    CHECK(h1.size() == 1);
    CHECK(!h1.empty());
    h1.extractMin();
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    for (double i = 0; i < 5000; ++i) {
        h1.insert(i);
    }
    CHECK(h1.getMin() == 0);
    h1.extractMin();
    CHECK(h1.size() == 4999);
    CHECK(h1.getMin() == 1.0);
}

TEST_CASE("pairing-heap: check const, reference methods") {
    pairing_heap_with_buffer<int> h1;
    h1.insert(6);
    h1.insert(1);
    CHECK_MESSAGE(std::is_reference<decltype(h1.getMin())>::value,
                  "method getMin() must be &getMin()");
    CHECK_MESSAGE(
        std::is_const_v<std::remove_reference_t<decltype(h1.getMin())>>,
        "method getMin() must be const");
}
/*
TEST_CASE("pairing-heap: voids must be voids...") {
    pairing_heap_with_buffer<int, 7000> h1;
    h1.insert(6);
    h1.insert(1);
    int p[10]{};
    std::string str1(typeid(h1.extractMin()).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.extractMin())>,
                  "method extractMin() must be void! not " + str1 + " !");

    std::string str2(typeid(h1.make(p, 10)).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.make(p, 10))>,
                  "method make( must be void! not " + str2 + "!");

    std::string str3(typeid(h1.extractMin()).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.insert(10))>,
                  "method insert() must be void! not " + str3 + " !");
}
*/
/*
TEST_CASE("pairing-heap: pairing_heap_with_buffer(other &&)") {
    pairing_heap_with_buffer<int> h2;
    h2.insert(1);
    h2.insert(2);
    pairing_heap_with_buffer<int> h1(std::move(h2));
    CHECK(h1.size() == 2);
    // CHECK(h2.size() == 0);
    CHECK(h1.getMin() == 1);
    h1.extractMin();
    CHECK(h1.size() == 1);
    // CHECK(h2.size() == 0);
    // CHECK(h2.empty());
    CHECK(h1.getMin() == 2);
}

TEST_CASE("pairing-heap: operator=(other &&)") {
    pairing_heap_with_buffer<int> h2;
    h2.insert(1);
    h2.insert(2);
    pairing_heap_with_buffer<int> h1 = std::move(h2);
    CHECK(h1.size() == 2);
    // CHECK(h2.size() == 0);
    CHECK(h1.getMin() == 1);
    h1.extractMin();
    CHECK(h1.size() == 1);
    // CHECK(h2.size() == 0);
    // CHECK(h2.empty());
    CHECK(h1.getMin() == 2);
}
*/
TEST_CASE("pairing-heap: default contructor must be implicit") {
    [[maybe_unused]] pairing_heap_with_buffer<int> h1 = {};
    // only compile
}




TEST_CASE("pairing-heap: constructors with Compare") {
    pairing_heap_with_buffer<int, CloserTo> heap(CloserTo(10));
    // only compile
}



TEST_CASE("pairing-heap-with-compare: 30000 insert") {
    pairing_heap_with_buffer<int, CloserTo> h1(CloserTo(10));
    for (int i = 0; i < 30000; ++i) {
        h1.insert(rand());
    }
    CHECK(h1.size() == 30000);
}

TEST_CASE("pairing-heap-with-compare: 300000 insert") {
    pairing_heap_with_buffer<int, CloserTo> h1(CloserTo(10));
    for (int i = 0; i < 300000; ++i) {
        h1.insert(rand());
    }
    CHECK(h1.size() == 300000);
}



TEST_CASE("pairing-heap-with-compare: 3000 insert and 3000 extract_min") {
    pairing_heap_with_buffer<int, CloserTo> h1(CloserTo(10));
    std::vector<int> res;
    for (int i = 0; i < 3000; ++i) {
        int x = rand() % 3000;
        h1.insert(x);
        res.push_back(std::abs(x - 10));
    }
    std::sort(res.begin(), res.end());
    CHECK_TIME(
        "After 3000 insert random  and sort random x, then start checking "
        "extract_min");
    for (int i = 0; i < 3000; ++i) {
        CHECK(std::abs(h1.getMin() - 10) == res[i]);
        h1.extractMin();
    }
}

TEST_CASE("pairing-heap-with-compare: 10000 insert and 10000 extract_min") {
    pairing_heap_with_buffer<int, CloserTo> h1(CloserTo(10));
    std::vector<int> res;
    for (int i = 0; i < 10000; ++i) {
        int x = rand() % 10000;
        h1.insert(x);
        res.push_back(std::abs(x - 10));
    }
    std::sort(res.begin(), res.end());
    CHECK_TIME(
        "After 10000 insert random  and sort random x, then start checking "
        "extract_min");
    for (int i = 0; i < 10000; ++i) {
        CHECK_MESSAGE(std::abs(h1.getMin() - 10) == res[i], "#"+std::to_string(i)+": "+std::to_string(std::abs(h1.getMin() - 10))+" != "+std::to_string(res[i]));
        h1.extractMin();
    }
}



TEST_CASE("pairing-heap-with-compare: 3000 insert and 3000 extract_min") {
    pairing_heap_with_buffer<int, CloserTo> h1(CloserTo(10));
    std::vector<int> res;
    for (int i = 0; i < 3000; ++i) {
        int x = rand() % 3000;
        h1.insert(x);
        res.push_back(std::abs(x - 10));
    }
    std::sort(res.begin(), res.end());
    CHECK_TIME(
        "After 3000 insert random  and sort random x, then start checking "
        "extract_min");
    for (int i = 0; i < 3000; ++i) {
        CHECK(std::abs(h1.getMin() - 10) == res[i]);
        h1.extractMin();
    }
}

TEST_CASE("pairing-heap-with-compare: 10000 insert and 10000 extract_min") {
    pairing_heap_with_buffer<int, CloserTo> h1(CloserTo(10));
    std::vector<int> res;
    for (int i = 0; i < 10000; ++i) {
        int x = rand() % 10000;
        h1.insert(x);
        res.push_back(std::abs(x - 10));
    }
    std::sort(res.begin(), res.end());
    CHECK_TIME(
        "After 10000 insert random  and sort random x, then start checking "
        "extract_min");
    for (int i = 0; i < 10000; ++i) {
        CHECK_MESSAGE(std::abs(h1.getMin() - 10) == res[i], std::to_string(std::abs(h1.getMin() - 10))+" != "+std::to_string(res[i]));
        h1.extractMin();
    }
}


/*
TEST_CASE("pairing-heap: make (30000 elements)") {
    pairing_heap_with_buffer<int, 7000, CloserTo> h1(CloserTo(10));
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
    CHECK_MESSAGE(std::abs(h1.getMin() - 10) == min,
                  "min heap must be equal to the array min");
}

TEST_CASE("pairing-heap: make (300000 elements)") {
    pairing_heap_with_buffer<int, 7000, CloserTo> h1(CloserTo(10));
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
    CHECK_MESSAGE(std::abs(h1.getMin() - 10) == min,
                  "min heap must be equal to the array min");
}
*/



TEST_CASE("pairing-heap-with-compare: random test") {
    pairing_heap_with_buffer<int, CloserTo> h1(CloserTo(10));
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    h1.insert(1);
    CHECK(h1.size() == 1);
    CHECK(!h1.empty());
    h1.extractMin();
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    for (int i = 0; i < 5000; ++i) {
        h1.insert(i);
    }
    CHECK(h1.getMin() == 10);
    h1.extractMin();
    CHECK(h1.size() == 4999);
    CHECK(h1.getMin() == 9 or h1.getMin() == 11);
}

TEST_CASE("pairing-heap-with-compare: check const, reference methods") {
    pairing_heap_with_buffer<int, CloserTo> h1(CloserTo(10));
    h1.insert(7);
    h1.insert(1);
    CHECK_MESSAGE(std::is_reference<decltype(h1.getMin())>::value,
                  "method getMin() must be &getMin()");
    CHECK_MESSAGE(
        std::is_const_v<std::remove_reference_t<decltype(h1.getMin())>>,
        "method getMin() must be const");
}
/*
TEST_CASE("pairing-heap-with-compare: voids must be voids...") {
    pairing_heap_with_buffer<int, 7000, CloserTo> h1(CloserTo(10));
    h1.insert(7);
    h1.insert(1);
    int p[10]{};
    std::string str1(typeid(h1.extractMin()).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.extractMin())>,
                  "method extractMin() must be void! not " + str1 + " !");

    std::string str2(typeid(h1.make(p, 10)).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.make(p, 10))>,
                  "method make( must be void! not " + str2 + "!");

    std::string str3(typeid(h1.extractMin()).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.insert(10))>,
                  "method insert() must be void! not " + str3 + " !");
}

TEST_CASE("pairing-heap-with-compare: pairing_heap_with_buffer(other &&)") {
    pairing_heap_with_buffer<int, CloserTo> h2(CloserTo(10));
    h2.insert(1);
    h2.insert(2);
    pairing_heap_with_buffer<int, CloserTo> h1(std::move(h2));
    CHECK(h1.size() == 2);
    //   CHECK(h2.size() == 0);
    CHECK(h1.getMin() == 2);
    h1.extractMin();
    CHECK(h1.size() == 1);
    // CHECK(h2.size() == 0);
    // CHECK(h2.empty());
    CHECK(h1.getMin() == 1);
}

TEST_CASE("pairing-heap-with-compare: operator=(other &&)") {
    pairing_heap_with_buffer<int, CloserTo> h2(CloserTo(10));
    h2.insert(1);
    h2.insert(2);
    pairing_heap_with_buffer<int, CloserTo> h1 = std::move(h2);
    CHECK(h1.size() == 2);
    // CHECK(h2.size() == 0);
    CHECK(h1.getMin() == 2);
    h1.extractMin();
    CHECK(h1.size() == 1);
    // CHECK(h2.size() == 0);
    // CHECK(h2.empty());
    CHECK(h1.getMin() == 1);
}

TEST_CASE(
    "pairing-heap-with-compare: constructor from comparator is explicit") {
    CHECK((std::is_constructible_v<pairing_heap_with_buffer<int>, std::less<int>>));
    CHECK((!std::is_convertible_v<std::less<int>, pairing_heap_with_buffer<int>>));
}
*/
TEST_CASE("pairing-heap-with-compare: with std::greater(on max)") {
    pairing_heap_with_buffer<int, std::greater<>> heap;
    const auto &ch = heap;

    heap.insert(5);
    CHECK(!ch.empty());
    CHECK(ch.getMin() == 5);

    heap.insert(15);
    CHECK(!ch.empty());
    CHECK(ch.getMin() == 15);

    heap.insert(10);
    CHECK(!ch.empty());
    CHECK(ch.getMin() == 15);

    heap.extractMin();
    CHECK(!ch.empty());
    CHECK(ch.getMin() == 10);

    heap.extractMin();
    CHECK(!ch.empty());
    CHECK(ch.getMin() == 5);

    heap.extractMin();
    CHECK(ch.empty());
}

void add(std::multiset<int> &pq, pairing_heap_with_buffer<int> &ph){
	int x = rand();
	pq.insert(x);
	ph.insert(x);
}

void extract(std::multiset<int> &pq, pairing_heap_with_buffer<int> &ph, int k){
	int x = *(pq.begin());
	int y = ph.getMin();
	pq.erase(pq.begin());
	ph.extractMin();
	REQUIRE_MESSAGE(x==y, "#"+std::to_string(k)+": minimums are not equal: "+std::to_string(x)+" != "+std::to_string(y));
}

TEST_CASE("stress test with priority_queue: 10^6 operations"){
	std::multiset<int> pq;
	pairing_heap_with_buffer<int> ph;
	for(int i = 0; i<100'000; ++i){
		int r = rand() % 3;
		if(r<2 or pq.empty()){ //с большей вероятностью push
			add(pq, ph);
			REQUIRE(pq.size()==ph.size());
		} else{
			extract(pq, ph, i);
			REQUIRE(pq.size()==ph.size());
		}
	}
}

TEST_CASE("stress test with priority_queue: 10^6 operations, with N"){
    std::multiset<int> pq;
    pairing_heap_with_buffer<int> ph(1'000'000);
    for(int i = 0; i<100'000; ++i){
        int r = rand() % 3;
        if(r<2 or pq.empty()){ //с большей вероятностью push
            add(pq, ph);
            REQUIRE(pq.size()==ph.size());
        } else{
            extract(pq, ph, i);
            REQUIRE(pq.size()==ph.size());
        }
    }
}
}  // namespace
