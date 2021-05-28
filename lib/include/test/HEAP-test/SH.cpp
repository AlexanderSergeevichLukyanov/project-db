#include "SoftHeap.h"
#include "mytest.h"

TEST_CASE("1'000 operations: add(Pr=~15/16); extract(Pr=~1/16)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 1'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 15) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}

TEST_CASE("10'000 operations: add(Pr=~15/16); extract(Pr=~1/16)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 10'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 15) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}

TEST_CASE("100'000 operations: add(Pr=~15/16); extract(Pr=~1/16)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 100'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 15) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}

TEST_CASE("1'000'000 operations: add(Pr=~15/16); extract(Pr=~1/16)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 1'000'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 15) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}

TEST_CASE("10'000'000 operations: add(Pr=~15/16); extract(Pr=~1/16)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 10'000'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 15) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}

TEST_CASE("100'000'000 operations: add(Pr=~15/16); extract(Pr=~1/16)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 100'000'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 15) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}

TEST_CASE("1'000'000'000 operations: add(Pr=~15/16); extract(Pr=~1/16)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 1'000'000'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 15) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}

TEST_CASE("1'000 operations: add(Pr=~7/8); extract(Pr=~1/8)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 1'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 7) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}

TEST_CASE("10'000 operations: add(Pr=~7/8); extract(Pr=~1/8)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 10'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 7) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}

TEST_CASE("100'000 operations: add(Pr=~7/8); extract(Pr=~1/8)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 100'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 7) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}

TEST_CASE("1'000'000 operations: add(Pr=~7/8); extract(Pr=~1/8)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 1'000'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 7) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}

TEST_CASE("10'000'000 operations: add(Pr=~7/8); extract(Pr=~1/8)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 10'000'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 7) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}

TEST_CASE("100'000'000 operations: add(Pr=~7/8); extract(Pr=~1/8)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 100'000'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 7) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}

TEST_CASE("1'000'000'000 operations: add(Pr=~7/8); extract(Pr=~1/8)") {
    std::size_t start_I = I_COUNTER;
    std::size_t start_O = O_COUNTER;
    SoftHeap<int> h;
    for (int i = 0; i < 1'000'000'000; ++i) {
        int r = rand();
        if (!h.empty() && (r & 7) == 0) {
            h.extract();
        } else {
            int x = rand();
            h.add(x);
        }
    }
    CHECK_TIME("Total. Insert: " + std::to_string(I_COUNTER - start_I) +
               "; Read: " + std::to_string(O_COUNTER - start_O));
}
