#include "bst_imp.h"
#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>

using namespace std;

template <typename T>
void assertEQ(const BST<T>& bst, const string &s) {
    stringstream ss;
    ss << bst;
    assert(ss.str() == s || ss.str() ==  s + ' ');
}

#define assertThrow(OP) { \
    try { \
    OP; \
    assert(false); \
    } catch(std::logic_error&) { } \
}


void assertEmpty(const BST<int> &bst) {
    assertEQ(bst, "");
    assert(bst.size() == 0);
    assertThrow(bst.min());
    assertThrow(bst.max());
    assert(bst.height() == 0);
}

void assertOne(const BST<int> &bst) {

        assertEQ(bst, "3");
        assert(bst.size() == 1);
        assert(bst.min() == 3);
        assert(bst.max() == 3);
        assert(bst.height() == 1);

}

int main() {
    {
        BST<int> bst({});
        assertEmpty(bst);
        BST<int> bst2 = bst;
        assertEmpty(bst2);
        std::vector<int> v;
        BST<int> bst3(v.begin(), v.end());
        assertEmpty(bst3);
        assertOne(bst + 3);
        assertOne(3 + bst);
    }


    {
        BST<int> bst({3});
        assertOne(bst);
    }

    {
        BST<int> bst({3, 4});
        // 3    |
        //  \   |
        //   4  |
        assertEQ(bst, "3 4");
        assert(bst.size() == 2);
        assert(bst.min() == 3);
        assert(bst.max() == 4);
        assert(bst.height() == 2);
    }

    {
        BST<int> bst({3, 4, 1});
        //   3    |
        //  / \   |
        // 1   4  |
        assertEQ(bst, "1 3 4");
        assert(bst.size() == 3);
        assert(bst.min() == 1);
        assert(bst.max() == 4);
        assert(bst.height() == 2);
    }

    {
        BST<int> bst({3, 4, 1, 2});
        //    3    |
        //   / \   |
        //  1   4  |
        //   \     |
        //    2    |
        assertEQ(bst, "1 2 3 4");
        assert(bst.size() == 4);
        assert(bst.min() == 1);
        assert(bst.max() == 4);
        assert(bst.height() == 3);
    }
    {
        BST<std::string> bst ({"abc", "a", "abc", "gde", "ab"});
        assertEQ(bst, "a ab abc abc gde");
        assert(bst.size() == 5);
        assert(bst.min() == "a");
        assert(bst.max() == "gde");
        assert(bst.height() == 4);
        assertEQ(bst.left(), "a ab abc");
        assertEQ(bst.right(), "gde");
        assertEQ(bst.find("ab"), "ab");
        assert(!bst.empty());
        assert(bst.value() == "abc");
        assert(spine(bst).height() == bst.size());
        assert((bst + string("XD")).size() == 6);
        assert((string("XD") + bst).size() == 6);
        vector<string> v;
        BST<string> (v.begin(), v.end());


    }


    {
        BST<int> bst({3,4,1,2,7});
        //    3      |
        //   / \     |
        //  1   4    |
        //   \   \   |
        //    2   7  |
        assertEQ(bst, "1 2 3 4 7");
        assert(bst.size() == 5);
        assert(bst.min() == 1);
        assert(bst.max() == 7);
        assert(bst.height() == 3);
        assert(bst.find(1).min() == 1);
        assert(bst.find(1).max() == 2);
        assert(max_diff(bst) == 3);
    }

    BST<int> bst({3,4,1,2,7,3});
    //    3          |
    //   / \         |
    //  1   4        |
    //   \   \       |
    //    2   7      |
    //     \         |
    //      3        |



    assertEQ(bst, "1 2 3 3 4 7");

    assert(bst.size() == 6);
    assert(bst.min() == 1);
    assert(bst.max() == 7);
    assert(bst.height() == 4);
    assert(spine(bst).height() == 6);
    auto spina = spine(bst);
    assert(spina.find(7).height() == 6);
    assert(spina.find(4).height() == 5);
    assert(spina.find(3).height() == 4);
    assert(spina.find(3).min() == 1);
    assert(spina.find(3).max() == 3);
    assert(spina.find(2).height() == 2);
    assert(spina.find(1).height() == 1);
    assert(spina.find(42).height() == 0);


    assertEQ(bst.find(4), "4 7");
    assertEQ(bst.find(11), "");
    assert(max_diff(bst) == 3);

    auto newBST = bst + 42;
    assertEQ(newBST, "1 2 3 3 4 7 42");
    assert(max_diff(newBST) == 35);
    assert(newBST.size() == 7);
    assert(newBST.min() == 1);
    assert(newBST.max() == 42);




    cout << "Done" << endl;
    return 0;
}


