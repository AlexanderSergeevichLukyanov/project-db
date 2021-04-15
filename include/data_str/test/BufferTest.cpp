#include "mytest.h" 
#include "buffer.h"

TEST_CASE("insert & getters & size") {
    buffer<int, 32> buf;
    CHECK(buf.empty());
    CHECK(buf.size() == 0);
    buf.insert(1);
    CHECK(buf.size() == 1);
    CHECK(buf.getMin() == 1);
    CHECK(buf.getMax() == 1);
    CHECK(!buf.empty());
    buf.insert(0);
    CHECK(buf.getMin() == 0);
    CHECK(buf.getMax() == 1);
    buf.insert(47);
    CHECK(buf.getMin() == 0);
    CHECK(buf.getMax() == 47);
    for (int i = 0; i < 29; ++i) {
        buf.insert(i * 2);
    }
    CHECK(buf.size() == 32);
    CHECK(buf.getMin() == 0);
    CHECK(buf.getMax() == 56);
    CHECK(!buf.empty());
}

TEST_CASE("extract") {
    buffer<int, 32> buf;
    buf.insert(1);
    buf.extractMin();
    CHECK(buf.empty());
    CHECK(buf.size() == 0);
    for (int i = 0; i < 10; ++i) {
        buf.insert(i * 2);
    }
    CHECK(buf.getMin() == 0);
    buf.extractMin();
    CHECK(buf.size() == 9);
    CHECK_MESSAGE(buf.getMin() == 2,
                  "your answer is " + std::to_string(buf.getMin()));
    buf.extractMax();
    CHECK_MESSAGE(buf.getMax() == 16,
                  "your answer is " + std::to_string(buf.getMax()));
    CHECK(buf.size() == 8);
    buf.extractMax();
    CHECK_MESSAGE(buf.getMax() == 14,
                  "your answer is " + std::to_string(buf.getMax()));
    CHECK(buf.size() == 7);
}

TEST_CASE("random #1") {
    buffer<int, 1024> buf;
    int maxi = INT_MIN;
    int mini = INT_MAX;
    for (int i = 0; i < 1024; ++i) {
        int x = rand();
        maxi = std::max(x, maxi);
        mini = std::min(x, mini);
        buf.insert(x);
    }
    CHECK(buf.getMin() == mini);
    CHECK(buf.getMax() == maxi);
}

TEST_CASE("random #2") {
    buffer<int, 1024> buf;
    std::vector<int> for_check;
    for (int i = 0; i < 1024; ++i) {
        int x = rand();
        for_check.push_back(x);
        buf.insert(x);
    }
    std::sort(for_check.begin(), for_check.end());
    CHECK(buf.getMin() == for_check[0]);
    CHECK(buf.getMax() == for_check[1023]);
    buf.extractMax();
    CHECK(buf.getMin() == for_check[0]);
    CHECK(buf.getMax() == for_check[1022]);
    buf.extractMin();
    CHECK(buf.getMin() == for_check[1]);
    CHECK(buf.getMax() == for_check[1022]);
    buf.extractMin();
    CHECK(buf.getMin() == for_check[2]);
    CHECK(buf.getMax() == for_check[1022]);
    buf.extractMax();
    CHECK(buf.getMin() == for_check[2]);
    CHECK(buf.getMax() == for_check[1021]);
}

TEST_CASE("time-test #1 (extractMax)") {
    buffer<int, 30000> buf;
    for (int i = 0; i < 30000; ++i) {
        buf.insert(rand());
    }
	CHECK(buf.size()==30000);
    CHECK_TIME("After 30000 insert");
    for (int i = 0; i < 30000; ++i) {
        buf.extractMax();
    }
    CHECK(buf.empty());
}

TEST_CASE("time-test #2 (extractMin)") {
    buffer<int, 30000> buf;
    for (int i = 0; i < 30000; ++i) {
        buf.insert(rand());
    }
	CHECK(buf.size()==30000);
    CHECK_TIME("After 30000 insert");
    for (int i = 0; i < 30000; ++i) {
        buf.extractMin();
    }
    CHECK(buf.empty());
}

TEST_CASE("2 cycle") {
    buffer<int, 30000> buf;
    for (int i = 0; i < 30000; ++i) {
        buf.insert(rand());
    }
    CHECK_TIME("After 30000 insert");
    for (int i = 0; i < 30000; ++i) {
        buf.extractMin();
    }
    CHECK(buf.empty());
    CHECK_TIME("After 1st cycle");
    for (int i = 0; i < 30000; ++i) {
        buf.insert(rand());
    }
    CHECK_TIME("After 30000 insert");
    for (int i = 0; i < 30000; ++i) {
        buf.extractMin();
    }
    CHECK(buf.empty());
}

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

TEST_CASE("with compare #1") {
    buffer<int, 6000, CloserTo> h1(CloserTo(10));
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

TEST_CASE("with compare #2") {
    buffer<int, 6000, CloserTo> h1(CloserTo(10));
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    h1.insert(1);
    CHECK(h1.size() == 1);
    CHECK(!h1.empty());
    h1.extractMax();
    CHECK(h1.empty());
    CHECK(h1.size() == 0);
    for (int i = 0; i < 5000; ++i) {
        h1.insert(i);
    }
    CHECK(h1.getMax() == 4999);
    h1.extractMax();
    CHECK(h1.size() == 4999);
    CHECK(h1.getMax() == 4998);
    h1.extractMin();
    CHECK(h1.getMin() == 9 or h1.getMin() == 11);
}

TEST_CASE("random #1 with compare") {
    buffer<int, 1024, CloserTo> buf(CloserTo(10));
    std::vector<int> for_check;
    for (int i = 0; i < 1024; ++i) {
        int x = rand();
        for_check.push_back(std::abs(x - 10));
        buf.insert(x);
    }
    std::sort(for_check.begin(), for_check.end());
    CHECK(std::abs(buf.getMin() - 10) == for_check[0]);
    CHECK(std::abs(buf.getMax() - 10) == for_check[1023]);
    buf.extractMax();
    CHECK(std::abs(buf.getMin() - 10) == for_check[0]);
    CHECK(std::abs(buf.getMax() - 10) == for_check[1022]);
    buf.extractMin();
    CHECK(std::abs(buf.getMin() - 10) == for_check[1]);
    CHECK(std::abs(buf.getMax() - 10) == for_check[1022]);
    buf.extractMin();
    CHECK(std::abs(buf.getMin() - 10) == for_check[2]);
    CHECK(std::abs(buf.getMax() - 10) == for_check[1022]);
    buf.extractMax();
    CHECK(std::abs(buf.getMin() - 10) == for_check[2]);
    CHECK(std::abs(buf.getMax() - 10) == for_check[1021]);
}

TEST_CASE("time-test #1 (extractMax) with compare") {
    buffer<int, 30000, CloserTo> buf(CloserTo(14));
    for (int i = 0; i < 30000; ++i) {
        buf.insert(rand());
    }
	CHECK(buf.size()==30000);
    CHECK_TIME("After 30000 insert");
    for (int i = 0; i < 30000; ++i) {
        buf.extractMax();
    }
    CHECK(buf.empty());
}

TEST_CASE("time-test #2 (extractMin) with compare") {
    buffer<int, 30000, CloserTo> buf(CloserTo(14));
    for (int i = 0; i < 30000; ++i) {
        buf.insert(rand());
    }
	CHECK(buf.size()==30000);
    CHECK_TIME("After 30000 insert");
    for (int i = 0; i < 30000; ++i) {
        buf.extractMin();
    }
    CHECK(buf.empty());
}