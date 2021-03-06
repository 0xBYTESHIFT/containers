#include "k_tree.hpp"
#include "test_struct.hpp"
#include <iostream>
#include <memory>
#include <vector>

using tree_ = cont::tree<test_struct>;

int main() {
    /* 0
       |
       1-2-5-7
         |
       6-3-4
       breadth-wise: 0 1 2 5 7 6 3 4
    */
    tree_ tree;
    auto it0 = tree.set_root<tree_::bf_iterator>(0);
    tree.append_child(it0, 1);
    auto it2 = tree.append_child(it0, 2);
    auto it3 = tree.append_child(it2, 3);
    tree.append_child(it2, 4);
    auto it5 = tree.append_child(it0, 5);
    tree.insert_left(it3, 6);
    tree.insert_right(it5, 7);

    tree_::bf_iterator it = tree.begin();
    tree_::bf_iterator end = tree.end();
    std::vector<int> result;
    while (it != end) {
        std::cout << *it << " ";
        result.emplace_back((*it).value());
        it++;
    }
    std::cout << std::endl;
    std::vector<int> desired = {0, 1, 2, 5, 7, 6, 3, 4};
    assert(result == desired);
    assert(alloc_counter == desired.size());
}
