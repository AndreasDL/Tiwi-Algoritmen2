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
	Node<K,D>* _k;
	int _size;
	Node<K,D>* searchNode(const K& key);

	void rotLeft();
	void rotRight();
	void addNode(RBTree<K,D>* j, const K& key, const D& data, int sw);
	bool isRed(){ return _k != 0 && !_k->_isBlack; }
};

template<class K, class D>
class Node{
friend class RBTree<K,D>;
public:
	Node(const K& key, const D& data, const bool isBlack): _key(key), _data(data),_isBlack(isBlack){}
	~Node();
	
	const D& getData(){ return _data; }
	const K& getKey(){ return _key; }
		
	template<class L,class E> 
	friend std::ostream &operator<<(std::ostream &stream, const Node<L,E> &n){
		return stream << (n._isBlack ? "Black " : "Red") 
			<< "Node Key= " << n._key
			<< " Data= " << n._data;
	}
private:
	bool _isBlack;
	K _key;
	D _data;
	RBTree<K,D>* _left;
	RBTree<K,D>* _right;
};


//tree implementation///////////////////////////////////////////////////////////
//Top Down it is!

//construct
template<class K, class D>
RBTree<K,D>::RBTree(): _size(0),_k(0){}

//search
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
	Node<K,D>* n = _k;
	Node<K,D>* o = _k;

	while (n != 0 && key != n->_key){
		o = n;
		if ( key < n->_key && n->_left != 0){
			n = n->_left->_k;
		}else if (n->_right != 0){
			n = n->_right->_k;
		}else{
			n = 0;
		}
	}
	//return n; this is either the node containing the key, or the should be parent
	return ( n == 0 ? o : n);
}

//rotate
template<class K,class D>
void RBTree<K,D>::rotRight(){
	Node<K,D>* down = _k;
	Node<K,D>* up = _k->_left->_k;
	down->_left->_k = up->_right->_k;
	up->_right->_k = down;
	_k = up;
	down = 0;
	up = 0;
}
template<class K,class D>
void RBTree<K,D>::rotLeft(){
	Node<K,D>* down = _k;
	Node<K,D>* up = _k->_right->_k;
	down->_right->_k = up->_left->_k;
	up->_left->_k = down;
	_k = up;
	down = 0;
	up = 0;
}

//top down add
template<class K, class D>
void RBTree<K,D>::add(const K& key, const D& data){
	addNode(this,key,data,0);
	_k->_isBlack = true;
}
template<class K, class D>
void RBTree<K,D>::addNode(RBTree<K,D>* h, const K& key, const D& data, int sw){
	//tree empty
	if (h->_k == 0){ 
		h->_k = new Node<K,D>(key,data,true); 
		return; 
	}
	
	//2 red children => make me red & them black
	if ( h->_k->_left->isRed() && h->_k->_right->isRed() ){
		h->_k->_isBlack = false;
		h->_k->_left->_k->_isBlack = true;
		h->_k->_right->_k->_isBlack = true;
	}

	//fixes
    if(key < h->_k->_key){ //left
        addNode( h->_k->_left, key, data, 0);
        if(h->isRed() && h->_k->_left->isRed() && sw) 
        	h->rotRight();
        if(h->_k->_left->isRed() && h->_k->_left->_k->_left->isRed()){
            h->rotRight(); 
            h->_k->_isBlack=true; 
            h->_k->_right->_k->_isBlack = false;
        }
    }else{ //right
        addNode( h->_k->_right, key, data, 1);
        if( !h->isRed() && h->_k->_right->isRed() && !sw )
        	h->rotLeft();
        if( h->_k->_right->isRed() && h->_k->_right->_k->_right->isRed() ){
            h->rotLeft(); 
            h->_k->_isBlack = true;
            h->_k->_left->_k->_isBlack = false;
        }
    }

}


//top down delete

#endif