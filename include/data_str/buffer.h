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

        make(T* arr, size_t s) - сделать буфер на массиве(для сброса всяческих
   блоков в буфер) Ещё: Массив плотно (подряд) - T buf

        TODO:
        Перенос блока на диск - ничто иное, как buf до size_ перенести в диск
        (?) save() - сохранить буфер на диск (для встроенных СУБД)
*/

#include <algorithm>
#include <cmath>
#include <memory>

template <typename T, std::size_t size_buffer, typename Comp = std::less<T>>
struct buffer {  //ничто иное как обычный minmax
private:
    Comp comp;
    std::size_t size_ = 0;
    T buf[size_buffer];  // Min-Max

    std::size_t gr_pa(size_t x) {  //дед
        if (x < 3) {               //слишком высоко
            abort();
        } else {
            return (x - 3) / 4;
        }
    }

    std::size_t father(size_t x) {
        if (x == 0) {
            abort();
        } else {
            return (x - 1) / 2;
        }
    }
	
	[[nodiscard]] bool is_min_level(std::size_t ind) const{  // мы на уровне минимумов?
        return ((static_cast<int>(log2(1 + ind)) & 1) == 0);  // идейный остаток по модулю 2, зато быстрый) TODO: fast log2
    }

    void sift_up_max(size_t x) {  //поднимаем максимум
        if (x > 2 && comp(buf[gr_pa(x)], buf[x])) {
            std::swap(buf[gr_pa(x)], buf[x]);
            sift_up_max(gr_pa(x));
        }
    }

    void sift_up_min(std::size_t x) {  //поднимаем минимум
        if (x > 2 && comp(buf[x], buf[gr_pa(x)])) {
            std::swap(buf[gr_pa(x)], buf[x]);
            sift_up_min(gr_pa(x));
        }
    }

    [[nodiscard]] bool is_grchild(std::size_t par, std::size_t ch) const { // является ли ch внуком par ?
        return ((((par + 1) * 4 - 2) < ch) && (((par + 1) * 4 + 3) > ch));
    }

    [[nodiscard]] bool has_child(std::size_t par) const { // есть ли у par дети?
        return (par * 2 + 1 < size_);
    }

    [[nodiscard]] std::size_t min_child( // возвращает минимальный элемент из передеанного массива индексов
        std::size_t *child,
        int count = 6) {  //возвращает индекс минимального ребетёнка (внучка)
        std::size_t res = child[0];
        T min_ch(buf[res]);
        for (int i = 1; i < count && child[i] < size_; ++i) {
            if (comp(buf[child[i]], min_ch)) {
                min_ch = buf[child[i]];
                res = child[i];
            }
        }
        return res;
    }

    void sift_down_min(std::size_t x) { // толкаем вниз по минимумам
        size_t y = x + 1;
        size_t child[6] = {2 * x + 1, 2 * x + 2, 4 * y - 1,
                           4 * y,     4 * y + 1, 4 * y + 2};

        if (has_child(x)) {
            size_t m = min_child(child);
            if (is_grchild(x, m)) {
                if (comp(buf[m], buf[x])) {
                    std::swap(buf[m], buf[x]);
                    if (comp(buf[father(m)], buf[m])) {
                        std::swap(buf[father(m)], buf[m]);
                    }
                    sift_down_min(m);
                }
            } else if (comp(buf[m], buf[x])) {
                std::swap(buf[m], buf[x]);
            }
        }
    }

    [[nodiscard]] std::size_t max_child( // возвращает минимальный элемент из передеанного массива индексов
        size_t *child,
        int count = 6) {  //возвращает индекс максимального ребёнка
        size_t res = child[0];
        T max_ch(buf[res]);
        for (int i = 1; i < count && child[i] < size_; ++i) {
            if (comp(max_ch, buf[child[i]])) {
                max_ch = buf[child[i]];
                res = child[i];
            }
        }
        return res;
    }

    void sift_down_max(std::size_t x) { // толкаем вниз по максимумам
        std::size_t y = x + 1;
        std::size_t child[6] = {2 * x + 1, 2 * x + 2, 4 * y - 1,
                           4 * y,     4 * y + 1, 4 * y + 2};

        if (has_child(x)) {
            std::size_t m = max_child(child);
            if (is_grchild(x, m)) {
                if (comp(buf[x], buf[m])) {
                    std::swap(buf[x], buf[m]);
                    if (comp(buf[m], buf[father(m)])) {
                        std::swap(buf[m], buf[father(m)]);
                    }
                    sift_down_max(m);
                }
            } else if (comp(buf[x], buf[m])) {
                std::swap(buf[x], buf[m]);
            }
        }
    }

    void sift_up(std::size_t x) { // толкаем вверх
        if (x != 0) {
            if (is_min_level(x)) {
                if (comp(buf[father(x)], buf[x])) {
                    std::swap(buf[father(x)], buf[x]);
                    sift_up_max(father(x));
                } else {
                    sift_up_min(x);
                }
            } else {
                if (comp(buf[x], buf[father(x)])) {
                    std::swap(buf[x], buf[father(x)]);
                    sift_up_min(father(x));
                } else {
                    sift_up_max(x);
                }
            }
        }
    }

    void sift_down(std::size_t x) { // толкаем вниз
        if (is_min_level(x)) {
            sift_down_min(x);
        } else {
            sift_down_max(x);
        }
    }

public:
    buffer() = default;
    explicit buffer(const Comp &comp_) : comp(comp_) {
    }

    void extractMin() { 
        if (size_ == 1) {
            size_--;
            return;
        }

        std::swap(buf[0], buf[--size_]);
        sift_down(0);
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

        if (comp(buf[2], buf[1])) {
            std::swap(buf[1], buf[--size_]);
            sift_down(1);
        } else {
            std::swap(buf[2], buf[--size_]);
            sift_down(2);
        }
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
            if (comp(buf[1], buf[2])) {
                return buf[2];
            } else {
                return buf[1];
            }
        }
    }

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    [[nodiscard]] bool empty() const {
        return (size_ == 0);
    }

    void insert(const T &x) {
        buf[size_++] = x;
        sift_up(size_ - 1);
    }

    //дальше ничего содержательного

    void make(T *values, size_t s) {  //например, сбросить в буфер блок
        for (int i = 0; i < s; ++i) {
            buf[i] = std::move(values[i]);
        }
    }

    void save() {  // TODO!
    }
};
