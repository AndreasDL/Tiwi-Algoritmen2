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
        Regexp re(s);
        cout<<s<<'\n'<<re<<'\n';
    }
    catch(Taalexceptie &te){
        cerr<<te.what()<<'\n';
    }
    return 0;
}
