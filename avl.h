#include <iostream>
#include <string>
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <memory>



template <typename T,typename A = std::allocator<T> >
class tree {
public:
    typedef A allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::reference reference;
    typedef typename A::const_reference const_reference;
    typedef typename A::difference_type difference_type;
    typedef typename A::size_type size_type;

private:
    class node {
    public:
	T data;
	short depth;
	size_type n;//number of nodes bellow 
	node *parent;
	node *left;
	node *right;

	node()
	    : depth(1)
	    , n(1)
	    , left(nullptr)
	    , right(nullptr)
	    , parent(nullptr) {}

	node(const T& t) 
	    : data(t)
	    , depth(1)
	    , n(1)
	    , left(nullptr)
	    , right(nullptr)
	    , parent(nullptr) {}

	node(T&& t) 
	    : data(std::move(t))
	    , depth(1)
	    , n(1)
	    , left(nullptr)
	    , right(nullptr)
	    , parent(nullptr) {}



	void update_depth() {
	    depth = 1 + std::max(left ? left->depth : 0,
				    right ? right->depth : 0);
	}

	void update_n() {
	    n = 1 + (left ? left->n : 0)
		+ (right ? right->n : 0);
	}

	short imbalance() {
	    return (right ? right->depth : 0)
		    - (left ? left->depth : 0);
	}
    };

public:
    class iterator {

	
	friend class tree;
    public:
	typedef typename A::difference_type difference_type;
	typedef typename A::value_type value_type;
	typedef typename A::reference reference;
	typedef typename A::pointer pointer;
	typedef std::random_access_iterator_tag iterator_category;

	iterator() {
	    ptr = nullptr;
	}

	iterator(node* p) {
	    ptr = p;
	}

	iterator(const iterator& it) {
	    ptr = it.ptr;
	}

	

	
	bool operator!=(const iterator& it) const {
	    return ptr != it.ptr;
	}
	bool operator==(const iterator& it) const {
	    return ptr == it.ptr;
	}


	bool operator>(const iterator& it) const {
	    return **this > *it;
	}

	

	bool operator>=(const iterator& it) const {
	    return **this >= *it;
	}

	bool operator<=(const iterator& it) const {
	    return **this <= *it;
	}

	iterator& operator=(const iterator& it) {
	    ptr = it.ptr;
	    return *this;
	}
	
	bool operator<(const iterator& it) const {
	    return **this < *it;
	}

	//Preorder traversal
	iterator& operator++() {
	    if (ptr->right) {
		ptr = ptr->right;
		while (ptr->left) {
		    ptr = ptr->left;
		}
	    } else {
		node* before;
		do {
		    before = ptr;
		    ptr = ptr->parent;
		} while (ptr && before == ptr->right);
	    }
	    return *this;
	}

	
	iterator operator++(int) {
	    iterator old(*this);
	    ++(*this);
	    return old;
	}

	
	iterator& operator--() {
	    if (ptr->left) {
		ptr = ptr->left;
		while (ptr->right) {
		    ptr = ptr->right;
		}
	    } else {
		node* before;
		do {
		    before = ptr;
		    ptr = ptr->parent;
		} while (ptr && before == ptr->left);
	    }
	    return *this;
	}

	
	iterator operator--(int) {
	    iterator old(*this);
	    --(*this);
	    return old;
	}

	reference operator*() const {
	    return ptr->data;
	}

	pointer operator->() const {
	    return &(ptr->data);
	}
    private:
	node *ptr;
    };


    tree()  {
	root = alloc.allocate(1);
	alloc.construct(root);
	root->n = 0;
    }

    tree(const tree& t)  {
	*this = t;
    }

    tree(tree&& t)  {
	root = t.root;
	t.root = alloc.allocate(1);
	alloc.construct(t.root);
	t.root->n = 0;
    }

    ~tree()  {
	clear_n(root);
	alloc.destroy(root);
	alloc.deallocate(root, 1);
    }

    tree& operator=(const tree& t)  {
	root = deepcopy(t.root);
	return *this;
    }

    tree& operator=(tree&& t)  {
	clear_n();
	std::swap(root, t.root);
	
    }

    bool operator==( tree& t)  {
	iterator it1, it2;
	for (it1 = this->begin(), it2 = t.begin();
		it1 != this->end() && it2 != t.end();
		++it1, ++it2) {
	    if (*it1 != *it2)
		return false;
	}
	if (it1 == this->end() && it2 == t.end()) {
	    return true;
	} else {
	    return false;
	}
    }

    bool operator!=(const tree& t) const {
	return !(*this == t);
    }

    iterator begin() {
	node *ptr = root;
	while (ptr->left) {
	    ptr = ptr->left;
	}
	return iterator(ptr);
    }



  
    iterator end() {
	node *ptr = root;
	while (ptr->right) {
	    ptr = ptr->right;
	}
	return iterator(ptr);
    }



   

    iterator insert(const T& t) {
	iterator r;


	node *parent = root;
	while (true) {
	    ++parent->n;
	    if (parent == root || t < parent->data) {
		if (parent->left) {
		    parent = parent->left;
		} else {
		    parent->left = alloc.allocate(1);//allocates memory
		    alloc.construct(parent->left, t);//constructs element object
		    parent->left->parent = parent;
		    r = iterator(parent->left);
		    break;
		}
	    } else {
		if (parent->right) {
		    parent = parent->right;
		} else {
		    parent->right = alloc.allocate(1);
		    alloc.construct(parent->right, t);
		    parent->right->parent = parent;
		    r = iterator(parent->right);
		    break;
		}
	    }
	}

	do {
	    
	    if (parent->imbalance() < -1) { 
		//left right rotation
		if (parent->left->imbalance() > 0) {
		    rotate_left(parent->left);
		}
		//left left rotation
		rotate_right(parent);
		break;
	    } 
	
	else if (parent->imbalance() > 1) {
		//right left rotation
		if (parent->right->imbalance() < 0) {
		    rotate_right(parent->right);
		}
		//right right rotation
		rotate_left(parent);
		break;
	    }
	
	    parent = parent->parent;
	} while(parent);
	return r;
    }

 
    

   iterator erase(iterator it) {
	iterator itn(it);
	++itn;
	node *ptr = it.ptr;
	node *q;
	//BST deletion
	if (!ptr->left || !ptr->right) {
	    q = ptr;
	} else {
	    q = itn.ptr;
	}
	node *s;
	if (q->left) {
	    s = q->left;
	    q->left = nullptr;
	} else {
	    s = q->right;
	    q->right = nullptr;
	}
	if (s) {
	    s->parent = q->parent;
	}
	if (q == q->parent->left) {
	    q->parent->left = s;
	} else {
	    q->parent->right = s;
	}
	node *q_parent = q->parent;
	if (q != ptr) {
	    q->parent = ptr->parent;
	    if (q->parent->left == ptr) {
		q->parent->left = q;
	    } else {
		q->parent->right = q;
	    }
	    q->left = ptr->left;
	    if (q->left)
		q->left->parent = q;
	    q->right = ptr->right;
	    if (q->right)
		q->right->parent = q;
	    q->n = ptr->n;
	    q->depth = ptr->depth;
	    ptr->left = nullptr;
	    ptr->right = nullptr;
	}
	if (q_parent == ptr) {
	    q_parent = q;
	}
	node *parent;
	for (parent = q_parent; parent; parent = parent->parent) {
	    --parent->n;
	}
	for (parent = q_parent; parent; parent = parent->parent) {
	    parent->update_depth();
	    if (parent == root)
		break;
	    if (parent->imbalance() < -1) {
		if (parent->left->imbalance() > 0) {
		    rotate_left(parent->left);
		}
		rotate_right(parent);
		break;
	    } else if (parent->imbalance() > 1) {
		if (parent->right->imbalance() < 0) {
		    rotate_right(parent->right);
		}
		rotate_left(parent);
		break;
	    }
	}
	alloc.destroy(ptr);
	alloc.deallocate(ptr, 1);
	return itn;
    }

    iterator find(const_reference t) {
	node *ptr = root->left;
	while (ptr) {
	    if (t == ptr->data) {
		return iterator(ptr);
	    } else if (t < ptr->data) {
		ptr = ptr->left;
	    } else {
		ptr = ptr->right;
	    }
	}
	return end();
    }

    int remove(const_reference t) {
	int y=0;
	iterator i = find(t);
	if (i == end())
	{
	  
	    return y;
	}
	
	do {
	    i = erase(i);
		y=1;
	} while(*i == t);
	return y;
    }

    void clear_n()  {
	clear_n(root);
	root->left = nullptr;
	root->n = 0;
	root->depth = 1;
    }

    void swap(tree& t) {
	std::swap(root, t.root);
    }

    size_type size() const {
	return root->n;
    }

   

    bool empty() const {
	return root->left == nullptr;
    }

    A get_allocator() {
	return alloc;
    }

private:
    void rotate_left(node *n) {
	node *tmp = n->right->left;
	if (n == n->parent->left) {
	    n->parent->left = n->right;
	} else {
	    n->parent->right = n->right;
	}
	n->right->parent = n->parent;
	n->right->left = n;
	n->parent = n->right;
	n->right = tmp;
	if (tmp)
	    tmp->parent = n;

	
	n->update_n();
	n->parent->update_n();

	
	do {
	    n->update_depth();
	    n = n->parent;
	} while (n);
    }

    void rotate_right(node *n) {
	node *tmp = n->left->right;
	if (n == n->parent->left) {
	    n->parent->left = n->left;
	} else {
	    n->parent->right = n->left;
	}
	n->left->parent = n->parent;
	n->left->right = n;
	n->parent = n->left;
	n->left = tmp;
	if (tmp)
	    tmp->parent = n;

	
	n->update_n();
	n->parent->update_n();

	
	do {
	    n->update_depth();
	    n = n->parent;
	} while (n);
    }

    node* deepcopy(const node *e) {
	node *cp = alloc.allocate(1);
	alloc.construct(cp, e->data);
	cp->n = e->n;
	cp->depth = e->depth;
	if (e->left) {
	    cp->left = deepcopy(e->left);
	    cp->left->parent = cp;
	}
	if (e->right) {
	    cp->right = deepcopy(e->right);
	    cp->right->parent = cp;
	}
	return cp;
    }

    void clear_n(node *e)  {
	if (e->left) {
	    clear_n(e->left);
	    alloc.destroy(e->left);
	    alloc.deallocate(e->left, 1);
	}
	if (e->right) {
	    clear_n(e->right);
	    alloc.destroy(e->right);
	    alloc.deallocate(e->right, 1);
	}
    }

    using All = typename std::allocator_traits<A>::template rebind_alloc<node>;
    All alloc;
    node *root;

   
};


