#include <iostream>
#include <exception>
#include <cassert>

#include "priorityqueue.hh"

#define line_log std::cout << "Line " << __LINE__ << std::endl


PriorityQueue<int, int> f(PriorityQueue<int, int> q)
{
  return q;
}

int main() {
  PriorityQueue<int, int> P = f(PriorityQueue<int, int>());
  assert(P.empty());

  line_log;

  P.insert(1, 42);
  P.insert(2, 13);

  assert(P.size() == 2);
  assert(P.maxKey() == 1);
  assert(P.maxValue() == 42);
  assert(P.minKey() == 2);
  assert(P.minValue() == 13);

  line_log;

  PriorityQueue<int, int> Q(f(P));
  assert(!Q.empty());
  assert(Q.size() == 2);

  Q.deleteMax();
  Q.deleteMin();
  Q.deleteMin();

  line_log;

  assert(Q.empty());

  PriorityQueue<int, int> R(Q);

  R.insert(1, 100);
  R.insert(2, 100);
  R.insert(3, 300);

  PriorityQueue<int, int> S;
  S = R;

  assert (!S.empty());

  line_log;

  try
  {
    S.changeValue(4, 400);
  }
  catch (const PriorityQueueNotFoundException& pqnfex)
  {
    std::cout << pqnfex.what() << std::endl;
  }
  catch (...)
  {
    assert(!"exception missing!");
  }

  line_log;
  S.changeValue(2, 200);
  line_log;

  try
  {
    line_log;
    while (true)
    {
      line_log;
      std::cout << S.minValue() << std::endl;
      S.deleteMin();
    }
    assert(!"S.minValue() on empty S did not throw!");
  }
  catch (const PriorityQueueEmptyException& pqeex)
  {
    std::cout << pqeex.what() << std::endl;
  }
  catch (...)
  {
    assert(!"exception missing!");
  }

  try
  {
    line_log;
    S.minKey();
    assert(!"S.minKey() on empty S did not throw!");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
  }
  catch (...)
  {
    assert(!"exception missing!");
  }

  line_log;

  PriorityQueue<int, int> T;
  T.insert(1, 1);
  T.insert(2, 4);
  S.insert(3, 9);
  S.insert(4, 16);
  S.merge(T);
  assert(S.size() == 4);
  assert(S.minValue() == 1);
  assert(S.maxValue() == 16);
  assert(T.empty());

  S = R;
  swap(R, T);
  assert(T == S);
  assert(T != R);

  R = std::move(S);
  assert(T != S);
  assert(T == R);

  PriorityQueue<int, int> AA, BB;
  assert(AA == BB);


  std::cout << "ALL OK!" << std::endl;

  return 0;
}