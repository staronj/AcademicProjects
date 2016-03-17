#include "bst_imp.h"
#include <cassert>

#define ASSERT_THROW(instruction) try { instruction; assert(0 && __LINE__); } catch(const std::logic_error& e) {  }
#define assert_stucture(ins, l, r, v) assert (!ins.left().empty() == l);  assert (!ins.right().empty() == r);  assert (ins.value() == v)

int main() {
  { // empty tree creation
    BST<int> bst({});
    assert(bst.empty());
    assert(bst.size() == 0);
    assert(bst.height() == 0);

    ASSERT_THROW(bst.left());
    ASSERT_THROW(bst.right());
    ASSERT_THROW(bst.value());
    ASSERT_THROW(bst.min());
    ASSERT_THROW(bst.max());

    int t[1] = {1};
    BST<int> bst2(t, t);
    assert(bst2.empty());
    assert(bst2.size() == 0);
    assert(bst2.height() == 0);
  }

  // testy struktury drzewa z treści
  {
    BST<int> bst({3});
    // 3

    assert_stucture (bst, false, false, 3);

    assert (bst.max() == 3);
    assert (bst.min() == 3);
  }

  {
    BST<int> bst({3, 4});
    // 3    |
    //  \   |
    //   4  |

    assert_stucture (bst, false, true, 3);
    assert_stucture (bst.right(), false, false, 4);

    assert (bst.max() == 4);
    assert (bst.min() == 3);
  }

  {
    BST<int> bst({3, 4, 1});
    //   3    |
    //  / \   |
    // 1   4  |

    assert_stucture (bst, true, true, 3);
    assert_stucture (bst.right(), false, false, 4);
    assert_stucture (bst.left(), false, false, 1);

    assert (bst.max() == 4);
    assert (bst.min() == 1);
  }

  {
    BST<int> bst({3, 4, 1, 2});
    //    3    |
    //   / \   |
    //  1   4  |
    //   \     |
    //    2    |

    assert_stucture (bst, true, true, 3);
    assert_stucture (bst.right(), false, false, 4);
    assert_stucture (bst.left(), false, true, 1);
    assert_stucture (bst.left().right(), false, false, 2);

    assert (bst.max() == 4);
    assert (bst.min() == 1);
  }
  {
    BST<int> bst({3,4,1,2,7});
    //    3      |
    //   / \     |
    //  1   4    |
    //   \   \   |
    //    2   7  |

    assert_stucture (bst, true, true, 3);
    assert_stucture (bst.right(), false, true, 4);
    assert_stucture (bst.left(), false, true, 1);
    assert_stucture (bst.left().right(), false, false, 2);
    assert_stucture (bst.right().right(), false, false, 7);

    assert (bst.max() == 7);
    assert (bst.min() == 1);
  }

  {
    BST<int> bst({3,4,1,2,7,3});
    //    3          |
    //   / \         |
    //  1   4        |
    //   \   \       |
    //    2   7      |
    //     \         |
    //      3        |

    assert_stucture (bst, true, true, 3);
    assert_stucture (bst.right(), false, true, 4);
    assert_stucture (bst.left(), false, true, 1);
    assert_stucture (bst.left().right(), false, true, 2);
    assert_stucture (bst.right().right(), false, false, 7);
    assert_stucture (bst.left().right().right(), false, false, 3);

    assert (bst.max() == 7);
    assert (bst.min() == 1);
  }

  {
    BST<int> a({3,4,1,2,7,3});
    BST<int> bst = spine(a);

    assert_stucture (bst, true, false, 7);
    assert_stucture (bst.left(), true, false, 4);
    assert_stucture (bst.left().left(), true, false, 3);
    assert_stucture (bst.left().left().left(), true, false, 3);
    assert_stucture (bst.left().left().left().left(), true, false, 2);
    assert_stucture (bst.left().left().left().left().left(), false, false, 1);

    assert (a.max() == 7);
    assert (a.min() == 1);
  }




}
