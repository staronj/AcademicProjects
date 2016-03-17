#ifndef _BST_IMP_
#define _BST_IMP_

#include <stdexcept>
#include <type_traits>
#include <iostream>
#include "bst.h"

namespace {
template <typename T, typename Iter>
BST<T> BuildTree_(Iter iter, Iter iter1) {
  using TreeType = BST<T>;
  using TreePtr = std::shared_ptr<TreeType>;
  TreePtr result = std::make_shared<TreeType>();
  for (; iter != iter1; ++iter) {
    result = std::make_shared<TreeType>(std::move(*result) + *iter);
  }
  return *result;
}
}

template <typename T>
BST<T>::BST(std::initializer_list<T> list) :
    BST(list.begin(), list.end()) { }

template <typename T>
template <typename Iter>
BST<T>::BST(Iter iter, Iter iter1) :
    BST(BuildTree_<T, Iter>(iter, iter1)) { }

template <typename T>
BST<T>::BST(T value, BST left, BST right) :
    m_root(std::make_shared<Node>(std::move(value), std::move(left.m_root), std::move(right.m_root))) { }

template <typename T>
BST<T> BST<T>::left() const {
  if (empty())
    throw std::logic_error("BST - empty tree");
  return m_root->m_left;
}

template <typename T>
BST<T> BST<T>::right() const {
  if (empty())
    throw std::logic_error("BST - empty tree");
  return m_root->m_right;
}

template <typename T>
T const & BST<T>::value() const {
  if (empty())
    throw std::logic_error("BST - empty tree");
  return m_root->m_value;
}

template <typename T>
bool BST<T>::empty() const {
  return !m_root;
}

template <typename T>
template <typename Acc, typename Functor>
Acc BST<T>::fold(Acc a, Functor fun) const {
  if (empty()) {
    return a;
  }
  else {
    Acc left_fold = fun(left().fold(std::move(a), fun), value());
    return right().template fold<Acc, Functor>(std::move(left_fold), fun);
  }
}

template <typename T>
T const & BST<T>::min() const {
  if (empty())
    throw std::logic_error("BST - empty tree");
  return left().empty()? value() : left().min();
}

template <typename T>
T const & BST<T>::max() const {
  if (empty())
    throw std::logic_error("BST - empty tree");
  return right().empty()? value() : right().max();
}

template <typename T>
BST<T> BST<T>::find(const T & t) const {
  if (empty())
    throw std::logic_error("BST - value not found");
  else if (t < value())
    return left().find(t);
  else if (value() < t)
    return right().find(t);
  else
    return *this;
}

template <typename T>
std::size_t BST<T>::size() const {
  return empty() ? 0 : (1 + left().size() + right().size());
}

template <typename T>
std::size_t BST<T>::height() const {
  return empty() ? 0 : 1 + std::max(left().height(), right().height());
}

template <typename T>
BST<T> operator+(BST<T> tree, T value) {
  if (tree.empty())
    return BST<T>(std::move(value), BST<T>(), BST<T>());
  else if (tree.value() < value)
    return BST<T>(tree.value(), tree.left(), tree.right() + std::move(value));
  else
    return BST<T>(tree.value(), tree.left() + std::move(value), tree.right());
}

template <typename T>
BST<T> operator+(T value, BST<T> tree) {
  return (std::move(tree) + std::move(value));
}

template <typename T>
BST<T> spine(BST<T> tree) {
  return tree.fold(BST<T>(),
                   [](BST<T> left, const T & value) {
                     return BST<T>(std::move(value), left, BST<T>());
                   }
  );
}

template <typename T>
std::ostream & operator<<(std::ostream & stream, BST<T> tree) {
  return tree.fold(std::ref(stream),
                   [](std::ostream & str, const T & value) -> std::ostream & {
                     return str << value << ' ';
                   }
  );
}

template <typename T>
T max_diff(BST<T> tree) {
  static_assert(std::is_arithmetic<T>(), "to use BST<T>::max_diff T must be arithmetic type!");
  if (tree.empty() || (tree.left().empty() && tree.right().empty()))
    throw std::logic_error("max_diff - tree not big enough");

  using ptr = std::unique_ptr<T>;
  ptr result;

  tree.fold(ptr(),
            [&result](ptr previous, const T & value) -> ptr {
              if (previous) {
                T difference = value - *previous;
                if (!result || *result < difference)
                  result.reset(new T(difference));
              }
              return ptr(new T(value));
            }
  );

  return *result;
}

#endif // _BST_IMP_