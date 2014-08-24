#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> getPrefix(string s){
	int m = s.size();
	vector<int> a(m);
	a[0] = 0;
	int k = 0;
	for (int q = 1 ; q < m ; q++){
		while( k > 0 && p[k] != q[k] ){
			k = a[k];
		}
		if (p[k] == q[k]){
			k++;
		}
		a[q] = k;
	}
}

int int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}