/*
 * Author: Yunho Cho
 * Date: 2019-05-27
 * Project:
 * Purpose: 
 * Notes:
 */

#ifndef BPLUSTREE_PAIR_H
#define BPLUSTREE_PAIR_H
#include <iostream>

template <typename K, typename V>
struct Pair{
    // Member variable
    K key;
    V value;

    // Constructor
    explicit Pair(const K& k=K(), const V& v=V());

    // Operators
    friend std::ostream& operator <<(std::ostream& outs, const Pair<K, V>& print_me) {
        std::cout << print_me.key << "::" << print_me.value;
        return outs;
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {

        return (lhs.key == rhs.key) && (lhs.value == rhs.value);
    }
    friend bool operator !=(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {

        return (lhs.key != rhs.key) || (lhs.value != rhs.value);
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key<rhs.key;
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key>rhs.key;
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key<=rhs.key;
    }
    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key>=rhs.key;
    }
//    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs);
};

template<typename K, typename V>
Pair<K, V>::Pair(const K &k, const V &v)
{
    key = k;
    value = v;
}
#endif //BPLUSTREE_PAIR_H
