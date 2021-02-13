#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

//#define pairing_wrong_test //не стоит расскоментировать:)
#define pairing_construct //default constructor
#define pairing_insert   // insert(T x)
#define pairing_get_min  // getMin()
//#define pairing_make // make(T* array, size_t n)
#define pairing_extract_min // extractMin()
#define pairing_solyanka // ... , check const, reference, voids methods and constructors =&& && 
#define pairing_construct_comp //default constructor(comp)
#define pairing_insert_comp // insert(T x) with Compare
#define pairing_get_min_comp // getMin() with Compare
//#define pairing_make_comp // make(T* array, size_t n) with Compare
#define pairing_extract_min_comp // extractMin() with Compare
//#define pairing_solyanka_comp // ... , check const, reference, voids methods
//and constructors =&& && with Compare #define pairing_heap_
////расскоментировать, когда куча будет готова

template <typename T>
struct HeapNode {
    T key;
    HeapNode *leftChild;
    HeapNode *nextSibling;

    HeapNode() : leftChild(NULL), nextSibling(NULL) {
    }

    // creates a new node
    HeapNode(T key_, HeapNode *leftChild_, HeapNode *nextSibling_)
        : key(key_), leftChild(leftChild_), nextSibling(nextSibling_) {
    }

    // Adds a child and sibling to the node
    void addChild(HeapNode *node) {
        if (leftChild == NULL)
            leftChild = node;
        else {
            node->nextSibling = leftChild;
            leftChild = node;
        }
    }
};

template <typename T>
bool Empty(HeapNode<T> *node) {
    return (node == NULL);
}

template <typename T, typename Compare = std::less<T>>
HeapNode<T> *Merge(HeapNode<T> *A, HeapNode<T> *B, Compare &comp) {
    if (A == NULL)
        return B;
    if (B == NULL)
        return A;

    if (comp(A->key, B->key)) {
        A->addChild(B);
        return A;
    } else {
        B->addChild(A);
        return B;
    }

    return NULL;  // Unreachable
}

template <typename T>
T top(HeapNode<T> *node) {
    return node->key;
}

template <typename T, typename Compare = std::less<T>>
HeapNode<T> *Insert(HeapNode<T> *node, T &key, Compare &comp) {
    return Merge(node, new HeapNode<T>(key, NULL, NULL), comp);
}

template <typename T, typename Compare = std::less<T>>
HeapNode<T> *TwoPassMerge(HeapNode<T> *node, Compare &comp) {
    if (node == NULL || node->nextSibling == NULL)
        return node;
    else {
        HeapNode<T> *A, *B, *newNode;
        A = node;
        B = node->nextSibling;
        newNode = node->nextSibling->nextSibling;

        A->nextSibling = NULL;
        B->nextSibling = NULL;

        return Merge(Merge(A, B, comp), TwoPassMerge(newNode, comp), comp);
    }

    return NULL;  // Unreachable
}

template <typename T, typename Compare = std::less<T>>
HeapNode<T> *Delete(HeapNode<T> *node, Compare &comp) {
    return TwoPassMerge(node->leftChild, comp);
}

template <typename T, typename Compare = std::less<T>>
struct pairing_heap {
private:
    HeapNode<T> *root;
    int size_ = 0;
    Compare comp;

public:
    pairing_heap() : root(NULL) {
    }
	pairing_heap(const pairing_heap &) = delete;
    pairing_heap &operator=(const pairing_heap &) = delete;
    pairing_heap(pairing_heap &&) noexcept = default;
    pairing_heap &operator=(pairing_heap &&) noexcept = default;
	explicit pairing_heap(Compare &comp_): comp(comp_){
		
	}

    [[nodiscard]] int size() const {
        return size_;
    }

    [[nodiscard]] bool empty() const {
        return (size_ == 0);
    }

    const T& getMin() const {
        return (*root).key;
    }

    void insert(T key) {
        root = ::Insert(root, key, comp);
        size_++;
    }
	
	void extractMin(){
        root = ::Delete(root, comp); 
		size_--;
	}

    void merge(pairing_heap &other) {  // not tested!
        root = ::Merge(root, other.root);
    }
};
