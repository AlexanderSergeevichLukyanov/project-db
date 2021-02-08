#include <cassert>
#include <cmath>
#include <functional>
#include <type_traits>
#include <utility>

//#define soft_wrong_test //не стоит расскоментировать:)
//#define soft_construct //default constructor
//#define soft_insert  // insert(T x)
//#define soft_get_min // getMin()
//#define soft_make // make(T* array, size_t n) 
//#define soft_extract_min // extractMin()
//#define soft_solyanka // ... , check const, reference, voids methods and constructors =&& &&
//#define soft_construct_comp //default constructor(comp)
//#define soft_insert_comp // insert(T x) with Compare
//#define soft_get_min_comp // getMin() with Compare
//#define soft_make_comp // make(T* array, size_t n) with Compare
//#define soft_extract_min_comp // extractMin() with Compare
//#define soft_solyanka_comp // ... , check const, reference, voids methods and constructors =&& && with Compare

template <typename T, typename Compare = std::less<T>>
struct soft_heap{
	
}