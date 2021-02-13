#include <cassert>
#include <cmath>
#include <functional>
#include <type_traits>
#include <utility>

//#define funnel_wrong_test //не стоит расскоментировать:)
//#define funnel_construct //default constructor
//#define funnel_insert  // insert(T x)
//#define funnel_get_min // getMin()
//#define funnel_make // make(T* array, size_t n)
//#define funnel_extract_min // extractMin()
//#define funnel_solyanka // ... , check const, reference, voids methods and
// constructors =&& && #define funnel_construct_comp //default constructor(comp)
//#define funnel_insert_comp // insert(T x) with Compare
//#define funnel_get_min_comp // getMin() with Compare
//#define funnel_make_comp // make(T* array, size_t n) with Compare
//#define funnel_extract_min_comp // extractMin() with Compare
//#define funnel_solyanka_comp // ... , check const, reference, voids methods
// and constructors =&& && with Compare #define funnel_heap_
// //расскоментировать, когда куча будет готова

template <typename T, typename Compare = std::less<T>>
struct funnel_heap {};