#include <cassert>
#include <cmath>
#include <functional>
#include <type_traits>
#include <utility>

static const int DELETED = 2;
static const double default_epsilon = 0.000001;

//#define soft_wrong_test //не стоит расскоментировать:)
#define soft_construct  // default constructor
#define soft_insert     // insert(T x)
#define soft_get_min    // getMin()
//#define soft_make // make(T* array, size_t n)
#define soft_extract_min  // extractMin()
#define soft_solyanka     // ... , check const, reference, voids methods and
// constructors =&& && #define soft_construct_comp //default constructor(comp)
#define soft_insert_comp   // insert(T x) with Compare
#define soft_get_min_comp  // getMin() with Compare
//#define soft_make_comp // make(T* array, size_t n) with Compare
#define soft_extract_min_comp  // extractMin() with Compare
#define soft_solyanka_comp  // ... , check const, reference, voids methods and
// constructors =&& && with Compare
#define soft_heap_  //расскоментировать,
// когда куча будет готова

template <typename E, typename Compare = std::less<E>>
struct soft_heap {
    Compare comp;
    std::size_t size_ = 0;
    struct ListCell {
        E *elem = nullptr;
        int del;
        ListCell *next = nullptr;

        ListCell(E *e);

        ~ListCell();
    };

    struct Node {
        int rank, size = 0, num;
        E *ckey = nullptr;
        Node *left = nullptr, *right = nullptr;
        ListCell *list = nullptr;

        Node(Node *l, Node *r);

        Node(E *e);

        ~Node();
    };

    struct Tree {
        Node *root = nullptr;
        Tree *prev = nullptr, *next = nullptr, *sufmin = nullptr;
        int rank;

        Tree(E *e);

        ~Tree();
    };

    soft_heap();

    explicit soft_heap(const Compare &comp_, double epsilon_ = 0.0000001)
        : comp(comp_) {
        this->epsilon = 0.000001;
        this->rank = 0;
        this->max_node_rank = std::ceil(log2(1. / this->epsilon)) + 5;
        this->first = nullptr;
    }

    soft_heap(const Compare &comp_, E *e, double epsilon_ = 0.0000001)
        : comp(comp_) {
        this->epsilon = epsilon_;
        this->rank = 0;
        this->max_node_rank = std::ceil(log2(1. / this->epsilon)) + 5;
        this->first = new Tree(e);
    }

    soft_heap(double epsi);

    ~soft_heap();

    [[nodiscard]] bool empty() const {
        return (size_ == 0);
    }

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    void insert(const E &e) {
        ++size_;
        E *e_copy = new E(e);
        if (this->first == nullptr) {
            this->first = new Tree(e_copy);
        } else {
            soft_heap<E, Compare> *q =
                new soft_heap<E, Compare>(comp, e_copy, epsilon);
            q->max_node_rank = this->max_node_rank;
            q->epsilon = this->epsilon;
            meld(q);
            delete q;
        }
    }

    void meld(soft_heap *Q);

    void extractMin() {
        extr();
    }

    double epsilon = 0.000001;
    Tree *first = nullptr;
    int max_node_rank;
    int rank;

    const E getMin() {
        E e_(extr());
        insert(e_);
        return e_;
    }

private:
    const E &extr() {
        --size_;
        int *deleted = new int(2);
        E *e = nullptr;
        while (*deleted == DELETED) {
            if (this->first == nullptr) {
                delete deleted;
            }

            Tree *t = this->first->sufmin;
            Node *x = t->root;
            e = pick_elem(t, deleted);

            if (x->num <= x->size / 2) {
                if (!leaf(x)) {
                    sift(x);
                    update_suffix_min(t);
                } else if (x->num == 0) {
                    delete x;
                    remove_tree(this, t);

                    if (t->next == nullptr) {
                        if (t->prev == nullptr)
                            this->rank = -1;
                        else
                            this->rank = t->prev->rank;
                    }

                    if (t->prev != NULL)
                        update_suffix_min(t->prev);
                    t->prev = nullptr;
                    t->next = nullptr;
                    t->root = nullptr;
                    delete t;
                }
            }
        }
        delete deleted;
        return *e;
    }

    void sift(Node *x);

    Node *combine(Node *x, Node *y);

    void merge_into(soft_heap *q);

    void repeated_combine(soft_heap *q, int rk);

    void update_suffix_min(Tree *t);

    void insert_tree(soft_heap *q, Tree *t1, Tree *t2);

    void remove_tree(soft_heap *q, Tree *t);

    bool leaf(Node *x);

    void concatenate(Node *n1, Node *n2);

    E *pick_elem(Tree *t, int *deleted);

    void swapLR(Node *x);

    void thisSwap(soft_heap *Q);

    int listSize(Node *x);
};

template <typename E, typename Compare>
soft_heap<E, Compare>::ListCell::ListCell(E *e) : elem(e), del(1) {
}

template <typename E, typename Compare>
soft_heap<E, Compare>::Node::Node(soft_heap<E, Compare>::Node *l,
                                  soft_heap<E, Compare>::Node *r)
    : rank(l->rank + 1), num(size), ckey(0), left(l), right(r) {
}

template <typename E, typename Compare>
soft_heap<E, Compare>::Node::Node(E *e) : rank(0), size(1), num(size), ckey(e) {
    this->list = new ListCell(e);
}

template <typename E, typename Compare>
soft_heap<E, Compare>::Tree::Tree(E *e) : rank(0) {
    this->root = new Node(e);
    this->sufmin = this;
}

template <typename E, typename Compare>
soft_heap<E, Compare>::soft_heap() : epsilon(default_epsilon), rank(0)  {
//    this->epsilon = 0.000001;
//    this->rank = 0;
    this->max_node_rank = std::ceil(log2(1. / this->epsilon)) + 5;
//    this->first = nullptr
}

template <typename E, typename Compare>
soft_heap<E, Compare>::soft_heap(double epsi) {
    this->epsilon = epsi;
    this->rank = 0;
    this->max_node_rank = std::ceil(log2(1. / this->epsilon)) + 5;
    this->first = nullptr;
}

/*DESTRUCTORS*/

template <typename E, typename Compare>
soft_heap<E, Compare>::~soft_heap() {
    if (this->first != nullptr) {
        delete this->first;
        this->first = nullptr;
    }
}

template <typename E, typename Compare>
soft_heap<E, Compare>::ListCell::~ListCell() {
    if (this->next != nullptr) {
        delete this->next;
        this->next = nullptr;
    }
}

template <typename E, typename Compare>
soft_heap<E, Compare>::Node::~Node() {
    if (this->list != nullptr) {
        delete this->list;
        this->list = nullptr;
    }
    if (this->left != nullptr) {
        delete this->left;
        this->left = nullptr;
    }
    if (this->right != nullptr) {
        delete this->right;
        this->right = nullptr;
    }
}

template <typename E, typename Compare>
soft_heap<E, Compare>::Tree::~Tree() {
    if (this->root != nullptr) {
        delete this->root;
        this->root = nullptr;
    }
    if (this->prev != nullptr) {
        this->prev->next = nullptr;
        delete this->prev;
        this->prev = nullptr;
    }
    if (this->next != nullptr) {
        this->next->prev = nullptr;
        delete this->next;
        this->next = nullptr;
    }
}

template <typename E, typename Compare>
void soft_heap<E, Compare>::swapLR(Node *x) {
    Node *tmp = x->left;
    x->left = x->right;
    x->right = tmp;
}

template <typename E, typename Compare>
void soft_heap<E, Compare>::thisSwap(soft_heap *Q) {
    Tree *tQ = Q->first;
    int rkQ = Q->rank;

    Q->first = this->first;
    Q->rank = this->rank;

    this->first = tQ;
    this->rank = rkQ;
}

template <typename E, typename Compare>
void soft_heap<E, Compare>::meld(soft_heap *Q) {
    if (this->rank > Q->rank)
        thisSwap(Q);

    merge_into(Q);
    // SEEMS ALWAYS GOOD AFTER MERGE_INTO
    // The tricky one seems to be repeated_combine
    repeated_combine(Q, this->rank);

    thisSwap(Q);
    Q->first = nullptr;
}

template <typename E, typename Compare>
E *soft_heap<E, Compare>::pick_elem(Tree *t, int *deleted) {
    soft_heap<E, Compare>::ListCell *actual = t->root->list;
    E *act_elem = actual->elem;
    *deleted = actual->del;
    if (actual->next == nullptr) {
        delete t->root->list;
        t->root->list = nullptr;
        sift(t->root);
    }
    /*First elt of the list size > 1*/
    else {
        t->root->list = actual->next;
        actual->next = nullptr;
        delete actual;
    }
    t->root->num--;
    return act_elem;
}

template <typename E, typename Compare>
int soft_heap<E, Compare>::listSize(Node *x) {
    if (x->list == nullptr)
        return 0;
    ListCell *list = x->list;
    int cpt = 1;
    while (list->next != nullptr) {
        list = list->next;
        cpt++;
    }
    return cpt;
}

template <typename E, typename Compare>
void soft_heap<E, Compare>::concatenate(Node *n1, Node *n2) {
    if (n1->list == nullptr && n2->list == nullptr)
        return;
    else if (n1->list == nullptr)
        n1->list = n2->list;
    else if (n2->list == nullptr)
        return;
    else {
        ListCell *l = n1->list;
        while (l->next != nullptr)
            l = l->next;
        l->next = n2->list;
    }
    n1->num += n2->num;
    n2->num = 0;
    n2->list = nullptr;
}

template <typename E, typename Compare>
void soft_heap<E, Compare>::sift(Node *x) {
    while (x->num < x->size && !leaf(x)) {
        if (x->left == nullptr ||
            (x->right != nullptr && comp(*x->right->ckey, *x->left->ckey)))
            swapLR(x);

        concatenate(x, x->left);
        x->ckey = x->left->ckey;

        if (leaf(x->left)) {
            delete x->left;
            x->left = nullptr;
        } else
            sift(x->left);
    }
}

/************EPSYLON USE HERE max node rank**********/

template <typename E, typename Compare>
typename soft_heap<E, Compare>::Node *soft_heap<E, Compare>::combine(Node *x,
                                                                     Node *y) {
    Node *z = new Node(x, y);
    if (z->rank <= this->max_node_rank)
        z->size = 1;
    else
        z->size = (3 * x->size + 1) / 2;
    sift(z);
    return z;
}

template <typename E, typename Compare>
bool soft_heap<E, Compare>::leaf(Node *x) {
    return x->left == nullptr && x->right == nullptr;
}

template <typename E, typename Compare>
void soft_heap<E, Compare>::merge_into(soft_heap *q) {
    if (this->rank > q->rank)
        return;

    Tree *t1 = this->first;
    Tree *t2 = q->first;

    while (t1 != nullptr) {
        while (t1->rank > t2->rank)
            t2 = t2->next;
        Tree *t1bis = t1->next;
        insert_tree(q, t1, t2);
        t1 = t1bis;
    }
}

template <typename E, typename Compare>
void soft_heap<E, Compare>::insert_tree(soft_heap *q, Tree *t1, Tree *t2) {
    t1->next = t2;
    if (t2->prev == nullptr) {
        q->first = t1;
        this->first = nullptr;
    } else
        t2->prev->next = t1;
    t2->prev = t1;
}

template <typename E, typename Compare>
void soft_heap<E, Compare>::update_suffix_min(Tree *t) {
    while (t != nullptr) {
        if (t->next == nullptr ||
            comp(*t->root->ckey, *t->next->sufmin->root->ckey))
            t->sufmin = t;
        else
            t->sufmin = t->next->sufmin;
        t = t->prev;
    }
}

template <typename E, typename Compare>
void soft_heap<E, Compare>::remove_tree(soft_heap *q, Tree *t) {
    if (t->prev == nullptr)
        q->first = t->next;
    else
        t->prev->next = t->next;
    if (t->next != nullptr)
        t->next->prev = t->prev;
}

template <typename E, typename Compare>
void soft_heap<E, Compare>::repeated_combine(soft_heap *q, int rk) {
    Tree *t = q->first;
    while (t->next != nullptr) {
        if (t->next->rank == t->rank &&
            (t->next->next == nullptr || t->rank != t->next->next->rank)) {
            t->root = combine(t->root, t->next->root);
            t->rank = t->root->rank;

            /*Want to delete just the tree object and not what is inside*/
            Tree *to_delete = t->next;
            remove_tree(q, t->next);
            to_delete->prev = nullptr;
            to_delete->next = nullptr;
            to_delete->root = nullptr;
            delete to_delete;

        } else if (t->rank > rk)
            break;
        else
            t = t->next;
    }

    if (t->rank > q->rank)
        q->rank = t->rank;
    update_suffix_min(t);
}