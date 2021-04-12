/*
	Конструктор :
	buffer<тип, размер, компаратор less по умолчанию>
	
	Методы:
	приватные - вспомогательные
	extractMin() - удалить минимум O(log n)
	extractMax() - убрать максимум O(log n)
	getMin() - узнать минимум
	getMax() - узнать максимум (для голов нужно), асимптотику не ухудшит
	insert(const T& x) - вставить O(log n)
	size(), empty()
	
	make(T* arr, size_t s) - сделать буфер на массиве(для сброса всяческих блоков в буфер)
	Ещё:
	Массив плотно (подряд) - T buf
	
	TODO:
	Перенос блока на диск - ничто иное, как buf до size_ перенести в диск
	(?) save() - сохранить буфер на диск (для встроенных СУБД)
*/


//#define debug

#ifdef debug
#include <cassert>
#include <string>
#include <vector>
#include "mytest.h"
#endif
#include <algorithm>
#include <cmath>
#include <memory>
template <typename T, size_t size_buffer, typename Comp = std::less<T>>
struct buffer {  //ничто иное как обычный minmax
private:
    Comp comp;
    size_t size_ = 0;
    T buf[size_buffer];  // Min-Max

    size_t gr_pa(size_t x) {  //дед
        if (x < 4) {          //слишком высоко
            abort();
        } else {
            return (x - 3) / 4;
        }
    }

    size_t father(size_t x) {
        if (x == 0) {
            abort();
        } else {
            return (x - 1) / 2;
        }
    }

    void sift_up_max(size_t x) {  //поднимаем максимум
        if (x < 4)
            return;
        size_t gr_pa_ = gr_pa(x);
        if (comp(buf[gr_pa_], buf[x])) {
            std::swap(buf[gr_pa_], buf[x]);
            sift_up_max(gr_pa_);
        }
    }

    void sift_up_min(size_t x) {  //поднимаем минимум
        if (x < 4)
            return;
        size_t gr_pa_ = gr_pa(x);
        if (comp(buf[x], buf[gr_pa_])) {
            std::swap(buf[gr_pa_], buf[x]);
            sift_up_min(gr_pa_);
        }
    }

    [[nodiscard]] size_t min_child(
        size_t *child,
        int count) {  //возвращает индекс минимального ребетёнка
        size_t res = child[0];
        T min_ch(buf[res]);
        for (int i = 1; i <= count; ++i) {
            if (comp(buf[child[i]], min_ch)) {
                min_ch = buf[child[i]];
                res = child[i];
            }
        }
        return res;
    }

    void sift_down_min(size_t x) {
        // if(x==-1) return;
        size_t y = x + 1;
        size_t child[4] = {4 * y - 1, 4 * y, 4 * y + 1, 4 * y + 2};
        assert(is_min_level(x));
        if (size_ <= child[0]) {
            //всё
            // std::cerr<<"lol";
        } else {
            size_t min_ch =
                min_child(child, std::min(child[3], size_ - 1) -
                                     child[0]);  //выбрали младшенького внучка
            // std::cerr<<min_ch;
            if (comp(buf[min_ch],
                     buf[x])) {  //если дедушка не младше - фигово, тут всё
                                 //наоборот
                std::swap(buf[min_ch], buf[x]);
                size_t fath = father(min_ch);
                if (comp(buf[fath],
                         buf[min_ch])) {  //а не конфликтуем ли с папой?
                    //он больше должен быть
                    std::swap(buf[fath], buf[min_ch]);
                }
                int i = min_ch;
                if (size_ <= 2 * i + 1) {
                } else if (size_ <= 2 * i + 2) {
                    if (comp(buf[2 * i + 1], buf[min_ch])) {
                        std::swap(buf[min_ch], buf[2 * i + 1]);
                    }
                } else {
                    if (comp(buf[2 * i + 2], buf[2 * i + 2])) {
                        if (comp(buf[2 * i + 2], buf[min_ch])) {
                            std::swap(buf[min_ch], buf[2 * i + 2]);
                        }
                    } else {
                        if (comp(buf[2 * i + 1], buf[min_ch])) {
                            std::swap(buf[min_ch], buf[2 * i + 1]);
                        }
                    }
                }
                sift_down_min(min_ch);  //продолжаем наше движение
            }
        }
    }

    [[nodiscard]] size_t max_child(
        size_t *child,
        int count) {  //возвращает индекс минимального ребёнка
        size_t res = child[0];
        T max_ch(buf[res]);
        for (int i = 1; i <= count; ++i) {
            if (comp(max_ch, buf[child[i]])) {
                max_ch = buf[child[i]];
                res = child[i];
            }
        }
        return res;
    }

    void sift_down_max(size_t x) {
        // if(x==-1) return;
        size_t y = x + 1;
        size_t child[4] = {4 * y - 1, 4 * y, 4 * y + 1, 4 * y + 2};
        assert(!is_min_level(x));
        if (size_ <= child[0]) {
            //всё
        } else {
            size_t max_ch =
                max_child(child, std::min(child[3], size_ - 1) -
                                     child[0]);  //выбрали старшенького внучка
            // std::cerr<<max_ch;
            if (comp(buf[x],
                     buf[max_ch])) {  //если дедушка не старше - фигово, неправильно это
                std::swap(buf[max_ch], buf[x]);  // TODO!!!!
                size_t i = max_ch;
                if (size_ <= 2 * i + 1) {
                } else if (size_ <= 2 * i + 2) {
                    if (comp(buf[max_ch], buf[2 * i + 1])) {
                        std::swap(buf[max_ch], buf[2 * i + 1]);
                    }
                } else {
                    if (comp(buf[2 * i + 1], buf[2 * i + 2])) {
                        if (buf[max_ch] < buf[2 * i + 2]) {
                            std::swap(buf[max_ch], buf[2 * i + 2]);
                        }
                    } else {
                        if (comp(buf[max_ch], buf[2 * i + 1])) {
                            std::swap(buf[max_ch], buf[2 * i + 1]);
                        }
                    }
                }
                sift_down_max(max_ch);  //продолжаем наше движение
            }
        }
    }

    [[nodiscard]] bool is_min_level(size_t ind) {  //мы на уровне минимумов?
        return (static_cast<int>(log2(++ind)) % 2 == 0);  // TODO: & 1
    }

public:
    buffer() = default;
    explicit buffer(const Comp &comp_) : comp(comp_) {
    }

    void extractMin() {  // TODO!
        if (size_ == 1) {
            size_--;
            return;
        }

        std::swap(buf[0], buf[--size_]);
        sift_down_min(0);
    }

    void extractMax() {  // TODO!
        if (size_ < 3) {
            size_--;
            return;
        }
        if (size_ == 3 && comp(buf[2], buf[1])) {
            buf[1] = buf[2];
            size_--;
            return;
        }  //минимальные случаи закончились

        // T *max_val = buf[2];
        if (comp(buf[2], buf[1])) {
            std::swap(buf[1], buf[--size_]);
            sift_down_max(1);
            // max_val = &buf[1];
        } else {
            std::swap(buf[2], buf[--size_]);
            sift_down_max(2);
        }
        /// size_--;
    }

    const T &getMin() const {
        return buf[0];
    }

    const T &getMax() const {
        if (size_ == 1) {
            return buf[0];
        } else if (size_ == 2) {
            return buf[1];
        } else {
            if (comp(buf[1], buf[2]))
                return buf[2];
            else
                return buf[1];
            // return comp(buf[1], buf[2]) ? buf[2] : buf[1];
        }
    }

    [[nodiscard]] size_t size() {
        return size_;
    }

    [[nodiscard]] bool empty() {
        return (size_ == 0);
    }

    void insert(const T &x) {  // TODO!
        if (size_ == 0) {
            buf[size_++] = x;
            return;
        }

        size_t ind = size_++;
        buf[ind] = x;
        if (is_min_level(ind)) {
            size_t fath = father(ind);
            // assert(fath==-1);
            if (comp(buf[fath],
                     buf[ind])) {  //мы должны быть меньше своего отца на
                //минимальном уровне, иначе:
                std::swap(buf[fath], buf[ind]);
                sift_up_max(fath);
            }
            sift_up_min(ind);
        } else {
            size_t fath = father(ind);
            // assert(fath==-1);
            if (comp(buf[ind],
                     buf[fath])) {  //мы на уровне максимумов, должны быть
                //больше своего отца, иначе:
                std::swap(buf[fath], buf[ind]);
                sift_up_min(fath);
            }
            sift_up_max(ind);
        }
        //вставка
    }

	void make(T* values, size_t s){ //например, сбросить в буфер блок
		for(int i=0; i<s; ++i){
			buf[i] = std::move(values[i]);
		}
	}

    void save() {  // TODO!
    }
};
#ifdef debug
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

#endif
