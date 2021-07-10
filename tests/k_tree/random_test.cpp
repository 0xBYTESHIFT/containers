#include "k_tree.hpp"
#include <iostream>
#include <random>

static size_t alloc_counter = 0;
struct test_struct {
    int val;
    test_struct() {
        val = -1;
        alloc_counter++;
        std::cout << "default constructed at " << this << " cnt:" << alloc_counter << "\n";
    }
    test_struct(test_struct&& str) {
        this->val = str.val;
        std::cout << "moved:" << val << " at " << this << " cnt:" << alloc_counter << "\n";
    }
    test_struct(const test_struct& str) {
        this->val = str.val;
        alloc_counter++;
        std::cout << "copied:" << val << " at " << this << " cnt:" << alloc_counter << "\n";
    }
    test_struct(int val) {
        this->val = val;
        alloc_counter++;
        std::cout << "constructed:" << val << " at " << this << " cnt:" << alloc_counter << "\n";
    }
    ~test_struct() {
        alloc_counter--;
        std::cout << "deleted:" << val << " at " << this << " cnt:" << alloc_counter << "\n";
    }
    auto& operator=(const test_struct& rhs) {
        this->val = rhs.val;
        std::cout << "assigned:" << val << " at " << this << " cnt:" << alloc_counter << "\n";
        return *this;
    }
    auto& operator=(test_struct&& rhs) {
        this->val = rhs.val;
        std::cout << "move-assigned:" << val << " at " << this << " cnt:" << alloc_counter << "\n";
        return *this;
    }
    friend bool operator==(const test_struct& lhs, const test_struct& rhs) {
        return lhs.val == rhs.val;
    }
    friend bool operator!=(const test_struct& lhs, const test_struct& rhs) {
        return !(lhs == rhs);
    }
    friend std::ostream& operator<<(std::ostream& str, const test_struct& s) {
        str << s.val;
        return str;
    }
};

using tree_ = cont::tree<test_struct>;
int main() {
    const int size = 100;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 4);
    tree_ t;
    t.set_root<tree_::df_iterator>(0);
    int resulting_size = 1;
    for (int i = 0; i < size; i++) {
        auto num = dist(gen);
        std::uniform_int_distribution<int> node_dist(0, t.size() - 1);
        auto node_num = node_dist(gen);
        auto it = std::next(t.begin(), node_num);
        if ((num == 1 || num == 2) && it == t.begin()) {
            //incorrect operation
            continue;
        }
        std::cout << "selected node:" << node_num << "\t";
        if (num == 0) {
            std::cout << "inserting:";
            *it = i;
        } else if (num == 1) {
            std::cout << "inserting left:";
            t.insert_left(it, i);
            resulting_size++;
        } else if (num == 2) {
            std::cout << "inserting right:";
            t.insert_right(it, i);
            resulting_size++;
        } else if (num == 3) {
            std::cout << "appending child:";
            t.append_child(it, i);
            resulting_size++;
        } else if (num == 4) {
            std::cout << "prepending child:";
            t.prepend_child(it, i);
            resulting_size++;
        }
        std::cout << i << std::endl;
    }
    std::cout << "tree size:" << t.size() << "\twanted nodes num:" << resulting_size << std::endl;
    assert(t.size() == resulting_size);
    auto it = t.begin();
    while (it != t.end()) {
        std::cout << "node:" << *it << "\n";
        ++it;
    }
    return 0;
}
