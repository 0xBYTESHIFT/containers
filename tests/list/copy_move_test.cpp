#include "list.hpp"
#include "test_struct.hpp"
#include <cassert>
#include <iostream>

using list_ = cont::list<test_struct>;
auto print_it(const list_& l, const list_::iterator_base& it) {
    std::cout << "iter addr:" << &it << std::endl;
    std::cout << "node addr:" << it.n << std::endl;
    if (it != l.end()) {
        std::cout << "val:" << *it << std::endl;
    } else {
        std::cout << "val:null" << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    {
        //0 - 1 - 2 - 3
        list_ l;
        auto it0 = l.insert(l.begin(), 0);
        l.insert(l.begin(), 2);
        auto it2 = l.insert(it0, 1);
        auto it3 = l.insert_before(l.end(), 3);
        auto it4 = l.insert_before(it3, 4);
        auto it5 = l.erase(it4);
        assert(it5 == it3);

        {
            auto it = l.begin();
            auto end = l.end();
            std::cout << "begin" << std::endl;
            print_it(l, it);
            std::cout << "end" << std::endl;
            print_it(l, end);
            while (it != end) {
                std::cout << "it #" << std::distance(l.begin(), it) << std::endl;
                print_it(l, it);
                ++it;
            }
        }

        list_ copy = l;
        std::cout << "copy done\n";
        {
            auto it = copy.begin();
            auto end = copy.end();
            std::cout << "begin" << std::endl;
            print_it(copy, it);
            std::cout << "end" << std::endl;
            print_it(copy, end);
            while (it != end) {
                std::cout << "it #" << std::distance(copy.begin(), it) << std::endl;
                print_it(copy, it);
                ++it;
            }
        }

        assert(copy == l);
        auto rvalue = std::move(copy);
        std::cout << "move done\n";
        assert(rvalue == l);
    }
    assert(alloc_counter == 0);
}
