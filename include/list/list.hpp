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

template<class T, class Allocator = std::allocator<T>>
class list {
    Allocator p_alloc /**< allocator for values */;
    struct node;
    using nodeptr = node*;

    struct node {
        nodeptr left, /**< Left neighbour of a node */
            right;    /**< Right neighbour of a node */
        T* value;     /**< Templated value of a node */
    };

public:
    /**
     * Iterator base class
     */
    class iterator_base {
    protected:
        friend class list;
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
     * Forward iterator class
     */
    class iterator : public iterator_base {
    public:
        /**
         * Constructor
         * @param n node for an iterator
         */
        iterator(nodeptr n);
        /**
         * Copy Constructor
         * @param rhs rvalue of a copying
         */
        iterator(const iterator_base& rhs);
        /**
         * Prefix increment operator
         * @return reference to current iterator
         */
        auto operator++() -> iterator&;
        /**
         * Postfix increment operator
         * @return copy of current iterator
         */
        auto operator++(int) -> iterator;
        /**
         * Prefix decrement operator
         * @return reference to current iterator
         */
        auto operator--() -> iterator&;
        /**
         * Postfix decrement operator
         * @return copy of current iterator
         */
        auto operator--(int) -> iterator;
    };

    /**
     * Reverse iterator class
     */
    class reverse_iterator : public iterator_base {
    public:
        /**
         * Constructor
         * @param n node for an iterator
         */
        reverse_iterator(nodeptr n);
        /**
         * Copy Constructor
         * @param rhs rvalue of a copying
         */
        reverse_iterator(const iterator_base& rhs);
        /**
         * Prefix increment operator
         * @return reference to current iterator
         */
        auto operator++() -> reverse_iterator&;
        /**
         * Postfix increment operator
         * @return copy of current iterator
         */
        auto operator++(int) -> reverse_iterator;
        /**
         * Prefix decrement operator
         * @return reference to current iterator
         */
        auto operator--() -> reverse_iterator&;
        /**
         * Postfix decrement operator
         * @return copy of current iterator
         */
        auto operator--(int) -> reverse_iterator;
    };

private:
    nodeptr head, /**< Head of a list, has value */
        tail;     /**< Tail of a list, hasn't value */

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
        n->left = n->right = nullptr;
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
        head = p_node_allocate(false);
        tail = head;
    }

    void p_erase(nodeptr beg, nodeptr end) {
        if (!beg || !end) {
            return;
        }
        for (auto n = beg; n != end;) {
            auto bak = n->right;
            p_node_deallocate(n);
            n = bak; //increment
        }
    }

    void p_transfer(const list<T, Allocator>& rhs) {
        if (rhs.empty()) {
            return;
        }
        auto it = rhs.begin();
        while (it != rhs.end()) {
            insert_before(end(), *it.n->value);
            ++it;
        }
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

    /**
     * Default constructor
     * @param count count of nodes to pre-allocate
     * @param val the value to initialize elements of the container with
     * @param alloc allocator for values
     */
    list(size_t count = 0, const T& val = T(), const Allocator& alloc = Allocator());
    /**
     * Copy constructor, copies list structire and values
     */
    list(const list<T, Allocator>& rhs);
    /**
     * Move constructor, moves entire list
     * NOTE: move constructor is far more optimized
     */
    list(list<T, Allocator>&& rhs);
    /**
     * Destructor
     */
    ~list();
    /**
     * Assign copy operator, clears current list,
     * copies rhs structure and values
     */
    auto operator=(const list<T, Allocator>& rhs) -> list&;
    /**
     * Assign move operator, clears current list,
     * copies rhs structure and values.
     * NOTE: move assigment is far more optimized
     */
    auto operator=(list<T, Allocator>&& rhs) -> list&;
    /**
     * Checks if list is empty.
     * If head's address equals tail's address, return true.
     */
    auto empty() const -> bool;
    /**
     * Clears current list
     */
    void clear();
    /**
     * Erases node of given iterator
     * @param it iterator to erase
     * @return next iterator of given iterator
     */
    template<class It>
    auto erase(const It& it) -> It;
    /**
     * Erases nodes between given iterators
     * @param it0 begin of range
     * @param it1 end of range
     * @return next iterator of end 
     */
    template<class It>
    auto erase(const It& it0, const It& it1) -> It;
    /** 
     * Inserts value constructed with provided args after provided iterator
     * @param it iterator to insert value after
     * @param args parameter pack for construction of a value
     * @return iterator to inserted value
     * */
    template<class It, class... Args>
    auto insert(const It& it, Args&&... args) -> It;
    /**
     * Inserts value constructed with provided args before provided iterator
     * @param it iterator to insert value before
     * @param args parameter pack for construction of a value
     * @return iterator to inserted value
     */
    template<class It, class... Args>
    auto insert_before(const It& it, Args&&... args) -> It;
    /**
     * @return iterator to head of a list
     */
    template<class It = iterator>
    auto begin() const -> It;
    /**
     * @return iterator to tail of a list
     */
    template<class It = iterator>
    auto end() const -> It;
    /**
     * @return iterator to head of a list
     */
    template<class It = iterator>
    auto begin() -> It;
    /**
     * @return iterator to tail of a list
     */
    template<class It = iterator>
    auto end() -> It;
    /**
     * @return size of a list, difference of begin() and end()
     */
    auto size() const -> size_type;
    /**
     * Equals operator
     * Checks if rhs values are equeal to current list's.
     * @param rhs list to check equality
     * @return Equality. "true" if lists are equal. "false" otherwise.
     */
    auto operator==(const list<T, Allocator>& rhs) const -> bool;
    /**
     * Non-equals operator
     * Checks if rhs values are not equeal to current list's.
     * @param rhs list to check non-equality
     * @return Non-equality. "true" if lists are non-equal.
     *      "false" otherwise.
     */
    auto operator!=(const list<T, Allocator>& rhs) const -> bool;
};

//*** iterator_base ***
template<class T, class Allocator>
list<T, Allocator>::iterator_base::iterator_base(nodeptr n) {
    this->n = n;
}

template<class T, class Allocator>
list<T, Allocator>::iterator_base::iterator_base(const iterator_base& rhs) {
    this->n = rhs.n;
}

template<class T, class Allocator>
auto list<T, Allocator>::iterator_base::operator*() -> reference {
    return *n->value;
}

template<class T, class Allocator>
auto list<T, Allocator>::iterator_base::operator*() const -> const_reference {
    return *n->value;
}

template<class T, class Allocator>
auto list<T, Allocator>::iterator_base::operator==(const iterator_base& rhs) const -> bool {
    return this->n == rhs.n;
}

template<class T, class Allocator>
auto list<T, Allocator>::iterator_base::operator!=(const iterator_base& rhs) const -> bool {
    return this->n != rhs.n;
}

//*** iterator ***
template<class T, class Allocator>
list<T, Allocator>::iterator::iterator(nodeptr n)
    : iterator_base(n) {}

template<class T, class Allocator>
list<T, Allocator>::iterator::iterator(const iterator_base& rhs)
    : iterator_base(rhs) {}

template<class T, class Allocator>
auto list<T, Allocator>::iterator::operator++() -> iterator& {
    this->n = this->n->right;
    return *this;
}

template<class T, class Allocator>
auto list<T, Allocator>::iterator::operator--() -> iterator& {
    this->n = this->n->left;
    return *this;
}

template<class T, class Allocator>
auto list<T, Allocator>::iterator::operator++(int) -> iterator {
    auto copy = *this;
    ++(*this);
    return copy;
}

template<class T, class Allocator>
auto list<T, Allocator>::iterator::operator--(int) -> iterator {
    auto copy = *this;
    --(*this);
    return copy;
}

//*** reverse_iterator ***
template<class T, class Allocator>
list<T, Allocator>::reverse_iterator::reverse_iterator(nodeptr n)
    : iterator(n) {}

template<class T, class Allocator>
list<T, Allocator>::reverse_iterator::reverse_iterator(
    const iterator_base& rhs)
    : iterator(rhs) {}

template<class T, class Allocator>
auto list<T, Allocator>::reverse_iterator::operator++() -> reverse_iterator& {
    return iterator::operator--();
}

template<class T, class Allocator>
auto list<T, Allocator>::reverse_iterator::operator--() -> reverse_iterator& {
    return iterator::operator++();
}

template<class T, class Allocator>
auto list<T, Allocator>::reverse_iterator::operator++(int) -> reverse_iterator {
    auto copy = *this;
    ++(*this);
    return copy;
}

template<class T, class Allocator>
auto list<T, Allocator>::reverse_iterator::operator--(int) -> reverse_iterator {
    auto copy = *this;
    --(*this);
    return copy;
}

/*** list ***/
template<class T, class Allocator>
list<T, Allocator>::list(size_t count, const T& val, const Allocator& alloc)
    : p_alloc(alloc) {
    p_init();
    for (size_t i = 0; i < count; ++i) {
        insert_before(end(), val);
    }
}

template<class T, class Allocator>
list<T, Allocator>::list(const list<T, Allocator>& rhs)
    : p_alloc(rhs.p_alloc) {
    p_init();
    *this = rhs;
}

template<class T, class Allocator>
list<T, Allocator>::list(list<T, Allocator>&& rhs)
    : p_alloc(rhs.p_alloc) {
    this->head = rhs.head;
    this->tail = rhs.tail;
    rhs.head = nullptr;
    rhs.tail = nullptr;
}

template<class T, class Allocator>
list<T, Allocator>::~list() {
    clear();
    p_node_deallocate(tail);
}

template<class T, class Allocator>
auto list<T, Allocator>::operator=(const list<T, Allocator>& rhs) -> list<T, Allocator>& {
    clear();
    p_transfer(rhs);
    return *this;
}

template<class T, class Allocator>
auto list<T, Allocator>::operator=(list<T, Allocator>&& rhs) -> list<T, Allocator>& {
    p_erase(head, tail);
    this->head = rhs.head;
    this->tail = rhs.tail;
    rhs.head = nullptr;
    rhs.tail = nullptr;
    return *this;
}

template<class T, class Allocator>
auto list<T, Allocator>::empty() const -> bool {
    return this->head == this->tail;
}

template<class T, class Allocator>
void list<T, Allocator>::clear() {
    p_erase(head, tail);
    head = tail;
}

template<class T, class Allocator>
template<class It>
auto list<T, Allocator>::erase(const It& it) -> It {
    auto& n = it.n;
    if (it == end<It>()) {
        return end<It>();
    }
    It bak = It(n->right);
    if (it == begin<It>()) {
        this->head = n->right;
        n->right->left = nullptr;
    } else {
        n->right->left = n->left;
        n->left->right = n->right;
    }
    p_node_deallocate(n);
    return bak;
}

template<class T, class Allocator>
template<class It>
auto list<T, Allocator>::erase(const It& it0, const It& it1) -> It {
    if (it0 == end<It>()) {
        return end<It>();
    }
    auto it = it0;
    while (it != it1) {
        it = erase(it);
    }
    it = erase(it);
    return it;
}

template<class T, class Allocator>
template<class It, class... Args>
auto list<T, Allocator>::insert(const It& it, Args&&... args) -> It {
    auto tmp = p_node_allocate(true, std::forward<Args>(args)...);
    if (head == tail) { //if list is empty, replace head
        this->head = tmp;
        this->head->right = tail;
        this->tail->left = head;
    } else {
        assert(it != end());
        it.n->right->left = tmp;
        tmp->right = it.n->right;
        it.n->right = tmp;
        tmp->left = it.n;
    }
    return It(tmp);
}

template<class T, class Allocator>
template<class It, class... Args>
auto list<T, Allocator>::insert_before(const It& it, Args&&... args) -> It {
    nodeptr tmp = p_node_allocate(true, std::forward<Args>(args)...);
    if (head == tail) { //if list is empty, replace head
        this->head = tmp;
        this->head->right = tail;
        this->tail->left = head;
    } else {
        if (it == begin()) {
            tmp->right = it.n;
            it.n->left = tmp;
            head = tmp;
        } else {
            tmp->left = it.n->left;
            tmp->right = it.n;
            it.n->left->right = tmp;
            it.n->left = tmp;
        }
    }
    return It(tmp);
}

template<class T, class Allocator>
template<class It>
auto list<T, Allocator>::begin() const -> It {
    return It(this->head);
}

template<class T, class Allocator>
template<class It>
auto list<T, Allocator>::end() const -> It {
    return It(this->tail);
}

template<class T, class Allocator>
template<class It>
auto list<T, Allocator>::begin() -> It {
    return It(this->head);
}

template<class T, class Allocator>
template<class It>
auto list<T, Allocator>::end() -> It {
    return It(this->tail);
}

template<class T, class Allocator>
auto list<T, Allocator>::size() const -> size_type {
    size_type result = 0;
    auto it = begin();
    while (it != end()) {
        ++it;
        ++result;
    }
    return result;
}

template<class T, class Allocator>
auto list<T, Allocator>::operator==(const list<T, Allocator>& rhs) const -> bool {
    auto lhs_it = begin();
    auto rhs_it = rhs.begin();
    while (rhs_it != rhs.end()) {
        if (lhs_it == end()) {
            return false;
        }
        if (*rhs_it != *lhs_it) {
            return false;
        }
        ++lhs_it;
        ++rhs_it;
    }
    if (lhs_it != end()) {
        return false;
    }
    return true;
}

template<class T, class Allocator>
auto list<T, Allocator>::operator!=(const list<T, Allocator>& rhs) const -> bool {
    return !(*this == rhs);
}
}; // namespace cont
