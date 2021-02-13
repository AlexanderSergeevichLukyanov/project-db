#include <cassert>
#include <cmath>
#include <functional>
#include <type_traits>
#include <utility>

//#define buffer_wrong_test //не стоит расскоментировать:)
//#define buffer_construct //default constructor
//#define buffer_insert  // insert(T x)
//#define buffer_get_min // getMin()
//#define buffer_make // make(T* array, size_t n)
//#define buffer_extract_min // extractMin()
//#define buffer_solyanka // ... , check const, reference, voids methods and
//constructors =&& && #define buffer_construct_comp //default constructor(comp)
//#define buffer_insert_comp // insert(T x) with Compare
//#define buffer_get_min_comp // getMin() with Compare
//#define buffer_make_comp // make(T* array, size_t n) with Compare
//#define buffer_extract_min_comp // extractMin() with Compare
//#define buffer_solyanka_comp // ... , check const, reference, voids methods
//and constructors =&& && with Compare #define buffer_heap_ //расскоментировать,
//когда куча будет готова

template <typename T, typename Compare = std::less<T>>
struct buffer_heap {};