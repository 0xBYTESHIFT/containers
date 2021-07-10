#pragma once
/***
MIT License

Copyright (c) 2020 0xBYTESHIFT

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
***/
#include <cassert>
#include <deque>
#include <iterator>
#include <memory>
#include <queue>

namespace cont {

namespace tree_algo {
/**
 * Gives depth-distance between two iterators.
 * Counting from lhs to rhs, going up.
 * @param lhs iterator to compute depth-distance.
 * @param rhs target iterator.
 * @return depth-distance from lsh to rhs.
 *      Zero if iterators are not parent-related or rhs is down from lhs.
 *      Positive if rhs is up from lhs.
 */
template<class It, class Ret = typename It::difference_type>
static Ret depth_between(const It& lhs, const It& rhs);
/**
 * Gives breadth-distance between two iterators.
 * Counting from lhs to rhs, going right.
 * @param lhs iterator to compute breadth-distance.
 * @param rhs target iterator.
 * @return breadth-distance from lsh to rhs.
 *      Zero if iterators are not breadth-related or rsh is left from lhs.
 *      Positive if rsh is right from lhs.
 */
template<class It, class Ret = typename It::difference_type>
static Ret breadth_between(const It& lhs, const It& rhs);
/**
 * Checks if lhs is parent from rhs
 * Counting from lhs to rhs, going up.
 * @param lhs iterator to check parent-relation
 * @param rhs target iterator.
 * @return parent-relation. "true" if iterators are parent-related,
 *      "false" otherwise.
 */
template<class It>
static bool is_parent_to(const It& lhs, const It& rhs);
/**
 * Checks if lhs is left from rhs
 * Counting from lhs to rhs, going right.
 * @param lhs iterator to check left-relation
 * @param rhs target iterator.
 * @return left-relation. "true" if iterators are left-related,
 *      "false" otherwise.
 */
template<class It>
static bool is_left_to(const It& lhs, const It& rhs);
/**
 * Checks if lhs is right from rhs
 * Counting from lhs to rhs, going left.
 * @param lhs iterator to check right-relation
 * @param rhs target iterator.
 * @return right-relation. "true" if iterators are right-related,
 *      "false" otherwise.
 */
template<class It>
static bool is_right_to(const It& lhs, const It& rhs);
}; // namespace tree_algo

template<class T, class Allocator = std::allocator<T>>
class tree {
    Allocator p_alloc /**< allocator for values */;
    struct node;
    using nodeptr = node*;

    /**
     * Node struct for k_tree
     * Contains pointers to parent, left and right neighbours,
     * begin and end of children
     */
    struct node {
        nodeptr parent;      /**< Parent of a node */
        nodeptr left,        /**< Left neighbour of a node */
            right;           /**< Right neighbour of a node */
        nodeptr child_begin, /**< Pointer to childrens begin */
            child_end;       /**< Pointer to childrens end */
        T* value;            /**< Templated value of a node */
    };

public:
    /**
     * Iterator base class
     */
    class iterator_base {
    protected:
        friend class tree;
        /**
         * Protected constructor
         * @param n node for an iterator
         */
        iterator_base(nodeptr n);

    public:
        nodeptr n; /**< Node of an iterator */
        using allocator_t = Allocator;
        using self_type = iterator_base;
        using value_type = T;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using difference_type = size_t;
        using iterator_category = std::forward_iterator_tag;

        /**
         * Copy constructor
         * @param rhs rvalue of a copying
         */
        iterator_base(const iterator_base& rhs);
        /**
         * Dereference operator
         * @return reference of a node value
         */
        auto operator*() -> reference;
        /**
         * Const-dereference operator
         * @return const-reference of a node value
         */
        auto operator*() const -> const_reference;
        /**
         * Equal operator
         * @param rhs rvalue to compare to
         */
        auto operator==(const iterator_base& rhs) const -> bool;
        /**
         * Non-equal operator
         * @param rhs rvalue to compare to
         */
        auto operator!=(const iterator_base& rhs) const -> bool;
    };

    /**
     * Depth-first iterator class
     * First, it iterates through node's children recursively from left to right.
     * Second, it iterates through node's neighbours recursively from left to
     * right.
     */
    class df_iterator : public iterator_base {
    public:
        /**
         * Constructor
         * @param n node for an iterator
         */
        df_iterator(nodeptr n);
        /**
         * Copy Constructor
         * @param rhs rvalue of a copying
         */
        df_iterator(const iterator_base& rhs);
        /**
         * Prefix increment operator
         * @return reference to current iterator
         */
        auto operator++() -> df_iterator&;
        /**
         * Postfix increment operator
         * @return copy of current iterator
         */
        auto operator++(int) -> df_iterator;
        /**
         * Prefix decrement operator
         * @return reference to current iterator
         */
        auto operator--() -> df_iterator&;
        /**
         * Postfix decrement operator
         * @return copy of current iterator
         */
        auto operator--(int) -> df_iterator;
    };

    /**
     * Depth-first reverse iterator class
     * First, it iterates through node's children recursively from right to left.
     * Second, it iterates through node's neighbours recursively from right to
     * left.
     */
    class df_reverse_iterator : public df_iterator {
    protected:
    public:
        /**
         * Constructor
         * @param n node for an iterator
         */
        df_reverse_iterator(nodeptr n);
        /**
         * Copy Constructor
         * @param rhs rvalue of a copying
         */
        df_reverse_iterator(const iterator_base& rhs);
        /**
         * Prefix increment operator
         * @return reference to current iterator
         */
        auto operator++() -> df_reverse_iterator&;
        /**
         * Postfix increment operator
         * @return copy of current iterator
         */
        auto operator++(int) -> df_reverse_iterator;
        /**
         * Prefix decrement operator
         * @return reference to current iterator
         */
        auto operator--() -> df_reverse_iterator&;
        /**
         * Postfix decrement operator
         * @return copy of current iterator
         */
        auto operator--(int) -> df_reverse_iterator;
    };

    /**
     * Breadth-first iterator
     * Iterates through tree nodes from left to right.
     * Classic queue approach.
     */
    class bf_iterator : public iterator_base {
        nodeptr end;

    public:
        /**
         * Constructor
         * @param n node for an iterator
         */
        bf_iterator(nodeptr n);
        /**
         * Copy Constructor
         * @param rhs rvalue of a copying
         */
        bf_iterator(const iterator_base& rhs);
        /**
         * Prefix increment operator
         * @return reference to current iterator
         */
        auto operator++() -> bf_iterator&;
        /**
         * Postfix increment operator
         * @return copy of current iterator
         */
        auto operator++(int) -> bf_iterator;
        /**
         * Prefix decrement operator
         * @return reference to current iterator
         */
        auto operator--() -> bf_iterator& = delete; // not impl
        /**
         * Postfix decrement operator
         * @return copy of current iterator
         */
        auto operator--(int) -> bf_iterator = delete; // not impl
    private:
        std::queue<nodeptr> q;
    };

private:
    nodeptr root, /**< Begin of a tree, has value */
        foot;     /**< End of a tree, hasn't value */

    /**
     * Function that is used like a constructor
     * Initializes pointers to other nodes to nullptr
     * @param allocate determines whether memory should be allocated or not
     * @param args parameter pack for value
     * @return node pointer
     */
    template<class... Args>
    auto p_node_allocate(bool allocate, Args&&... args) -> nodeptr {
        nodeptr n = new node();
        n->parent = nullptr;
        n->left = n->right = nullptr;
        n->child_begin = n->child_end = nullptr;
        n->value = nullptr;
        if (allocate) {
            n->value = p_alloc.allocate(1);
            allocator_traits_t::construct(p_alloc, n->value, std::forward<Args>(args)...);
        }
        return n;
    }

    /**
     * Function that is used like a destructor
     * @param n pointer to a node to destruct
     */
    void p_node_deallocate(nodeptr n) {
        if (!n) {
            return;
        }
        if (n->value) {
            allocator_traits_t::destroy(p_alloc, n->value);
            p_alloc.deallocate(n->value, 1);
        }
        delete n;
    }

    void p_init() {
        root = p_node_allocate(false);
        foot = root;
    }

    void p_erase_children(nodeptr beg, nodeptr end) {
        for (auto n = beg; n != end;) {
            if (n->child_begin) {
                auto nbeg = n->child_begin;
                auto nend = n->child_end;
                p_erase_children(nbeg, nend);
            }
            auto bak = n->right;
            p_node_deallocate(n);
            n = bak; //increment
        }
        if (end && end->child_begin) {
            p_erase_children(end->child_begin, end->child_end);
        }
        p_node_deallocate(end);
    }

    void p_transfer(const tree<T, Allocator>& rhs) {
        if (rhs.empty()) {
            return;
        }
        auto it = df_iterator(rhs.end());
        it--;
        auto prev_dist = tree_algo::depth_between(it, rhs.begin());
        std::deque<std::pair<nodeptr, size_t>> queue;
        while (it != rhs.begin()) {
            auto tmp = p_node_allocate(true, *it.n->value);
            auto it_dist = tree_algo::depth_between(it, rhs.begin());
            if (it_dist < prev_dist) { // we moved up
                tmp->child_begin = queue.back().first;
                auto bak = tmp->child_begin;
                while (!queue.empty() && queue.back().second != it_dist) {
                    auto n = queue.back().first;
                    bak = n;
                    n->parent = tmp;
                    queue.pop_back();
                }
                tmp->child_end = bak;
                tmp->right = queue.back().first;
                queue.back().first->left = tmp;
            } else if (it_dist == prev_dist && !queue.empty()) {
                tmp->right = queue.back().first;
                queue.back().first->left = tmp;
            }
            queue.emplace_back(tmp, it_dist); // NOTE:reverse order
            prev_dist = it_dist;
            it--;
        }
        set_root<df_iterator>(*it.n->value);
        root->child_begin = queue.back().first;
        auto bak = root->child_begin;
        while (!queue.empty()) {
            auto n = queue.back().first;
            bak = n;
            n->parent = root;
            queue.pop_back();
        }
        root->child_end = bak;
    }

public:
    using allocator_t = Allocator;
    using allocator_traits_t = std::allocator_traits<Allocator>;
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = df_iterator;
    using const_iterator = const df_iterator;

    /**
     * Default constructor
     * @param args parameter pack for a root. May be empty
     * @param alloc allocator for values
     */
    template<class... Args>
    tree(Args&&... args, Allocator alloc = Allocator());

    /**
     * Constructor
     * @param alloc allocator for values
     * @param args parameter pack for a root. May be empty
     */
    template<class... Args>
    tree(const Allocator& alloc, Args&&... args);

    /**
     * Copy constructor, copies tree structire and values
     */
    tree(const tree<T, Allocator>& rhs);
    /**
     * Move constructor, moves entire tree
     * NOTE: move constructor is far more optimized
     */
    tree(tree<T, Allocator>&& rhs);
    /**
     * Destructor
     */
    ~tree();
    /**
     * Assign copy operator, clears current tree,
     * copies rhs structure and values
     */
    auto operator=(const tree<T, Allocator>& rhs) -> tree&;
    /**
     * Assign move operator, clears current tree,
     * copies rhs structure and values.
     * NOTE: move assigment is far more optimized
     */
    auto operator=(tree<T, Allocator>&& rhs) -> tree&;
    /**
     * Checks if tree is empty.
     * If root's address equals foot's address, return true.
     */
    auto empty() const -> bool;
    /**
     * Clears current tree, resets it's structure
     */
    void clear();
    /**
     * Erases given node and all it's children
     * @param it iterator to erase
     * @return next iterator of given iterator
     */
    template<class It>
    auto erase(const It& it) -> It;
    /**
     * Sets root value, copy or move
     * If tree is empty, inserts root node
     * If tree is not empty, replaces root node's value
     * @param args params for value to set root to, copy or move
     * @return iterator to root
     */
    template<class It, class... Args>
    auto set_root(Args&&... val) -> It;
    /**
     * @return iterator to root of a tree
     */
    template<class It = df_iterator>
    auto begin() const -> It;
    /**
     * @return iterator to foot of a tree
     */
    template<class It = df_iterator>
    auto end() const -> It;
    /**
     * @return size of a tree, difference of begin() and end()
     */
    auto size() const -> size_type;
    /**
     * Inserts value left from given iterator (left neighbour)
     * @param it iterator for relative left insert
     * @param args params for node's value type
     * @return iterator to new node. Same class as "it" param
     */
    template<class It, class... Args>
    auto insert_left(It& it, Args&&... args) -> It;
    /**
     * Inserts value right from given iterator (right neighbour)
     * @param it iterator for relative right insert
     * @param args params for node's value type
     * @return iterator to new node. Same class as "it" param
     */
    template<class It, class... Args>
    auto insert_right(It& it, Args&&... args) -> It;
    /**
     * Appends child with value to a given iterator (right-most child)
     * @param it iterator for child append
     * @param args params for node's value type
     * @return iterator to resulting child
     */
    template<class It, class... Args>
    auto append_child(It& it, Args&&... args) -> It;
    /**
     * Prepends child with value to a given iterator (left-most child)
     * @param it iterator for child prepend
     * @param args params for node's value type
     * @return iterator to resulting child, same class as "it" param
     */
    template<class It, class... Args>
    auto prepend_child(It& it, Args&&... args) -> It;
    /**
     * Equals operator
     * Checks if rhs structure and values are equeal to current tree.
     * @param rhs tree to check equality
     * @return Equality. "true" if trees are equal. "false" otherwise.
     */
    auto operator==(const tree<T, Allocator>& rhs) const -> bool;
    /**
     * Non-equals operator
     * Checks if rhs structure and values are not equeal to current tree.
     * @param rhs tree to check non-equality
     * @return Non-equality. "true" if trees are non-equal.
     *      "false" otherwise.
     */
    auto operator!=(const tree<T, Allocator>& rhs) const -> bool;
};

//*** iterator_base ***
template<class T, class Allocator>
tree<T, Allocator>::iterator_base::iterator_base(nodeptr n) {
    this->n = n;
}

template<class T, class Allocator>
tree<T, Allocator>::iterator_base::iterator_base(const iterator_base& rhs) {
    this->n = rhs.n;
    std::vector<int> v;
}

template<class T, class Allocator>
auto tree<T, Allocator>::iterator_base::operator*() -> reference {
    return *n->value;
}

template<class T, class Allocator>
auto tree<T, Allocator>::iterator_base::operator*() const -> const_reference {
    return *n->value;
}

template<class T, class Allocator>
auto tree<T, Allocator>::iterator_base::operator==(const iterator_base& rhs) const -> bool {
    return this->n == rhs.n;
}

template<class T, class Allocator>
auto tree<T, Allocator>::iterator_base::operator!=(const iterator_base& rhs) const -> bool {
    return this->n != rhs.n;
}

//*** df_iterator ***
template<class T, class Allocator>
tree<T, Allocator>::df_iterator::df_iterator(nodeptr n)
    : iterator_base(n) {}

template<class T, class Allocator>
tree<T, Allocator>::df_iterator::df_iterator(const iterator_base& rhs)
    : iterator_base(rhs) {}

template<class T, class Allocator>
auto tree<T, Allocator>::df_iterator::operator++() -> df_iterator& {
    if (this->n->child_begin) {
        this->n = this->n->child_begin;
    } else {
        while (!this->n->right) {
            this->n = this->n->parent;
            if (!this->n) {
                return *this;
            }
        }
        this->n = this->n->right;
    }
    return *this;
}

template<class T, class Allocator>
auto tree<T, Allocator>::df_iterator::operator--() -> df_iterator& {
    if (this->n->left) {
        this->n = this->n->left;
        while (this->n->child_end) {
            this->n = this->n->child_end;
        }
    } else {
        this->n = this->n->parent;
    }
    return *this;
}

template<class T, class Allocator>
auto tree<T, Allocator>::df_iterator::operator++(int) -> df_iterator {
    auto copy = *this;
    ++(*this);
    return copy;
}

template<class T, class Allocator>
auto tree<T, Allocator>::df_iterator::operator--(int) -> df_iterator {
    auto copy = *this;
    --(*this);
    return copy;
}

//*** df_reverse_iterator ***
template<class T, class Allocator>
tree<T, Allocator>::df_reverse_iterator::df_reverse_iterator(nodeptr n)
    : df_iterator(n) {}

template<class T, class Allocator>
tree<T, Allocator>::df_reverse_iterator::df_reverse_iterator(
    const iterator_base& rhs)
    : df_iterator(rhs) {}

template<class T, class Allocator>
auto tree<T, Allocator>::df_reverse_iterator::operator++() -> df_reverse_iterator& {
    return df_iterator::operator--();
}

template<class T, class Allocator>
auto tree<T, Allocator>::df_reverse_iterator::operator--() -> df_reverse_iterator& {
    return df_iterator::operator++();
}

template<class T, class Allocator>
auto tree<T, Allocator>::df_reverse_iterator::operator++(int) -> df_reverse_iterator {
    auto copy = *this;
    ++(*this);
    return copy;
}

template<class T, class Allocator>
auto tree<T, Allocator>::df_reverse_iterator::operator--(int) -> df_reverse_iterator {
    auto copy = *this;
    --(*this);
    return copy;
}

/*** bf_iterator ***/
template<class T, class Allocator>
tree<T, Allocator>::bf_iterator::bf_iterator(nodeptr n)
    : iterator_base(n) {
    q.emplace(n);
}

template<class T, class Allocator>
tree<T, Allocator>::bf_iterator::bf_iterator(
    const iterator_base& rhs)
    : iterator_base(rhs) {
    q.emplace(rhs.n);
}

template<class T, class Allocator>
auto tree<T, Allocator>::bf_iterator::operator++() -> bf_iterator& {
    if (this->n->right) {
        if (this->n->parent && this->n->right) { // it's not foot
            this->n = this->n->right;
            q.emplace(this->n);
            return *this;
        } else {
            this->end = this->n->right; // save foot
        }
    }
    // find first node with children in queue
    nodeptr top;
    do {
        top = q.front();
        q.pop();
    } while (!top->child_begin && !q.empty());
    if (!top->child_begin) {
        this->n = end;
    } else {
        this->n = top->child_begin;
        q.emplace(this->n);
    }
    return *this;
}

template<class T, class Allocator>
auto tree<T, Allocator>::bf_iterator::operator++(int) -> bf_iterator {
    auto copy = *this;
    ++(*this);
    return copy;
}

/*** tree ***/
template<class T, class Allocator>
template<class... Args>
tree<T, Allocator>::tree(Args&&... args, Allocator alloc)
    : tree(alloc, std::forward<Args>(args)...) {}

/*** tree ***/
template<class T, class Allocator>
template<class... Args>
tree<T, Allocator>::tree(const Allocator& alloc, Args&&... args)
    : p_alloc(alloc) {
    p_init();
    set_root<df_iterator>(std::forward<Args>(args)...);
}

template<class T, class Allocator>
tree<T, Allocator>::tree(const tree<T, Allocator>& rhs)
    : tree(rhs.p_alloc) {
    p_transfer(rhs);
}

template<class T, class Allocator>
tree<T, Allocator>::tree(tree<T, Allocator>&& rhs)
    : p_alloc(rhs.p_alloc) {
    this->root = rhs.root;
    this->foot = rhs.foot;
    rhs.root = nullptr;
    rhs.foot = nullptr;
}

template<class T, class Allocator>
tree<T, Allocator>::~tree() {
    p_erase_children(root, foot);
}

template<class T, class Allocator>
auto tree<T, Allocator>::operator=(const tree<T, Allocator>& rhs) -> tree<T, Allocator>& {
    clear();
    p_transfer(rhs);
    return *this;
}

template<class T, class Allocator>
auto tree<T, Allocator>::operator=(tree<T, Allocator>&& rhs) -> tree<T, Allocator>& {
    p_erase_children(root, foot);
    this->root = rhs.root;
    this->foot = rhs.foot;
    rhs.root = nullptr;
    rhs.foot = nullptr;
    return *this;
}

template<class T, class Allocator>
auto tree<T, Allocator>::empty() const -> bool {
    return this->root == this->foot;
}

template<class T, class Allocator>
void tree<T, Allocator>::clear() {
    if (root == foot) {
        return;
    }
    erase(df_iterator(root));
}

template<class T, class Allocator>
template<class It>
auto tree<T, Allocator>::erase(const It& it) -> It {
    assert(it.n != foot);
    if (it.n->child_begin) {
        p_erase_children(it.n->child_begin, it.n->child_end);
    }
    It bak = (it.n->right) ? It(it.n->right) : It(it.n->parent);
    if (it.n->left) {
        it.n->left->right = it.n->right;
    }
    if (it.n->right) {
        it.n->right->left = it.n->left;
    }
    if (it.n->parent) {
        if (it.n->parent->child_begin == it.n) {
            it.n->parent->child_begin = it.n->right;
        }
        if (it.n->parent->child_end == it.n) {
            it.n->parent->child_end = it.n->left;
        }
    }
    if (it.n == root) {
        root = foot;
    }
    p_node_deallocate(it.n);
    return bak;
}

template<class T, class Allocator>
template<class It, class... Args>
auto tree<T, Allocator>::set_root(Args&&... args) -> It {
    if (root == foot) { //if tree is empty, create foot
        foot = p_node_allocate(false);
        root->right = foot;
        foot->left = root;
    }
    if (this->root->value) {
        allocator_traits_t::destroy(p_alloc, this->root->value);
    } else {
        this->root->value = allocator_traits_t::allocate(p_alloc, 1);
    }
    allocator_traits_t::construct(p_alloc, this->root->value, std::forward<Args>(args)...);
    return It(this->root);
}

template<class T, class Allocator>
template<class It>
auto tree<T, Allocator>::begin() const -> It {
    return It(this->root);
}

template<class T, class Allocator>
template<class It>
auto tree<T, Allocator>::end() const -> It {
    return It(this->foot);
}

template<class T, class Allocator>
auto tree<T, Allocator>::size() const -> size_type {
    size_type result = 0;
    auto it = begin();
    while (it != end()) {
        it++;
        result++;
    }
    return result;
}

template<class T, class Allocator>
template<class It, class... Args>
auto tree<T, Allocator>::insert_left(It& it, Args&&... args) -> It {
    assert(it != begin());
    assert(it != end());
    auto tmp = p_node_allocate(true, std::forward<Args>(args)...);
    if (it.n->left) { //if lhs has left node, insert rhs between them
        tmp->left = it.n->left;
        it.n->left->right = tmp;
    } else if (it.n == it.n->parent->child_begin) { //if lhs is leftmost child
        it.n->parent->child_begin = tmp;
    }
    tmp->right = it.n;
    it.n->left = tmp;
    tmp->parent = it.n->parent;
    return It(tmp);
}

template<class T, class Allocator>
template<class It, class... Args>
auto tree<T, Allocator>::insert_right(It& it, Args&&... args) -> It {
    assert(it != begin());
    assert(it != end());
    auto tmp = p_node_allocate(true, std::forward<Args>(args)...);
    if (it.n->right) { //if lhs has right node, insert rhs between them
        tmp->right = it.n->right;
        it.n->right->left = tmp;
    } else if (it.n == it.n->parent->child_end) { //if lhs is rightmost child
        it.n->parent->child_end = tmp;
    }
    it.n->right = tmp;
    tmp->left = it.n;
    tmp->parent = it.n->parent;
    return It(tmp);
}

template<class T, class Allocator>
template<class It, class... Args>
auto tree<T, Allocator>::append_child(It& it, Args&&... args) -> It {
    assert(it != end());
    if (!it.n->child_end) { // iterator has no children
        return prepend_child(it, std::forward<Args>(args)...);
    }
    auto tmp = p_node_allocate(true, std::forward<Args>(args)...);
    tmp->parent = it.n;
    tmp->left = it.n->child_end;
    it.n->child_end->right = tmp;
    it.n->child_end = tmp;
    return It(tmp);
}

template<class T, class Allocator>
template<class It, class... Args>
auto tree<T, Allocator>::prepend_child(It& it, Args&&... args) -> It {
    assert(it != end());
    auto tmp = p_node_allocate(true, std::forward<Args>(args)...);
    tmp->parent = it.n;
    if (!it.n->child_begin) {
        it.n->child_begin = tmp;
        it.n->child_end = tmp;
    } else {
        it.n->child_begin->left = tmp;
        tmp->right = it.n->child_begin;
        it.n->child_begin = tmp;
    }
    return It(tmp);
}

template<class T, class Allocator>
auto tree<T, Allocator>::operator==(const tree<T, Allocator>& rhs) const -> bool {
    using namespace tree_algo;
    auto this_it = begin();
    auto this_it_bak = this_it;
    auto it = rhs.begin();
    auto it_bak = it;
    while (it != rhs.end()) {
        if (this_it == end()) {
            return false;
        }
        if (*it != *this_it) {
            return false;
        }
        if (is_parent_to(it, it_bak) != is_parent_to(this_it, this_it_bak) ||
            is_left_to(it, it_bak) != is_left_to(this_it, this_it_bak) ||
            is_right_to(it, it_bak) != is_right_to(this_it, this_it_bak)) {
            return false;
        }
        it_bak = it;
        this_it_bak = this_it;
        it++;
        this_it++;
    }
    if (this_it != end()) {
        return false;
    }
    return true;
}

template<class T, class Allocator>
auto tree<T, Allocator>::operator!=(const tree<T, Allocator>& rhs) const -> bool {
    return !(*this == rhs);
}

template<class It, class Ret>
auto tree_algo::depth_between(const It& lhs, const It& rhs) -> Ret {
    typename It::difference_type i = 0;
    auto tmp = lhs.n;
    while (tmp->parent) {
        tmp = tmp->parent;
        i++;
    }
    if (tmp != rhs.n)
        return 0;
    return i;
}

template<class It, class Ret>
auto tree_algo::breadth_between(const It& lhs, const It& rhs) -> Ret {
    typename It::difference_type i = 0;
    auto tmp = lhs.n;
    while (tmp->right) {
        tmp = tmp->right;
        i++;
    }
    if (tmp != rhs.n)
        return 0;
    return i;
}

template<class It>
auto tree_algo::is_parent_to(const It& lhs, const It& rhs) -> bool {
    return tree_algo::depth_between(lhs, rhs) != 0;
}

template<class It>
auto tree_algo::is_left_to(const It& lhs, const It& rhs) -> bool {
    return tree_algo::breadth_between(rhs, lhs) != 0;
}

template<class It>
auto tree_algo::is_right_to(const It& lhs, const It& rhs) -> bool {
    return tree_algo::breadth_between(lhs, rhs) != 0;
}
}; // namespace cont
