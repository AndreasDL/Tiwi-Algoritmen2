#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include "graaf.h"
int main(int argc, char *argv[]){
    const char* knoopnamen[]={
    "nul","een","twee","drie","vier"};
    int aantal=sizeof(knoopnamen)/sizeof(char*);
    GraafMetKnoopdata<ONGERICHT,const char*> g(knoopnamen,knoopnamen+aantal);
    g.voegVerbindingToe(0,1);
    g.voegVerbindingToe(1,2);
    g.voegVerbindingToe(2,3);
    g.voegVerbindingToe(1,3);
    std::cerr<<g<<'\n';
    GraafMetTakdata<GERICHT,std::string> gg(5);
    gg.voegVerbindingToe(0,1);
    gg.voegVerbindingToe(1,2,"hallo");
    gg.voegVerbindingToe(2,3);
    gg.voegVerbindingToe(1,3,"wereld");
    std::cerr<<gg<<'\n';
    
    return 0;
}
