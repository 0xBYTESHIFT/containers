#include "list.hpp"
#include "test_struct.hpp"
#include <iostream>
#include <random>

using list_ = cont::list<test_struct>;
int main() {
    {
        const int size = 1000;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 3);
        list_ l(1, 0);
        int resulting_size = 1;
        for (int i = 0; i < size; i++) {
            auto num = dist(gen);
            std::uniform_int_distribution<int> node_dist(0, l.size() - 1);
            auto node_num = node_dist(gen);
            auto it = std::next(l.begin(), node_num);
            std::cout << "selected node:" << node_num << "\t";
            if (num == 0) {
                std::cout << "inserting:";
                *it = i;
            } else if (num == 1) {
                std::cout << "inserting left:";
                l.insert_before(it, i);
                resulting_size++;
            } else if (num == 2) {
                std::cout << "inserting right:";
                l.insert(it, i);
                resulting_size++;
            } else if (num == 3 && l.size() > 1) {
                std::cout << "erasing child:";
                l.erase(it);
                resulting_size--;
            }
            std::cout << i << std::endl;
        }
        std::cout << "tree size:" << l.size() << "\twanted nodes num:" << resulting_size << std::endl;
        assert(l.size() == resulting_size);
        auto it = l.begin();
        while (it != l.end()) {
            std::cout << "node:" << *it << "\n";
            ++it;
        }
    }
    assert(alloc_counter == 0);
    return 0;
}
