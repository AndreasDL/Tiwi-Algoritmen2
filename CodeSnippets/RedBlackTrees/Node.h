#ifndef __Node_H
#define __Node_H
#include <iostream>
#include "RBTree.h"
//template<class K, class D>
//class RBTree;

template <class K,class D>
class Node{
friend class RBTree<K,D>;
public:
	Node(const K& key, const D& data, const bool isBlack);
	~Node();
	
	const D& getData();
	const K& getKey();

	bool isBlack();
	Node* getLeft();
	Node* getRight();
	
	void setBlack(bool b);
	void setLeft(Node* n);
	void setRight(Node* n);
	
	template<class L,class E> 
	friend std::ostream &operator<<(std::ostream &stream, const Node<L,E> &n);
	template<class L,class E> 
	friend std::ostream &operator<(const Node<L,E> &m, const Node<L,E> &n){ return m._key < n._key;	}
	template<class L,class E> 
	friend std::ostream &operator>(const Node<L,E> &m, const Node<L,E> &n){ return m._key > n._key; }
	template<class L,class E>
	friend std::ostream &operator<=(const Node<L,E> &m, const Node<L,E> &n){ return m._key <= n._key; }
	template<class L,class E> 
	friend std::ostream &operator>=(const Node<L,E> &m, const Node<L,E> &n){ return m._key >= n._key; }
	template<class L,class E>
	friend std::ostream &operator==(const Node<L,E> &m, const Node<L,E> &n){ return m._key == n._key; }
	
private:
	bool _isBlack;
	K _key;
	D _data;
	Node<K,D>* _left;
	Node<K,D>* _right;
};

template <class K,class D>
Node<K,D>::Node(const K& key,const D& data, bool isBlack)
	: _key(key), _data(data),_isBlack(isBlack){

}

template <class K,class D>
const D& Node<K,D>::getData(){
	return _data;
}
template <class K,class D>
const K& Node<K,D>::getKey(){
	return _key;
}
template <class K,class D>
bool Node<K,D>::isBlack(){
	return _isBlack;
}
template <class K,class D>
Node<K,D>* Node<K,D>::getLeft(){
	return _left;
}
template <class K,class D>
Node<K,D>* Node<K,D>::getRight(){
	return _right;
}

template <class K,class D>
void Node<K,D>::setBlack(bool isBlack){
	_isBlack = isBlack;
}
template <class K,class D>
void Node<K,D>::setRight(Node* n){
	_right = n;
}
template <class K,class D>
void Node<K,D>::setLeft(Node* n){
	_left = n;
}
template <class L,class E>
std::ostream &operator<<(std::ostream &stream, const Node<L,E> &n){
	return stream << (n._isBlack ? "Black " : "Red") 
		<< "Node Key= " << n._key
		<< " Data= " << n._data << std::endl;
}


#endif