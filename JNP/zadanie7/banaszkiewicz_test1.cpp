#include "bst_imp.h"
#include "bst_imp.h" //IFNDEF!!!
#include <vector>
#include <cassert>
#include <algorithm>

int main() 
{

    {
        BST<int> bst({3});
        // 3
    }

    {
        BST<int> bst({3, 4});
        // 3    |
        //  \   |
        //   4  |
    }

    {
        BST<int> bst({3, 4, 1});
        //   3    |
        //  / \   |
        // 1   4  |
    }

    {
        BST<int> bst({3, 4, 1, 2});
        //    3    |
        //   / \   |
        //  1   4  |
        //   \     |
        //    2    |
    }
    {
        BST<int> bst({3,4,1,2,7});
        //    3      |
        //   / \     |
        //  1   4    |
        //   \   \   |
        //    2   7  |
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

        std::cout << bst << std::endl; // prints 1 2 3 3 4 7
        std::cout << bst.size() << std::endl; //prints 6
        std::cout << bst.min() << std::endl; // prints 1
        std::cout << bst.max() << std::endl; // prints 7
        std::cout << bst.height() << std::endl; // prints 4
        std::cout << spine(bst).height() << std::endl; // prints 6
        std::cout << bst.find(4) << std::endl; // prints 4 7
        int exceptions = 0;
        try {
            std::cout << bst.find(11) << std::endl; //throws
        } 
        catch (std::logic_error)
        {
            exceptions++;
        }
        assert(exceptions);
        
        std::cout << max_diff(bst) << std::endl; //prints 3
    }
    
    {
         BST<int> bst({});
         assert(bst.empty());
         
         int exceptions = 0;

         try 
         {
            bst.left();
         } 
         catch(std::logic_error)
         {
            ++exceptions;
         }

         try 
         {
            bst.right();
         } 
         catch(std::logic_error)
         {
            ++exceptions;
         }

         try 
         {
            bst.value();
         } 
         catch(std::logic_error)
         {
            ++exceptions;
         }

         try 
         {
            bst.min();
         } 
         catch(std::logic_error)
         {
            ++exceptions;
         }

         try 
         {
            bst.max();
         } 
         catch(std::logic_error)
         {
            ++exceptions;
         }

         try 
         {
            max_diff(bst);
         } 
         catch(std::logic_error)
         {
            ++exceptions;
         }

         try 
         {
            bst.find(0);
         } 
         catch(std::logic_error)
         {
            ++exceptions;
         }

         assert(exceptions == 7);
         assert(!bst.size());
         assert(!bst.height());
    }

    {
        BST<int> bst({100, 52, 142, 166, 109, 105, 134, 150, 170});
        assert(!bst.empty());
        assert(bst.size() == 9);
        assert(bst.min() == 52);
        assert(bst.max() == 170);

        assert(bst.height() == 4); 
        assert(spine(bst).height() == 9);
        assert(bst.find(52).height() == 1);
        assert(bst.find(109).height() == 2);
        assert(bst.find(142).height() == 3);
        assert(max_diff(bst) == 48);

        BST<int> bst2 = bst + 1000;

        assert(bst2.size() - 1 == bst.size());
        assert(bst2.max() == 1000);

        BST<int> bst3 = 1 + bst2;
        assert(bst3.size() - 1 == bst2.size());
        assert(bst3.min() == 1);

    }

    {
        BST<int> bst({1});
        assert(!bst.empty());
        assert(bst.size() == 1);
        assert(bst.min() == 1);
        assert(bst.max() == 1);

        assert(bst.height() == 1); 
        assert(spine(bst).height() == 1);
        assert(bst.find(1).height() == 1);
        
        int exceptions = 0;

        try 
        {
            max_diff(bst);
        } 
        catch(...)
        {
            ++exceptions;
        }

        assert(exceptions);

        BST<int> bst2 = bst + 1;
        BST<int> bst3 = bst2 + 1;

        assert(!bst.empty());
        assert(bst.size() == 1);
        assert(bst.min() == 1);
        assert(bst.max() == 1);

        assert(bst.height() == 1); 
        assert(spine(bst).height() == 1);
        assert(bst.find(1).height() == 1);


        assert(bst3.height() == 3); 
        assert(spine(bst3).height() == 3);
        // assert(bst3.find(1).height() == 3); // depends on implementation 

        assert(!max_diff(bst3));
    }

    {
        BST<int> bst({7, 0, 1, 1, 1, 1, 1, 5, 6});
        

        auto f = [] (int a, int b) {
            return a + b;
        };

        assert(bst.fold(0, f) == 23);

    }

    {
        BST<int> bst = BST<int>();
        
        auto f = [] (int a, int b) {
            return a + b;
        };

        assert(bst.fold(10, f) == 10);

    }

    {
        BST<int> bst({});
        
        BST<int> bst2 = bst + 1;

        assert(!bst2.empty());
        assert(bst.empty());
        assert(bst2.size() == 1);
        assert(bst2.height() == 1);
        assert(bst2.min() == 1);
        assert(bst2.max() == 1);
    }

    {
        BST<int> bst({});
        
        BST<int> bst2 = -1 + bst;

        assert(!bst2.empty());
        assert(bst2.size() == 1);
        assert(bst2.height() == 1);
        assert(bst2.min() == -1);
        assert(bst2.max() == -1);
    }

    {
        std::vector<int> vect;

        for (int i = 1; i <= 100; ++i) 
            vect.push_back(i); 

        std::random_shuffle (vect.begin(), vect.end());

        BST<int> bst(vect.begin(), vect.end());

        assert(spine(bst).height() == 100);
        assert(bst.size() == 100);
        assert(bst.min() == 1);
        assert(bst.max() == 100);
        std::cout << bst << std::endl; //prints 1..100

    }

    {
        BST<int> bst({25, 5, 9, 1});
        assert(max_diff(bst) == 16);

        BST<int> bst2 = bst + 500;
        BST<int> bst3 = bst2 + 700;
        BST<int> bst4 = bst3 + 495;
        assert(max_diff(bst4) == 470);

    }

    {
        BST<int> bst({10, 30, 11, 31});
        assert(max_diff(bst) == 19);
    }

    {
        BST<int> bst({10, 30, 11, 31});
        assert(max_diff(bst) == 19);
    }

    {
        BST<int> bst({3, -7, 10, -20, 1});
        assert(max_diff(bst) == 13);
    }

    {

        std::vector<BST<int>> bst_vect;

        bst_vect.push_back(BST<int>({}));

        for (int i = 1; i <= 100; ++i) 
            bst_vect.push_back(i + bst_vect[i - 1]);

        for (int i = 1; i <= 100; ++i) 
        {
            assert(bst_vect[i].size() == (size_t) i);
            assert(bst_vect[i].height() == (size_t) i);
            assert(bst_vect[i].max() == i);
            assert(bst_vect[i].min() == 1);
        }

    }

    {
        std::vector<int> vect;
        BST<int> bst(vect.begin(), vect.end());
        assert(bst.empty());
    }

    {
        BST<int> bst({5});
        BST<int> bst2 = bst.find(5);
        BST<int> bst3 = bst + 1;
        BST<int> bst4 = 19 + bst2;

        assert(bst3.value() == 5);
        assert(bst3.min() == 1);
        assert(bst3.max() == 5);

        assert(bst4.value() == 5);
        assert(bst4.min() == 5);
        assert(bst4.max() == 19);
        
    }

    {
        BST<int> bst({});

        BST<int> bst2 = spine(bst);

        assert(!bst2.size());
        assert(bst2.empty());
    }

    {
        BST<int> bst({5, 6, 12, 43, 0, 45, 0, 3, 5});

        assert(bst.find(3).size() == 2);
        assert(bst.find(0).height() == 3);
    }

    {
        BST<int> bst({5, 6, 12, 43, 0, 45, 0, 3, 5});

        int exceptions = 0;

        try 
        {
            bst.find(4);
        }
        catch(std::logic_error)
        {
            ++exceptions;
        }

        try 
        {
            bst.find(7);
        }
        catch(std::logic_error)
        {
            ++exceptions;
        }

        assert(exceptions == 2);
    }

    {
        BST<int> bst({1});

        int exceptions = 0;

        try 
        {
            bst.find(4);
        }
        catch(std::logic_error)
        {
            ++exceptions;
        }

        assert(exceptions);
    }

    {
        BST<int> bst({-1});

        int exceptions = 0;

        try 
        {
            bst.find(1);
        }
        catch(std::logic_error)
        {
            ++exceptions;
        }

        assert(exceptions);
    }



    // {
    //     BST<int> bst({1, 2});
    //     BST<int> bst2({1, 2});

    //     BST<int> bst3 = BST<int>(1, bst, bst2);

    //     std::cout << bst3;

    // }
    
    return 0;
}