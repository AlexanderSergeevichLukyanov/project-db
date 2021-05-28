#include <algorithm>
#include <set>
#include <type_traits>
#include <vector>
#include "EMHS.h"
#include "mytest.h"

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

TEST_CASE("flower-heap: constructors without Compare") {
    FlowerHeap_t<int> h1(1, 10 * EMHS::B);
    // only compile
}

TEST_CASE("flower-heap: 30000 add") {
    FlowerHeap_t<uint64_t> h1(1, 10 * EMHS::B);
    for (int i = 0; i < 3000; ++i) {
        h1.add(rand());
    }
    CHECK(h1.size() == 3000);
}

TEST_CASE("flower-heap: 300000 add") {
    FlowerHeap_t<uint64_t> h1(1, 10 * EMHS::B);
    for (int i = 0; i < 3000; ++i) {
        h1.add(rand());
    }
    CHECK(h1.size() == 3000);
}

TEST_CASE("flower-heap: top()") {
    FlowerHeap_t<uint64_t> h1(1, 10 * EMHS::B);
    int min = 300000;
    for (int i = 0; i < 300000; ++i) {
        int x = rand() % 300000;
        if (x < min)
            min = x;
        h1.add(x);
    }
    CHECK_MESSAGE(h1.top() == min, "generate min is other");
}

TEST_CASE("flower-heap: 300000 top()") {
    FlowerHeap_t<uint64_t> h1(1, 10 * EMHS::B);
    int min = 300000;
    for (int i = 0; i < 300000; ++i) {
        int x = rand() % 300000;
        if (x < min)
            min = x;
        h1.add(x);
        CHECK_MESSAGE(h1.top() == min, "#" + std::to_string(i) +
                                           ": generate min is other: " +
                                           std::to_string(h1.top()) +
                                           " != " + std::to_string(min));
        if (h1.top() != min)
            break;
    }
}

TEST_CASE("flower-heap: 3000 add and 3000 extract_min") {
    FlowerHeap_t<uint64_t> h1(1, 10 * EMHS::B);
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
        if (h1.top() != res[i])
            break;
        h1.extract();
    }
}

TEST_CASE("flower-heap: 10000 add and 10000 extract_min") {
    FlowerHeap_t<uint64_t> h1(1, 10 * EMHS::B);
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
        if (h1.top() != res[i])
            break;
        h1.extract();
    }
}

TEST_CASE("flower-heap: random test#1") {
    FlowerHeap_t<uint64_t> h1(1, 10 * EMHS::B);
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

TEST_CASE("flower-heap: random test#2") {
    FlowerHeap_t<uint64_t> h1(1, 10 * EMHS::B);
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

TEST_CASE("flower-heap: voids must be voids...") {
    FlowerHeap_t<uint64_t> h1(1, 10 * EMHS::B);
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

void add(std::multiset<int> &pq, FlowerHeap_t<uint64_t> &ph) {
    int x = rand();
    pq.insert(x);
    ph.add(x);
}

void extract(std::multiset<int> &pq, FlowerHeap_t<uint64_t> &ph, int k) {
    int x = *(pq.begin());
    int y = ph.top();
    pq.erase(pq.begin());
    ph.extract();
    REQUIRE_MESSAGE(x == y, "#" + std::to_string(k) +
                                ": minimums are not equal: " +
                                std::to_string(x) + " != " + std::to_string(y));
}

TEST_CASE("stress test with multiset: 10^5 operations") {
    std::multiset<int> pq;
    FlowerHeap_t<uint64_t> ph(1, 10 * EMHS::B);
    for (int i = 0; i < 100'000; ++i) {
        int r = rand() % 3;
        if (r < 2 or pq.empty()) {  //с большей вероятностью add
            add(pq, ph);
            REQUIRE(pq.size() == ph.size());
        } else {
            extract(pq, ph, i);
            REQUIRE(pq.size() == ph.size());
        }
    }
}

TEST_CASE("stress test with multiset: 10^5 operations") {
    std::multiset<int> pq;
    FlowerHeap_t<uint64_t> ph(1, 10 * EMHS::B);
    for (int i = 0; i < 100'000; ++i) {
        int r = rand() % 7;
        if (r < 7 or pq.empty()) {  //с большей вероятностью add
            add(pq, ph);
            REQUIRE(pq.size() == ph.size());
        } else {
            extract(pq, ph, i);
            REQUIRE(pq.size() == ph.size());
        }
    }
}

TEST_CASE("stress test with multiset: 10^5 operations") {
    std::multiset<int> pq;
    FlowerHeap_t<uint64_t> ph(1, 10 * EMHS::B);
    for (int i = 0; i < 100'000; ++i) {
        int r = rand() % 7;
        if (r < 6 or pq.empty()) {  //с большей вероятностью add
            add(pq, ph);
            REQUIRE(pq.size() == ph.size());
        } else {
            extract(pq, ph, i);
            REQUIRE(pq.size() == ph.size());
        }
    }
}

TEST_CASE("all") {
    std::cerr << "Input: " << I_COUNTER << "\n";
    std::cerr << "Output: " << O_COUNTER << "\n";
}

}  // namespace
