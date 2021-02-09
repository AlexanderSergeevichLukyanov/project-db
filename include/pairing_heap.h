#include <cassert>
#include <cmath>
#include <functional>
#include <type_traits>
#include <utility>

//#define pairing_wrong_test //не стоит расскоментировать:)
//#define pairing_construct //default constructor
//#define pairing_insert  // insert(T x)
//#define pairing_get_min // getMin()
//#define pairing_make // make(T* array, size_t n) 
//#define pairing_extract_min // extractMin()
//#define pairing_solyanka // ... , check const, reference, voids methods and constructors =&& &&
//#define pairing_construct_comp //default constructor(comp)
//#define pairing_insert_comp // insert(T x) with Compare
//#define pairing_get_min_comp // getMin() with Compare
//#define pairing_make_comp // make(T* array, size_t n) with Compare
//#define pairing_extract_min_comp // extractMin() with Compare
//#define pairing_solyanka_comp // ... , check const, reference, voids methods and constructors =&& && with Compare
//#define pairing_heap_ //расскоментировать, когда куча будет готова

template <typename T, typename Compare = std::less<T>>
struct pairing_heap{
	
};