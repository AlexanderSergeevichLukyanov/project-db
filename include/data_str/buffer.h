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
#endif
#include <algorithm>
#include <cmath>
#include <memory>
template <typename T, size_t size_buffer, typename Comp = std::less<T>>
struct buffer {  //ничто иное как обычный minmax
private:
    Comp comp;
    size_t size_ = 0;
public:
    T buf[size_buffer];  // Min-Max
private:

    size_t gr_pa(size_t x) {  //дед
        if (x < 3) {          //слишком высоко
            abort();
        } else {
            return (x - 3) / 4;
        }
    } //ok

    size_t father(size_t x) {
        if (x == 0) {
            abort();
        } else {
            return (x - 1) / 2;
        }
    } //ok

    void sift_up_max(size_t x) {  //поднимаем максимум
        if (x > 2 && comp(buf[gr_pa(x)], buf[x])) {
            std::swap(buf[gr_pa(x)], buf[x]);
            sift_up_max(gr_pa(x));
        }
    } //ok

    void sift_up_min(size_t x) {  //поднимаем минимум
        if (x>2 && comp(buf[x], buf[gr_pa(x)])) {
            std::swap(buf[gr_pa(x)], buf[x]);
            sift_up_min(gr_pa(x));
        }
    } //ok
	
	[[nodiscard]] bool is_grchild(size_t par, size_t ch) const{
		return ((((par+1)*4-2)<ch) and (((par+1)*4+3)>ch));
	}

	[[nodiscard]] bool has_child(size_t par) const{
		return (par*2+1<size_);
	}

    [[nodiscard]] size_t min_child(
        size_t *child,
        int count=6) {  //возвращает индекс минимального ребетёнка (внучка)
        size_t res = child[0];
        T min_ch(buf[res]);
        for (int i = 1; i < count && i < size_; ++i) {
            if (comp(buf[child[i]], min_ch)) {
                min_ch = buf[child[i]];
                res = child[i];
            }
        }
        return res;
    } // ok

    void sift_down_min(size_t x) {
     
	 // if(x==-1) return;
        size_t y = x + 1;
        size_t child[6] = {2*x+1, 2*x+2, 4 * y - 1, 4 * y, 4 * y + 1, 4 * y + 2};
		if(has_child(x)){
			size_t m = min_child(child);
			if(is_grchild(x, m)){
				if(comp(buf[m], buf[x])){
					std::swap(buf[m], buf[x]);
					if(comp(buf[father(m)], buf[m])){
						std::swap(buf[father(m)], buf[m]);
					}
					sift_down_min(m);
				}
			} else if(comp(buf[m], buf[x])){
				std::swap(buf[m], buf[x]);
			}
		}
/*
        if (size_ <= child[0]){
			int i = x;
            if (size_ <= 2 * i + 1) {
                } else if (size_ <= 2 * i + 2) {
                    if (comp(buf[2 * i + 1], buf[i])) {
                        std::swap(buf[i], buf[2 * i + 1]);
                    }
                } else {
                    if (comp(buf[2 * i + 2], buf[2 * i + 1])) {
                        if (comp(buf[2*i+2], buf[i])) {
                            std::swap(buf[i], buf[2 * i + 2]);
                        }
                    } else {
                        if (comp(buf[2*i+1], buf[i])) {
                            std::swap(buf[i], buf[2 * i + 1]);
                        }
                    }
                }
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
		*/
    }

    [[nodiscard]] size_t max_child(
        size_t *child,
        int count=6) {  //возвращает индекс минимального ребёнка
        size_t res = child[0];
        T max_ch(buf[res]);
        for (int i = 1; i < count && i<size_; ++i) {
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
        size_t child[6] = {2*x+1, 2*x+2, 4 * y - 1, 4 * y, 4 * y + 1, 4 * y + 2};
        // assert(!is_min_level(x));
		if(has_child(x)){
			size_t m = max_child(child);
			if(is_grchild(x, m)){
				if(comp(buf[x], buf[m])){
					std::swap(buf[x], buf[m]);
					if(comp(buf[m], buf[father(m)])){
						std::swap(buf[m], buf[father(m)]);
					}
					sift_up_max(m);
				}
			} else if(comp(buf[x], buf[m])){
				std::swap(buf[x], buf[m]);
			}
		}
		/*
        if (size_ <= child[0]) {
			int i = x;
            if (size_ <= 2 * i + 1) {
                } else if (size_ <= 2 * i + 2) {
                    if (comp(buf[i], buf[2 * i + 1])) {
                        std::swap(buf[i], buf[2 * i + 1]);
                    }
                } else {
                    if (comp(buf[2 * i + 1], buf[2 * i + 2])) {
                        if (comp(buf[i], buf[2 * i + 2])) {
                            std::swap(buf[i], buf[2 * i + 2]);
                        }
                    } else {
                        if (comp(buf[i], buf[2 * i + 1])) {
                            std::swap(buf[i], buf[2 * i + 1]);
                        }
                    }
                }
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
                        if (comp(buf[max_ch], buf[2 * i + 2])) {
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
        }*/
    }

    [[nodiscard]] bool is_min_level(size_t ind) {  //мы на уровне минимумов?
		if(ind==0) return true;
        return (static_cast<int>(log2(++ind)) % 2 == 0);  // TODO: & 1
    }

	void sift_up(size_t x){
		if(x!=0){
			if(is_min_level(x)){
				if(comp(buf[father(x)], buf[x])){
					std::swap(buf[father(x)], buf[x]);
					sift_up_max(father(x));
				} else{
					sift_up_min(x);
				}
			} else{
				if(comp(buf[x], buf[father(x)])){
					std::swap(buf[x], buf[father(x)]);
					sift_up_min(father(x));
				} else{
					sift_up_max(x);
				}
			}
		}
	}
	
	void sift_down(size_t x){
		if(is_min_level(x)){
			sift_down_min(x);
		} else{
			sift_down_max(x);
		}
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
            sift_down(1);
            // max_val = &buf[1];
        } else {
            std::swap(buf[2], buf[--size_]);
            sift_down(2);
        }
        /// size_--;
    }

    const T &getMin() const {
        return buf[0];
    }

    const T &getMax() const {
        if (size_ == 1) {
            return buf[0];
        } else if (size_ == 2) { //opt!
			if(comp(buf[0], buf[1])){
				return buf[1];
			} else{
				return buf[0];
			}
        } else {
            if (comp(buf[1], buf[2]))
                return buf[2];
            else
                return buf[1];
            // return comp(buf[1], buf[2]) ? buf[2] : buf[1];
        }
    }

    [[nodiscard]] std::size_t size() const{
        return size_;
    } //ok

    [[nodiscard]] bool empty() const{
        return (size_ == 0);
    } //ok

    void insert(const T &x) {  // TODO!
		buf[size_++]=x;
		sift_up(size_-1);
       /* if (size_ == 0) {
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
            // // // assert(fath==-1);
            if (comp(buf[ind],
                     buf[fath])) {  //мы на уровне максимумов, должны быть
                //больше своего отца, иначе:
                std::swap(buf[fath], buf[ind]);
                sift_up_min(fath);
            }
            sift_up_max(ind);
        }*/
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
