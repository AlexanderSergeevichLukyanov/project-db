#include <algorithm>
#include <type_traits>
#include <vector>
#include "funnel_heap.h"
#include "mytest.h"
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

#ifdef funnel_wrong_test
TEST_CASE("funnel-heap: demo-test") {
    CHECK(2 * 2 == 5);
}
#endif

#ifdef funnel_construct
TEST_CASE("funnel-heap: constructors without Compare") {
    funnel_heap<10000> h1;
//    funnel_heap<10000> h2;
//   funnel_heap<std::string> h3;
    // only compile
}
#endif

#ifdef funnel_insert
TEST_CASE("funnel-heap: 30000 insert") {
    funnel_heap<10000> h1;
    for (int i = 0; i < 30000; ++i) {
        h1.insert(rand());
    }
   // CHECK(h1.size() == 30000);
}

TEST_CASE("funnel-heap: 300000 insert") {
    funnel_heap<10000> h1;
    for (int i = 0; i < 300000; ++i) {
        h1.insert(rand());
    }
  //  CHECK(h1.size() == 300000);
}
#endif

#ifdef funnel_get_min
TEST_CASE("funnel-heap: GetMin()") {
    funnel_heap<10000> h1;
    int min = 300000;
    for (int i = 0; i < 300000; ++i) {
        int x = rand() % 300000;
        if (x < min)
            min = x;
        h1.insert(x);
    }
    CHECK_MESSAGE(h1.getMin() == min, "generate min is other, "+std::to_string(h1.getMin())+"!="+std::to_string(min));
}

TEST_CASE("funnel-heap: 300000 GetMin()") {
    funnel_heap<10000> h1;
    int min = 300000;
    for (int i = 0; i < 300000; ++i) {
        int x = rand() % 300000;
        if (x < min)
            min = x;
        h1.insert(x);
        CHECK_MESSAGE(h1.getMin() == min, "generate min is other");
        if (h1.getMin() != min)
            break;
    }
}
#endif

#ifdef funnel_extract_min
TEST_CASE("funnel-heap: 3000 insert and 3000 extract_min") {
    funnel_heap<10000> h1;
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
		if(h1.getMin() != res[i]) break;
        h1.extractMin();
    }
}

TEST_CASE("funnel-heap: 10000 insert and 10000 extract_min") {
    funnel_heap<10000> h1;
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
		if(h1.getMin() != res[i]) break;
        h1.extractMin();
    }
}
#endif

#ifdef funnel_make
TEST_CASE("funnel-heap: make (30000 elements)") {
    funnel_heap<10000> h1;
    int arr[30000];
    int min = 30000;
    for (int i = 0; i < 30000; ++i) {
        int x = rand() % 30000;
        arr[i] = x;
        if (min > x)
            min = x;
    }
    h1.make(arr, 30000);
 /*   CHECK_MESSAGE(h1.size() == 30000,
                  "size heap must be equal to the array size(30000), but no " +
                      std::to_string(h1.size()));*/
    CHECK_MESSAGE(h1.getMin() == min,
                  "min heap must be equal to the array min");
}

TEST_CASE("funnel-heap: make (300000 elements)") {
    funnel_heap<10000> h1;
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
#endif

#ifdef funnel_solyanka
TEST_CASE("funnel-heap: random test#1") {
    funnel_heap<10000> h1;
   // CHECK(h1.empty());
  //  CHECK(h1.size() == 0);
    h1.insert(1);
   // CHECK(h1.size() == 1);
   // CHECK(!h1.empty());
    h1.extractMin();
   // CHECK(h1.empty());
   // CHECK(h1.size() == 0);
    for (int i = 0; i < 5000; ++i) {
        h1.insert(i);
    }
    CHECK(h1.getMin() == 0);
    h1.extractMin();
    //CHECK(h1.size() == 4999);
    CHECK(h1.getMin() == 1);
}

TEST_CASE("funnel-heap: random test#2") {
    funnel_heap<10000> h1;
//    CHECK(h1.empty());
//    CHECK(h1.size() == 0);
    h1.insert(1.0);
//    CHECK(h1.size() == 1);
//    CHECK(!h1.empty());
    h1.extractMin();
//    CHECK(h1.empty());
//    CHECK(h1.size() == 0);
    for (int i = 0; i < 50000; ++i) {
        h1.insert(i);
    }
    CHECK(h1.getMin() == 0);
    h1.extractMin();
//    CHECK(h1.size() == 4999);
    CHECK(h1.getMin() == 1);
}

TEST_CASE("funnel-heap: check const, reference methods") {
    funnel_heap<10000> h1;
    h1.insert(6);
    h1.insert(1);
    CHECK_MESSAGE(std::is_reference<decltype(h1.getMin())>::value,
                  "method getMin() must be &getMin()");
    CHECK_MESSAGE(
        std::is_const_v<std::remove_reference_t<decltype(h1.getMin())>>,
        "method getMin() must be const");
}

TEST_CASE("funnel-heap: voids must be voids...") {
    funnel_heap<10000> h1;
    h1.insert(6);
    h1.insert(1);
    int p[10]{};
    std::string str1(typeid(h1.extractMin()).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.extractMin())>,
                  "method extractMin() must be void! not " + str1 + " !");
#ifdef funnel_make
    std::string str2(typeid(h1.make(p, 10)).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.make(p, 10))>,
                  "method make( must be void! not " + str2 + "!");
#endif
    std::string str3(typeid(h1.extractMin()).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.insert(10))>,
                  "method insert() must be void! not " + str3 + " !");
}
/*
TEST_CASE("funnel-heap: funnel_heap(other &&)") {
    funnel_heap<10000> h2;
    h2.insert(1);
    h2.insert(2);
    funnel_heap<10000> h1(std::move(h2));
 //   CHECK(h1.size() == 2);
    // CHECK(h2.size() == 0);
    CHECK(h1.getMin() == 1);
    h1.extractMin();
    CHECK(h1.size() == 1);
    // CHECK(h2.size() == 0);
    // CHECK(h2.empty());
    CHECK(h1.getMin() == 2);
}

TEST_CASE("funnel-heap: operator=(other &&)") {
    funnel_heap<10000> h2;
    h2.insert(1);
    h2.insert(2);
    funnel_heap<10000> h1 = std::move(h2);
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
TEST_CASE("funnel-heap: default contructor must be implicit") {
    [[maybe_unused]] funnel_heap<10000> h1 = {};
    // only compile
}

#endif

#ifdef funnel_construct_comp
TEST_CASE("funnel-heap: constructors with Compare") {
    funnel_heap<10000, CloserTo> heap(CloserTo(10));
    // only compile
}
#endif

#ifdef funnel_insert_comp
TEST_CASE("funnel-heap-with-compare: 30000 insert") {
    funnel_heap<10000, CloserTo> h1(CloserTo(10));
    for (int i = 0; i < 30000; ++i) {
        h1.insert(rand());
    }
  //  CHECK(h1.size() == 30000);
}

TEST_CASE("funnel-heap-with-compare: 300000 insert") {
    funnel_heap<10000, CloserTo> h1(CloserTo(10));
    for (int i = 0; i < 300000; ++i) {
        h1.insert(rand());
    }
   //CHECK(h1.size() == 300000);
}
#endif

#ifdef funnel_extract_min_comp
TEST_CASE("funnel-heap-with-compare: 3000 insert and 3000 extract_min") {
    funnel_heap<10000, CloserTo> h1(CloserTo(10));
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
		int s= h1.getMin() - 10;
        CHECK(std::abs(s) == res[i]);
		if(std::abs(s) != res[i]) break;
        h1.extractMin();
    }
}

TEST_CASE("funnel-heap-with-compare: 10000 insert and 10000 extract_min") {
    funnel_heap<10000, CloserTo> h1(CloserTo(10));
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
		int s = h1.getMin() - 10;
        CHECK(std::abs(s) == res[i]);
		if(std::abs(s) != res[i]) break;
        h1.extractMin();
    }
}
#endif

#ifdef funnel_extract_min_comp
TEST_CASE("funnel-heap-with-compare: 3000 insert and 3000 extract_min") {
    funnel_heap<10000, CloserTo> h1(CloserTo(10));
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
		int s= h1.getMin() - 10;
        CHECK(std::abs(s) == res[i]);
        h1.extractMin();
    }
}

TEST_CASE("funnel-heap-with-compare: 10000 insert and 10000 extract_min") {
    funnel_heap<10000, CloserTo> h1(CloserTo(10));
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
		int s = h1.getMin() - 10;
        CHECK(std::abs(s) == res[i]);
        h1.extractMin();
    }
}
#endif

#ifdef funnel_make_comp
TEST_CASE("funnel-heap: make (30000 elements)") {
    funnel_heap<10000, CloserTo> h1(CloserTo(10));
    int arr[30000];
    int min = 30000;
    for (int i = 0; i < 30000; ++i) {
        int x = rand() % 30000;
        arr[i] = x;
        if (min > std::abs(x - 10))
            min = std::abs(x - 10);
    }
    h1.make(arr, 30000);
  /*  CHECK_MESSAGE(h1.size() == 30000,
                  "size heap must be equal to the array size(30000), but no " +
                      std::to_string(h1.size()));*/
    CHECK_MESSAGE(std::abs(h1.getMin() - 10) == min,
                  "min heap must be equal to the array min");
}

TEST_CASE("funnel-heap: make (300000 elements)") {
    funnel_heap<10000, CloserTo> h1(CloserTo(10));
    int arr[300000];
    int min = 300000;
    for (int i = 0; i < 300000; ++i) {
        int x = rand() % 300000;
        arr[i] = x;
        if (min > std::abs(x - 10))
            min = std::abs(x - 10);
    }
    h1.make(arr, 300000);
/*    CHECK_MESSAGE(h1.size() == 300000,
                  "size heap must be equal to the array size(300000), but no " +
                      std::to_string(h1.size()));*/
    CHECK_MESSAGE(std::abs(h1.getMin() - 10) == min,
                  "min heap must be equal to the array min");
}
#endif

#ifdef funnel_solyanka_comp

TEST_CASE("funnel-heap-with-compare: random test") {
    funnel_heap<10000, CloserTo> h1(CloserTo(10));
//    CHECK(h1.empty());
//    CHECK(h1.size() == 0);
    h1.insert(1);
//    CHECK(h1.size() == 1);
//    CHECK(!h1.empty());
    h1.extractMin();
//    CHECK(h1.empty());
//    CHECK(h1.size() == 0);
    for (int i = 0; i < 5000; ++i) {
        h1.insert(i);
    }
    CHECK(h1.getMin() == 10);
    h1.extractMin();
//    CHECK(h1.size() == 4999);
    CHECK(h1.getMin() == 9 or h1.getMin() == 11);
}

TEST_CASE("funnel-heap-with-compare: check const, reference methods") {
    funnel_heap<10000, CloserTo> h1(CloserTo(10));
    h1.insert(7);
    h1.insert(1);
    CHECK_MESSAGE(std::is_reference<decltype(h1.getMin())>::value,
                  "method getMin() must be &getMin()");
    CHECK_MESSAGE(
        std::is_const_v<std::remove_reference_t<decltype(h1.getMin())>>,
        "method getMin() must be const");
}

TEST_CASE("funnel-heap-with-compare: voids must be voids...") {
    funnel_heap<10000, CloserTo> h1(CloserTo(10));
    h1.insert(7);
    h1.insert(1);
    int p[10]{};
    std::string str1(typeid(h1.extractMin()).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.extractMin())>,
                  "method extractMin() must be void! not " + str1 + " !");
#ifdef funnel_make_comp
    std::string str2(typeid(h1.make(p, 10)).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.make(p, 10))>,
                  "method make( must be void! not " + str2 + "!");
#endif
    std::string str3(typeid(h1.extractMin()).name());
    CHECK_MESSAGE(std::is_void_v<decltype(h1.insert(10))>,
                  "method insert() must be void! not " + str3 + " !");
}
/*
TEST_CASE("funnel-heap-with-compare: funnel_heap(other &&)") {
    funnel_heap<10000, CloserTo> h2(CloserTo(10));
    h2.insert(1);
    h2.insert(2);
    funnel_heap<10000, CloserTo> h1(std::move(h2));
    CHECK(h1.size() == 2);
    CHECK(h2.size() == 0);
    CHECK(h1.getMin() == 2);
    h1.extractMin();
    CHECK(h1.size() == 1);
    CHECK(h2.size() == 0);
    CHECK(h2.empty());
    CHECK(h1.getMin() == 1);
}

TEST_CASE("funnel-heap-with-compare: operator=(other &&)") {
    funnel_heap<10000, CloserTo> h2(CloserTo(10));
    h2.insert(1);
    h2.insert(2);
    funnel_heap<10000, CloserTo> h1 = std::move(h2);
    CHECK(h1.size() == 2);
    CHECK(h2.size() == 0);
    CHECK(h1.getMin() == 2);
    h1.extractMin();
    CHECK(h1.size() == 1);
    CHECK(h2.size() == 0);
    CHECK(h2.empty());
    CHECK(h1.getMin() == 1);
}
*/
TEST_CASE("funnel-heap-with-compare: constructor from comparator is explicit") {
    CHECK((std::is_constructible_v<funnel_heap<10000>, std::less<int>>));
    CHECK((!std::is_convertible_v<std::less<int>, funnel_heap<10000>>));
}

TEST_CASE("funnel-heap-with-compare: with std::greater(on max)") {
    funnel_heap<10000, std::greater<>> heap;
    const auto &ch = heap;

    heap.insert(5);
//    CHECK(!ch.empty());
    CHECK(ch.getMin() == 5);

    heap.insert(15);
//    CHECK(!ch.empty());
    CHECK(ch.getMin() == 15);

    heap.insert(10);
//    CHECK(!ch.empty());
    CHECK(ch.getMin() == 15);

    heap.extractMin();
//    CHECK(!ch.empty());
    CHECK(ch.getMin() == 10);

    heap.extractMin();
//    CHECK(!ch.empty());
    CHECK(ch.getMin() == 5);

    heap.extractMin();
//    CHECK(ch.empty());
}

#endif
}  // namespace
