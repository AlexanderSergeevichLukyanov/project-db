#include <cassert>
#include <cmath>
#include <functional>
#include <type_traits>
#include <utility>

#define ABORTED_DEL 0
#define NOT_DELETED 1
#define DELETED 2


//#define soft_wrong_test //не стоит расскоментировать:)
#define soft_construct //default constructor
#define soft_insert  // insert(T x)
#define soft_get_min // getMin()
//#define soft_make // make(T* array, size_t n)
#define soft_extract_min // extractMin()
#define soft_solyanka // ... , check const, reference, voids methods and
// constructors =&& && #define soft_construct_comp //default constructor(comp)
#define soft_insert_comp // insert(T x) with Compare
#define soft_get_min_comp // getMin() with Compare
//#define soft_make_comp // make(T* array, size_t n) with Compare
#define soft_extract_min_comp // extractMin() with Compare
#define soft_solyanka_comp // ... , check const, reference, voids methods and
// constructors =&& && with Compare #define soft_heap_ //расскоментировать,
// когда куча будет готова

template <typename E, typename Compare=std::less<E>>
struct soft_heap {
	Compare comp;
	std::size_t size_=0;
    struct ListCell {
        E *elem;
        int del;
        ListCell *next;

        ListCell(E *e);

        ~ListCell();
    };

    struct Node {
        int rank, size, num;
        E *ckey;
        Node *left, *right;
        ListCell *list;

        Node(Node *l, Node *r);

        Node(E *e);

        ~Node();
    };

    struct Tree {
        Node *root;
        Tree *prev, *next, *sufmin;
        int rank;

        Tree(E *e);

        ~Tree();
    };

    soft_heap();

    soft_heap(E *e);
	
	explicit soft_heap(const Compare &comp_): comp(comp_){
		this->epsilon = 0.000001;
    this->rank = 0;
    this->max_node_rank = std::ceil(log2(1. / this->epsilon)) + 5;
    this->first = nullptr;
	}
	
	soft_heap(const Compare &comp_, E *e): comp(comp_) {
    this->epsilon = 0.000001;
    this->rank = 0;
    this->max_node_rank = std::ceil(log2(1. / this->epsilon)) + 5;
    this->first = new Tree(e);
}
	
    soft_heap(double epsi);

    ~soft_heap();

	[[nodiscard]] bool empty() const {
		return (size_==0);
	}
	
	[[nodiscard]] std::size_t size() const{
		return size_;
	}

    void insert(const E &e){
		++size_;
		E *e_copy = new E(e);
		if (this->first == nullptr) {
			this->first = new Tree(e_copy);
		}
		else{
			soft_heap <E, Compare> *q = new soft_heap<E, Compare>(comp, e_copy);
			q->max_node_rank = this->max_node_rank;
			q->epsilon = this->epsilon;
			meld(q);
			delete q;
		}
	}
	
    void meld(soft_heap *Q);

    void extractMin() {
		extr();
    //return e;
	}
	
    double epsilon = 0.000001;
    Tree *first;
    int max_node_rank;
    int rank;
	
	const E getMin(){
		E e_(extr());
		insert(e_);
		return e_;
	}		

    private:
	
	const E &extr(){
		--size_;
		int *deleted = new int(2);//DELETED = 2
		E *e = nullptr;
		while (*deleted == DELETED) {
			if (this->first == nullptr) {
				delete deleted;
	//            return std::nullopt;
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

    int searchAndDestroy(Node *parent, Node *child, E *e, bool force_delete = false);

    bool searchAndDestroyFake(Node *child, E *e);

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

template<typename E, typename Compare>
soft_heap<E, Compare>::ListCell::ListCell(E *e) {
    this->elem = e;
    this->del = 1; //NOT_DELETED = 1
    this->next = nullptr;
}

template<typename E, typename Compare>
soft_heap<E, Compare>::Node::Node(soft_heap<E, Compare>::Node *l, soft_heap<E, Compare>::Node *r) {
    this->list = nullptr;
    this->rank = l->rank + 1;
    this->size = 0;
    this->num = this->size;
    this->ckey = 0;
    this->left = l;
    this->right = r;
}

template<typename E, typename Compare>
soft_heap<E, Compare>::Node::Node(E *e) {
    this->list = new ListCell(e);
    this->rank = 0;
    this->size = 1;
    this->num = this->size;
    this->ckey = e;
    this->left = nullptr;
    this->right = nullptr;
}

template<typename E, typename Compare>
soft_heap<E, Compare>::Tree::Tree(E *e) {
    this->root = new Node(e);
    this->rank = 0;
    this->prev = nullptr;
    this->next = nullptr;
    this->sufmin = this;
}

template<typename E, typename Compare>
soft_heap<E, Compare>::soft_heap(E *e) {
    this->epsilon = 0.000001;
    this->rank = 0;
    this->max_node_rank = std::ceil(log2(1. / this->epsilon)) + 5;
    this->first = new Tree(e);
}

template<typename E, typename Compare>
soft_heap<E, Compare>::soft_heap() {
    this->epsilon = 0.000001;
    this->rank = 0;
    this->max_node_rank = std::ceil(log2(1. / this->epsilon)) + 5;
    this->first = nullptr;
}

template<typename E, typename Compare>
soft_heap<E, Compare>::soft_heap(double epsi) {
    this->epsilon = epsi;
    this->rank = 0;
    this->max_node_rank = std::ceil(log2(1. / this->epsilon)) + 5;
    this->first = nullptr;
}


/*DESTRUCTORS*/


template<typename E, typename Compare>
soft_heap<E, Compare>::~soft_heap() {
    if (this->first != nullptr) {
        delete this->first;
        this->first = nullptr;
    }
}

template<typename E, typename Compare>
soft_heap<E, Compare>::ListCell::~ListCell() {
    if (this->next != nullptr) {
        delete this->next;
        this->next = nullptr;
    }
}

template<typename E, typename Compare>
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

template<typename E, typename Compare>
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


template<typename E, typename Compare>
void soft_heap<E, Compare>::swapLR(Node *x) {
    Node *tmp = x->left;
    x->left = x->right;
    x->right = tmp;
}


template<typename E, typename Compare>
void soft_heap<E, Compare>::thisSwap(soft_heap *Q) {
    Tree *tQ = Q->first;
    int rkQ = Q->rank;

    Q->first = this->first;
    Q->rank = this->rank;

    this->first = tQ;
    this->rank = rkQ;
}


template<typename E, typename Compare>
void soft_heap<E, Compare>::meld(soft_heap *Q) {
    if (this->rank > Q->rank)
        thisSwap(Q);

    merge_into(Q);
    //SEEMS ALWAYS GOOD AFTER MERGE_INTO
    //The tricky one seems to be repeated_combine
    repeated_combine(Q, this->rank);

    thisSwap(Q);
    Q->first = nullptr;
}


template<typename E, typename Compare>
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

template<typename E, typename Compare>
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


template<typename E, typename Compare>
void soft_heap<E, Compare>::concatenate(Node *n1, Node *n2) {
    if (n1->list == nullptr and n2->list == nullptr)
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


template<typename E, typename Compare>
void soft_heap<E, Compare>::sift(Node *x) {
    while (x->num < x->size && !leaf(x)) {
        if (x->left == nullptr || (x->right != nullptr && comp(*x->right->ckey, *x->left->ckey)))
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

template<typename E, typename Compare>
typename soft_heap<E, Compare>::Node *soft_heap<E, Compare>::combine(Node *x, Node *y) {
    Node *z = new Node(x, y);
    if (z->rank <= this->max_node_rank)
        z->size = 1;
    else
        z->size = (3 * x->size + 1) / 2;
    sift(z);
    return z;
}


template<typename E, typename Compare>
bool soft_heap<E, Compare>::leaf(Node *x) {
    return x->left == nullptr && x->right == nullptr;
}


template<typename E, typename Compare>
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


template<typename E, typename Compare>
void soft_heap<E, Compare>::insert_tree(soft_heap *q, Tree *t1, Tree *t2) {
    t1->next = t2;
    if (t2->prev == nullptr) {
        q->first = t1;
        this->first = nullptr;
    } else
        t2->prev->next = t1;
    t2->prev = t1;

}


template<typename E, typename Compare>
void soft_heap<E, Compare>::update_suffix_min(Tree *t) {
    while (t != nullptr) {
        if (t->next == nullptr || comp(*t->root->ckey, *t->next->sufmin->root->ckey))
            t->sufmin = t;
        else
            t->sufmin = t->next->sufmin;
        t = t->prev;
    }
}


template<typename E, typename Compare>
void soft_heap<E, Compare>::remove_tree(soft_heap *q, Tree *t) {
    if (t->prev == nullptr)
        q->first = t->next;
    else
        t->prev->next = t->next;
    if (t->next != nullptr)
        t->next->prev = t->prev;
}


template<typename E, typename Compare>
void soft_heap<E, Compare>::repeated_combine(soft_heap *q, int rk) {
    Tree *t = q->first;
    while (t->next != nullptr) {
        if (t->next->rank == t->rank && (t->next->next == nullptr || t->rank != t->next->next->rank)) {
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


/*************DELETE*************/


/**
 * Lets assume that this function delete and stop at the first occurence of the e element
 * About the case when e is present not only one time in the heap
 * This function is the users delete and not the delete of the extract
 * @tparam E
 * @param parent
 * @param child
 * @param e
 * @return
 */
template<typename E, typename Compare>
int soft_heap<E, Compare>::searchAndDestroy(Node *parent, Node *child, E *e, bool force_delete) {

    ListCell *l = child->list;
    ListCell *prev = nullptr;
    int success = NOT_DELETED;

    while (l != nullptr) {
        if (*l->elem == *e) {
            if (l->del == 2 && !force_delete)
                return ABORTED_DEL;

            if (prev == nullptr && l->next == nullptr) {
                child->list = nullptr;
                child->num--;
                sift(child);
                if (parent->left == child && parent->left->list == nullptr) {
                    parent->left = nullptr;
                    delete child;
                } else if (parent->right == child && parent->right->list == nullptr) {
                    parent->right = nullptr;
                    delete child;
                }

            }
                /*First elt of the list size > 1*/
            else if (prev == nullptr) {
                child->list = l->next;
                l->next = nullptr;

            }
                /*Last element*/
            else if (l->next == nullptr)
                prev->next = nullptr;

                /*Element in the middle of the list*/
            else
                prev->next = l->next;
            delete l;
            return DELETED;
        }
        prev = l;
        l = l->next;
    }

    if (child->left != nullptr) success = searchAndDestroy(child, child->left, e, force_delete);
    if (success == NOT_DELETED && child->right != nullptr) success = searchAndDestroy(child, child->right, e, force_delete);
    return success;
}


/*******FAKE DELETE********/

template<typename E, typename Compare>
bool soft_heap<E, Compare>::searchAndDestroyFake(Node *child, E *e) {

    ListCell *l = child->list;
    bool success = false;

    while (l != nullptr) {
        if (*l->elem == *e) {
            l->del = 2; //DELETED = 2
            return true;
        }
        l = l->next;
    }

    if (child->left != nullptr) success = searchAndDestroyFake(child->left, e);
    if (!success && child->right != nullptr) success = searchAndDestroyFake(child->right, e);
    return success;
}