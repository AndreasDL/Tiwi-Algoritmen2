#ifndef __REDBLACKTREE_H
#define __REDBLACKTREE_H
#include <iostream>
using namespace std;

template<class K, class D>
class Node;
template <class K,class D>
class RBTree;

template <class K,class D>
class RBTree{
	friend class Node<K,D>;
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

	void rotLeft(Node<K,D>* n);
	void rotRight(Node<K,D>* n);
};

template<class K, class D>
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


//tree implementation///////////////////////////////////////////////////////////

template<class K, class D>
RBTree<K,D>::RBTree(): _size(0),_root(0){}

template<class K, class D>
const D& RBTree<K,D>::search(const K& key){
	cout << "searching " << key;
	Node<K,D>* n = searchNode(key);

	if ( n != 0 && n->_key == key ){
		cout << " found " << *n << endl;
		return n->_data;
	}else{
		cout << " found nothing";
		if (n != 0)
			cout << " ended in " << *n;
		cout << endl;
		return 0;
	}
}
template<class K, class D>
Node<K,D>* RBTree<K,D>::searchNode(const K& key){
	Node<K,D>* n = _root;
	Node<K,D>* o = _root;

	while (n != 0 && key != n->_key){
		o = n;
		if ( key < n->_key){
			n = n->_left;
		}else{
			n = n->_right;
		}
	}
	//return n; this is either the node containing the key, or the should be parent
	return ( n == 0 ? o : n);
}

template<class K, class D>
void RBTree<K,D>::add(const K& key, const D& data){
	if ( _size == 0){
		_root = new Node<K,D>(key,data, true);
		_size++;
	}else{
		Node<K,D>* n = searchNode(key); //n can't be 0 here b/c that means the size = 0 and we checked that
		if (n->_key != key){ //key not in tree
			(key < n->_key ? n->_left : n->_right) = new Node<K,D>(key,data,false);			
		}
	}
}



//node implementation ////////////////////////////////////////////////////

template <class K,class D>
Node<K,D>::Node(const K& key,const D& data, bool isBlack): _key(key), _data(data),_isBlack(isBlack){}

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
		<< " Data= " << n._data;
}

#endif