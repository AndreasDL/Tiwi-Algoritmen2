#include "regexp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
using namespace std;

int main(int argc, char *argv[]){
    string s;
    cout<<"geef regexp:\n";
    cin>>s;
    cerr<<"string:"<<s<<":\n";
    try{
        //inlezen
		Regexp re(s);
        cout<<s<<'\n'<<re<<'\n';
        
        //graaf maken
        
        //overlopen
        
    }
    catch(Taalexceptie &te){
        cerr<<te.what()<<'\n';
    }
    return 0;
}
