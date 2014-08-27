#include <iostream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <functional> //voor greater
#include "graaf.h"
#include "tinyxml.h"

using namespace std;

const int K = 200;

void bepaalBuren(const GraafMetKnoopEnTakdata<ONGERICHT,string,int>* graaf, set<int>& buren, const int knoopnr) {
	buren.insert(knoopnr);
	for (Graaf<ONGERICHT>::Knoop::const_iterator it = (*graaf)[knoopnr].cbegin(); it != (*graaf)[knoopnr].cend(); it++) {
		if ( buren.find(it->first) == buren.end() ) {
			bepaalBuren(graaf, buren, it->first);
		}
	}
}

GraafMetKnoopEnTakdata<ONGERICHT,string,int>* bepaalKSamenhangendeComponenten(
										const GraafMetKnoopEnTakdata<ONGERICHT,string,int>* graaf) {
	
	GraafMetKnoopEnTakdata<ONGERICHT,string,int>* componenten = new GraafMetKnoopEnTakdata<ONGERICHT,string,int>();
	typedef pair<int,pair<int,int> > P;
	priority_queue<P, vector<P>, greater<P> > verbindingen; //pair->first is de afstand; pair->second is pair met de 2 knoopindexen

	for (int i = 0; i < graaf->aantalKnopen(); i++) {
		componenten->voegKnoopToe(*(graaf->geefKnoopdata(i)));
		for (Graaf<ONGERICHT>::Knoop::const_iterator it = (*graaf)[i].cbegin(); it != (*graaf)[i].cend(); it++) {
			if (i < it->first) { //maar 1 keer een verbinding toevoegen
				int afstand = *(graaf->geefTakdata(i, it->first));
				pair<int,int> knoopindexen(i, it->first);
				P paar(afstand, knoopindexen);
				verbindingen.push(paar);
			}
		}
	}

	int i = graaf->aantalKnopen();
	while ( i > K ) { //pas Kruskal toe tot er K componenten zijn
		int van		= verbindingen.top().second.first;
		int naar	= verbindingen.top().second.second;
		int afstand = verbindingen.top().first;
		verbindingen.pop();
		set<int> buren;
		bepaalBuren(componenten, buren, van);
		if ( buren.find(naar) == buren.end() ) {	
			componenten->voegVerbindingToe(van, naar, afstand);
			i--;
		}
	}

	return componenten;
}

void main() {
	TiXmlDocument doc( "zoogdieren.xml" );
    bool loadOkay = doc.LoadFile();
    assert(loadOkay);
    TiXmlNode* netwerk;
    netwerk = doc.RootElement();
    assert( netwerk );

	map<string,int> knopen;
	map<string,string> namen;
	GraafMetKnoopEnTakdata<ONGERICHT,string,int>* graaf = new GraafMetKnoopEnTakdata<ONGERICHT,string,int>();
    for (TiXmlNode* knooppunt=netwerk->FirstChild(); knooppunt!=0; knooppunt=knooppunt->NextSibling()){
		string id = dynamic_cast<TiXmlElement*>(knooppunt)->Attribute("ID");
		if (knopen.find(id) == knopen.end()) {
			knopen[id] = graaf->voegKnoopToe(id);
		}
			namen[id]  = dynamic_cast<TiXmlElement*>(knooppunt)->Attribute("name");
        for(TiXmlNode* route=knooppunt->FirstChild(); route != 0; route=route->NextSibling()){
			string idNaar = dynamic_cast<TiXmlElement*>(route)->Attribute("species");
			if (knopen.find(idNaar) == knopen.end()) {
				knopen[idNaar] = graaf->voegKnoopToe(idNaar);
			}
			int distance = atoi(dynamic_cast<TiXmlElement*>(route)->Attribute("distance"));
			if (graaf->verbindingsnummer(knopen[id], knopen[idNaar]) == -1) {
				graaf->voegVerbindingToe(knopen[id], knopen[idNaar], distance);
			}
        }
    }

	GraafMetKnoopEnTakdata<ONGERICHT,string,int>* componentengraaf = bepaalKSamenhangendeComponenten(graaf);

	string soortnaam = "(modern humans)";
	int i = 0;
	int knoopnr = -1;
	while ( i < componentengraaf->aantalKnopen() && knoopnr == -1 ) {
		if ( namen[*(componentengraaf->geefKnoopdata(i))] != soortnaam ) {
			i++;
		} else {
			knoopnr = i;
		}
	}
	if ( knoopnr != -1 ) {
		cout << "De soort '" << soortnaam << "' werd geclassificeerd samen met:" << endl;
		set<int> buren;
		bepaalBuren(componentengraaf, buren, knoopnr);
		buren.erase(knoopnr);
		for (set<int>::const_iterator it = buren.cbegin(); it != buren.cend(); it++) {
			cout << "\t" << namen[*(componentengraaf->geefKnoopdata(*it))] << endl;
		}
	}

	delete componentengraaf;
	delete graaf;

	cin.get();
}