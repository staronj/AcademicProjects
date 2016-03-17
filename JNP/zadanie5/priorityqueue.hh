#ifndef PRIORITYQUEUE_HH
#define PRIORITYQUEUE_HH

#include <memory>
#include <functional>
#include <exception>
#include <set>

class PriorityQueueNotFoundException : public std::exception {
  public:
  virtual const char* what() const noexcept override {
    return "PriorityQueueNotFoundException";
  }
};

class PriorityQueueEmptyException : public std::exception {
  public:
  virtual const char* what() const noexcept override {
    return "PriorityQueueEmptyException";
  }
};

template<typename K, typename V>
class PriorityQueue {
  public:
  // definicje typów
  using size_type = std::size_t;
  using key_type = K;
  using value_type = V;
  using queue_type = PriorityQueue<key_type, value_type>;

  using key_pointer = std::shared_ptr<key_type>;
  using value_pointer = std::shared_ptr<value_type>;

  using value_key_pair_type = std::pair<value_pointer, key_pointer>;
  using key_value_pair_type = std::pair<key_pointer, value_pointer>;

  PriorityQueue() = default;

  /**
   * Konstruktor kopiujący.
   * Złożoność O(size), ponieważ kopiowanie multisetu O(size).
   */
  PriorityQueue(const queue_type& queue) :
      value_key_set(queue.value_key_set),
      key_value_set(queue.key_value_set) { }

  /**
   * Konstruktor przenoszący.
   * Złożoność O(1), ponieważ przenoszenie setów w O(1).
   */
  PriorityQueue(queue_type&& queue) {
    value_key_set = std::move(queue.value_key_set);
    key_value_set = std::move(queue.key_value_set);
  }

  /**
   * Złożoność O(1) dla użycia  P = move(Q), ponieważ queue
   * będzie wtedy skonstruowane używając konstruktora przenoszącego
   * oraz swap w czasie stałym.
   * W przeciwnym przypadku O(size) ponieważ wywołany będzie konstruktor
   * kopiujący.
   */
  queue_type& operator=(queue_type queue) {
    if (this == &queue)
      return *this;

    swap(queue);
    return *this;
  }

  /**
   * O(1), bo empty() O(1)
   */
  bool empty() const noexcept {
    return value_key_set.empty();
  }

  /**
   * O(1), bo size() O(1)
   */
  size_type size() const noexcept {
    return value_key_set.size();
  }

  /**
   * O(log size), gdyż zarówno find jak i insert działają w czasie O(log size),
   * a erase na iteratorze działa w O(1) (zamortyzowane)
   */
  void insert(const K& key_, const V& value_) {
    key_pointer key = std::make_shared<key_type>(key_);
    value_pointer value = std::make_shared<value_type>(value_);

    auto it1 = value_key_set.find(std::make_pair(value, key)); // log size, can throw from new and comparator
    /*
     * jeśli w strukturze jest już taka para, znajdujemy ją a następnie podmieniamy
     * naszą, co spowoduje zwolnienie nowej pary
     **/
    if (it1 != value_key_set.end()) {
      value = it1->first;
      key = it1->second;
    }

    auto it2 = value_key_set.insert(std::make_pair(value, key)); // can throw from new and comparator
    try {
      key_value_set.insert(std::make_pair(key, value));
    }
    catch (...) {
      value_key_set.erase(it2);
      throw;
    }
  }

  /**
   * O(1), gdyż begin() w O(1)
   */
  const V& minValue() const {
    if (empty())
      throw PriorityQueueEmptyException();
    return *(value_key_set.begin()->first);
  }

  /**
   * O(1), gdyż rbegin() w O(1)
   */
  const V& maxValue() const {
    if (empty())
      throw PriorityQueueEmptyException();
    return *(value_key_set.rbegin()->first);
  }

  /**
   * O(1), gdyż begin() w O(1)
   */
  const K& minKey() const {
    if (empty())
      throw PriorityQueueEmptyException();
    return *(value_key_set.begin()->second);
  }

  /**
   * O(1), gdyż rbegin() w O(1)
   */
  const K& maxKey() const {
    if (empty())
      throw PriorityQueueEmptyException();
    return *(value_key_set.rbegin()->second);
  }

  /**
   * O(log size), gdyż begin() w O(1), find w O(log size), oraz
   * erase na iteratorze w O(1)
   */
  void deleteMin() {
    if (empty())
      return;

    key_pointer key;
    value_pointer value;
    auto it1 = value_key_set.begin();

    std::tie(value, key) = *it1;
    auto it2 = key_value_set.find(std::make_pair(key, value));

    value_key_set.erase(it1);
    key_value_set.erase(it2);
  }

  /**
   * Analogicznie jak deleteMin, oddatkowo
   */
  void deleteMax() {
    if (empty())
      return;

    key_pointer key;
    value_pointer value;
    auto it1 = std::prev(value_key_set.end());
    std::tie(value, key) = *it1;
    auto it2 = key_value_set.find(std::make_pair(key, value));

    value_key_set.erase(it1);
    key_value_set.erase(it2);
  }

  /**
   * O(log size), ponieważ lower_bound w czasie O(log size),
   * find w czasie O(log size), insert w czasie O(log size),
   * a erase na iteratorze w czasie O(1)
   */
  void changeValue(const K& key_, const V& value_) {
    key_pointer key = std::make_shared<key_type>(key_);
    value_pointer new_value = std::make_shared<value_type>(value_);

    // nullptr is before all other values, see our comparator
    auto old1 = key_value_set.lower_bound(std::make_pair(key, nullptr)); // can throw
    if (old1 == key_value_set.end() || key_ < *(old1->first) || *(old1->first) < key_ ) {
      throw PriorityQueueNotFoundException();
    }

    value_pointer old_value = old1->second;

    auto old2 = value_key_set.find(std::make_pair(old_value, key));

    auto new1 = value_key_set.insert(std::make_pair(new_value, key));
    try {
      key_value_set.insert(std::make_pair(key, new_value));
    }
    catch (...) {
      value_key_set.erase(new1);
      throw;
    }

    key_value_set.erase(old1);
    value_key_set.erase(old2);
  }

  /**
   * O(size() + queue.size() * log (queue.size() + size())) ponieważ
   * kopiowanie kolejki w O(size()), inserty w sumarycznym czasie
   * O(queue.size() * log (queue.size() + size()))
   */
  void merge(PriorityQueue& queue) {
    if (this == &queue)
      return;

    queue_type copy(*this);

    copy.value_key_set.insert(queue.value_key_set.begin(), queue.value_key_set.end());
    copy.key_value_set.insert(queue.key_value_set.begin(), queue.key_value_set.end());

    swap(copy);
    queue.value_key_set.clear();
    queue.key_value_set.clear();
  }

  /**
   * O(1), bo swap w O(1)
   */
  void swap(PriorityQueue& queue) {
    value_key_set.swap(queue.value_key_set);
    key_value_set.swap(queue.key_value_set);
  }

  /**
   * O(size), bo < w O(size)
   */
  friend bool operator==(const queue_type& a, const queue_type& b) {
    return a.size() == b.size() && !(a < b) && !(b < a);
  }

  /**
   * O(size) bo == w O(size)
   */
  friend bool operator!=(const queue_type& a, const queue_type& b) {
    return !(a == b);
  }

  /**
   * O(size), bo iteracja przez całe drzewo w zamortyzowanym O(size)
   */
  friend bool operator<(const queue_type& a, const queue_type& b) {
    return std::lexicographical_compare(a.key_value_set.begin(),
                                        a.key_value_set.end(),
                                        b.key_value_set.begin(),
                                        b.key_value_set.end(),
                                        SharedPairComparator<key_value_pair_type>());
  }

  friend bool operator>(const queue_type& a, const queue_type& b) {
    return b < a;
  }

  friend bool operator<=(const queue_type& a, const queue_type& b) {
    return !(b < a);
  }

  friend bool operator>=(const queue_type& a, const queue_type& b) {
    return !(a < b);
  }

  private:

  template<typename pair_type>
  struct SharedPairComparator {
    bool operator()(const pair_type& a, const pair_type& b) const {
      if (sharedPtrCompare(a.first, b.first))
        return true;
      else if (sharedPtrCompare(b.first, a.first))
        return false;
      else
        return sharedPtrCompare(a.second, b.second);
    }

    private:
    template<typename T>
    static bool sharedPtrCompare(const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
      if (!a || !b)
        return (a.get() == nullptr && b.get() != nullptr);
      else
        return *a < *b;
    }
  };

  using value_key_set_type = std::multiset<value_key_pair_type, SharedPairComparator<value_key_pair_type>>;
  using key_value_set_type = std::multiset<key_value_pair_type, SharedPairComparator<key_value_pair_type>>;

  value_key_set_type value_key_set;
  key_value_set_type key_value_set;
};

template<typename K, typename V>
inline void swap(PriorityQueue<K, V>& a, PriorityQueue<K, V>& b) {
  a.swap(b);
};

#endif // PRIORITYQUEUE_HH
