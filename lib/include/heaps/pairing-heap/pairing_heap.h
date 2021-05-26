#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

//#define pairing_wrong_test //не стоит расскоментировать:)
#define pairing_construct    // default constructor
#define pairing_insert       // insert(T x)
#define pairing_get_min      // getMin()
#define pairing_make         // make(T* array, size_t n)
#define pairing_extract_min  // extractMin()
#define pairing_solyanka     // ... , check const, reference, voids methods and
                             // constructors =&& &&
#define pairing_construct_comp    // default constructor(comp)
#define pairing_insert_comp       // insert(T x) with Compare
#define pairing_get_min_comp      // getMin() with Compare
#define pairing_make_comp         // make(T* array, size_t n) with Compare
#define pairing_extract_min_comp  // extractMin() with Compare
#define pairing_solyanka_comp     // ... , check const, reference, voids methods
                                  // and constructors =&& && with Compare
#define pairing_heap_  ////расскоментировать, когда куча будет готова
namespace {
template <typename T>
struct HeapNode {
    T key;
    HeapNode *leftChild = nullptr;
    HeapNode *nextSibling = nullptr;
    
    HeapNode() = default;
    HeapNode(T &key_) : key(key_){}
    HeapNode(T &key_, HeapNode *leftChild_, HeapNode *nextSibling_)
        : key(key_), leftChild(leftChild_), nextSibling(nextSibling_) {
    }

    void addChild(HeapNode *node) {
        if (!leftChild)
            leftChild = node;
        else {
            node->nextSibling = leftChild;
            leftChild = node;
        }
    }
};

template <typename T, typename Compare = std::less<T>>
HeapNode<T> *Merge(HeapNode<T> *A, HeapNode<T> *B, Compare &comp) {
    if (!A){
        return B;
    }
    if (!B){
        return A;
    }

    if ( comp(A->key, B->key) ) {
        A->addChild(B);
        return A;
    } else {
        B->addChild(A);
        return B;
    }

}

template <typename T, typename Compare = std::less<T>>
HeapNode<T> *Insert(HeapNode<T> *node, T &key, Compare &comp) {
    return Merge(node, new HeapNode<T>(key), comp);
}

template <typename T, typename Compare = std::less<T>>
HeapNode<T> *TwoPassMerge(HeapNode<T> *node, Compare &comp) {
    if ( !node ||  !(node->nextSibling) )
        return node;
    else {
        HeapNode<T> *A, *B, *newNode;
        A = node;
        B = node->nextSibling;
        newNode = node->nextSibling->nextSibling;

        A->nextSibling = nullptr;
        B->nextSibling = nullptr;

        return Merge(Merge(A, B, comp), TwoPassMerge(newNode, comp), comp);
    }
}

template <typename T, typename Compare = std::less<T>>
HeapNode<T> *Delete(HeapNode<T> *node, Compare &comp) {
    return TwoPassMerge(node->leftChild, comp);
}

}  // namespace
template <typename T, typename Compare = std::less<T>>
struct pairing_heap {
private:
    HeapNode<T> *root = nullptr;
    int size_ = 0;
    Compare comp;

public:
    pairing_heap() = default;
    explicit pairing_heap(const Compare &comp_) : comp(comp_){
    }
    pairing_heap(const pairing_heap &) = delete;
    pairing_heap &operator=(const pairing_heap &) = delete;
    pairing_heap(pairing_heap &&other) = default;
    pairing_heap &operator=(pairing_heap &&other) = default;
    ~pairing_heap() = default;
    
    [[nodiscard]] int size() const {
        return size_;
    }

    [[nodiscard]] bool empty() const {
        return (size_ == 0);
    }

    const T &getMin() const {
        return (*root).key;
    }

    void insert(T key) {
        root = ::Insert(root, key, comp);
        size_++;
    }

    void extractMin() {
        root = ::Delete(root, comp);
        size_--;
    }

    void make(const T *p, size_t n) {
        for (size_t i = 0; i < n; ++i) {
            insert(p[i]);
        }
    }

    void merge(pairing_heap &other) {  // not tested!
        root = ::Merge(root, other.root, comp);
    }
};