#include <cassert>
#include <cmath>
#include <functional>
#include <list>
#include <type_traits>
#include <utility>

//#define list_wrong_test //не стоит расскоментировать:)
#define list_construct       // default constructor
#define list_insert          // insert(T x)
#define list_get_min         // getMin()
#define list_make            // make(T* array, size_t n)
#define list_extract_min     // extractMin()
#define list_solyanka        // ... , check const, reference, voids methods and
                             // constructors =&& &&
#define list_construct_comp  // default constructor(comp)
#define list_insert_comp     // insert(T x) with Compare
#define list_get_min_comp    // getMin() with Compare
#define list_make_comp       // make(T* array, size_t n) with Compare
#define list_extract_min_comp  // extractMin() with Compare
#define list_solyanka_comp  // ... , check const, reference, voids methods and
                            // constructors =&& && with Compare
#define list_heap_  //расскоментировать, когда куча будет готова

template <typename T, typename Compare = std::less<T>>
struct list_heap {
private:
    std::list<T> list_for_heap;
    typename std::list<T>::iterator iter_min;
    Compare comp;

public:
    // Default copying is not good enough: the copied iterator should point to
    // the copied list, not the original one. Do not modify following five
    // lines.
    // list_heap(/const Compare &compare = Compare()*/) : comp(compare){};
    list_heap() = default;
    explicit list_heap(const Compare &compare) : comp(compare){};
    list_heap(const list_heap &) = delete;
    list_heap &operator=(const list_heap &) = delete;
    list_heap(list_heap &&) noexcept = default;
    list_heap &operator=(list_heap &&) noexcept = default;
    ~list_heap() = default;

    [[nodiscard]] int size() const {
        return list_for_heap.size();
    }

    [[nodiscard]] bool empty() const {
        return list_for_heap.empty();
    }

    void insert(const T &value) {
        list_for_heap.push_back(value);

        if (list_for_heap.size() == 1 || comp(value, *iter_min)) {
            iter_min = --list_for_heap.end();
        }
    }

    [[nodiscard]] const T &getMin() const {
        assert(!empty());
        return *iter_min;
    }

    void merge(list_heap &other) {
        if (other.empty()) {
            return;
        }
        if (empty() || comp(*(other.iter_min), *iter_min)) {
            iter_min = other.iter_min;
        }
        list_for_heap.splice(list_for_heap.end(), other.list_for_heap);
    }

    void make(const T *p, size_t n) {
        for (size_t i = 0; i < n; ++i) {
            insert(p[i]);
        }
    }

    void extractMin() {
        list_for_heap.erase(iter_min);
        if (empty()) {
            return;
        }
        iter_min = list_for_heap.begin();
        for (auto iter = list_for_heap.begin(); iter != list_for_heap.end();
             ++iter) {
            if (comp(*iter, *iter_min)) {
                iter_min = iter;
            }
        }
    }
};
