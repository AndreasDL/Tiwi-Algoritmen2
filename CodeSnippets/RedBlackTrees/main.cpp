#include <iostream>
#include <vector>
#include "RBTree.h"
using namespace std;

int main(int argc, char const *argv[]){
	Node<int,int>* n = new Node<int,int>(10,1,true);
	Node<int,int>* m = new Node<int,int>(20,1,true);
	cout << *n << endl << *m << endl
		 << "Grootste: " << (m>n ? " m Wint" : " n wint!") << endl;

	RBTree<int,int>* t = new RBTree<int,int>();
	cout << t->search(10) << endl;
	
	t->add(10,100);
	t->add(11,110);
	t->add(7,70);

	cout << t->search(10) << endl;
	cout << t->search(20) << endl;
	cout << t->search(7) << endl;

	return 0;
}