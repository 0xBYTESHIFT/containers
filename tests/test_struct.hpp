#pragma once
#include <iostream>

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
    auto value() const -> int { return val; }
};
