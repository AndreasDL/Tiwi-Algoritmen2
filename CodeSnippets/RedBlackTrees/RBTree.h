#ifndef __REDBLACKTREE_H
#define __REDBLACKTREE_H
#include "Node.h"

template<class K, class D>
class Node;

template <class K,class D>
class RBTree{
public:
	RBTree<K,D>();
	~RBTree();

	const D& search(const K& key);
	void add(const K& key, const D& data);
	void del(const K& key);
private:
	Node<K,D>* _root;
	int _size;
	Node<K,D>* searchNode(const K& key);
};

template<class K, class D>
RBTree<K,D>::RBTree(): _size(0),_root(0){}

template<class K, class D>
void RBTree<K,D>::add(const K& key, const D& data){
	if ( _size == 0){
		_root = new Node<K,D>(key,data);
		_size++;
	}
}

template<class K, class D>
Node<K,D>* RBTree<K,D>::searchNode(const K& key){
	Node<K,D>* n = _root;
	Node<K,D>* o = 0;
	while (n != 0 && key != n->_key){
		o = n;
		n = (key < (n->_left)->_key) ? n->_left : n->_right;
	}
	//return n; this is either the node containing the key, or the should be parent
	return ( n == 0 ? o : n);
}

template<class K, class D>
const D& RBTree<K,D>::search(const K& key){
	Node<K,D>* n = searchNode(key);
	
	if ( n != 0 && n->_key == key ){
		return n->_data;
	}else{
		return 0;
	}
}




#endif