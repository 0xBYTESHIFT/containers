#include "k_tree.hpp"
#include "test_struct.hpp"
#include <cassert>
#include <iostream>

using tree_ = cont::tree<test_struct>;
auto print_it(const tree_& t, const tree_::iterator_base& it) {
    std::cout << "iter addr:" << &it << std::endl;
    std::cout << "node addr:" << it.n << std::endl;
    if (it != t.end()) {
        std::cout << "val:" << *it << std::endl;
    } else {
        std::cout << "val:null" << std::endl;
    }
    std::cout << std::endl;
}

auto make_tree() {
    /*   0
        /|\
       1-2-5
         ^
        3-4
     depth-wise: 0 1 2 3 4 5
    */
    tree_ tree;
    std::cout << "default constructor done\n";
    auto it0 = tree.set_root<tree_::df_iterator>(0);
    std::cout << "set root done\n";
    tree.append_child(it0, 1);
    auto it2 = tree.append_child(it0, 2);
    tree.append_child(it2, 3);
    tree.append_child(it2, 4);
    auto it5 = tree.append_child(it0, 5);
    return tree;
}

int main() {
    auto bak = alloc_counter;
    {
        auto tree = make_tree();
        auto it = tree.begin();
        auto end = tree.end();
        std::cout << "begin" << std::endl;
        print_it(tree, it);
        std::cout << "end" << std::endl;
        print_it(tree, end);
        while (it != end) {
            std::cout << "it #" << std::distance(tree.begin(), it) << std::endl;
            print_it(tree, it);
            it++;
        }
    }
    assert(bak == alloc_counter);
    std::cout << "bak:" << bak << '\n';
    std::cout << "alloc:" << alloc_counter << '\n';
    std::cout << "simple tree test done\n\n";

    bak = alloc_counter;
    {
        auto tree = make_tree();
        std::cout << "bak:" << bak << '\n';
        std::cout << "alloc:" << alloc_counter << '\n';
        std::cout << "make tree done\n\n";
        tree_ copy;
        copy = tree;
        std::cout << "bak:" << bak << '\n';
        std::cout << "alloc:" << alloc_counter << '\n';
        std::cout << "copy tree done\n\n";
        tree_ move;
        move = std::move(copy);
        std::cout << "bak:" << bak << '\n';
        std::cout << "alloc:" << alloc_counter << '\n';
        std::cout << "move tree done\n\n";
    }
    std::cout << "bak:" << bak << '\n';
    std::cout << "alloc:" << alloc_counter << '\n';
    assert(bak == alloc_counter);
    std::cout << "copy tree test done\n\n";
}
