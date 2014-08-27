#include <iostream>
#include <string>
#include <queue>
#include "shiftand.h"

using namespace std;

int main(){
    string s = "kakad";

    const uchar *c = (uchar *) s.c_str();

    Shiftand search(c, (uint) s.length());

    queue<const uchar*> plaats;
    string t = "kakakad";
    const uchar* tekst = (uchar*) t.c_str();

    search.zoek(plaats, tekst, uint(t.length()));

    cout<<endl<<"----------------------------------------"<<endl;
    cout<<"Te zoeken patroon\t: "<<c<<endl;
    cout<<"Resultaten: "<<endl;

    while(!plaats.empty()){
        const uchar* gevonden = plaats.front();
        plaats.pop();
        cout<<"\t\t\t> "<<gevonden<<endl;
    }

}
