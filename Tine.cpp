#include "Tine.h"

using namespace std;


/* sender url til Request konstruktor */
Tine::Tine():Request((char*)"https://www.tine.no/"){
	initalNodeSet = getRegexNodes((xmlChar*)"//a/@href",url);
	vector<string> initialVektor = getInitialNodeList();
	for(size_t i = 0; i<initialVektor.size();i++){
		secondIterationNodeSet.push_back(getRegexNodes((xmlChar*)"//a/@href",&initialVektor.at(i)[0u]));
	}
	vector<string> secondVektor = getSecondIterationNodeList();
	for(size_t j = 0; j<secondVektor.size();j++){
		thirdIterationNodeSet.push_back(getRegexNodes((xmlChar*)"//a/@href",&secondVektor.at(j)[0u]));
	}
}

bool relativeURL(string url){
	return (url.at(0) == '/' && url.at(1) != '/');
}

bool malformedURL(string url){
	return (url.at(0)== '/' && url.at(1) == '/');
}

/* 
	2 typer feil:
	relative urls som mangler www.tine.no fremst
	urls som ender opp med to // fremst
*/
void fiksURLs(vector<string>&vektorAlias){
	for(size_t i = 0;i<vektorAlias.size();i++){
		string tempString = vektorAlias.at(i);
		try{
			if(relativeURL(tempString)){
				vektorAlias.at(i) = "www.tine.no"+tempString;
			} else if(malformedURL(tempString)){
				vektorAlias.at(i) = tempString.substr(2);
			}
		}catch(const out_of_range& oor){
			continue;
		}
	}
}

/* fjerner URL som inneholder disse ordene */
bool stringCheck(string s){
	string invalidWords[] = {"sporsmal","tine-handel","Driftsledelse","tjenester","tine-ravare","nettsider","arsrapport",
"instagram","facebook","kontakt","presserom","twitter","pinterest","praktisk-informasjon","medlemsp","kalender","jobb-i-tine",
"hjelp-og-brukerstotte","sunt-kosthold","fagprat","aktuelt","helse","om-oss","oppskrifter","medlem"};
	for(int i=0;i<sizeof(invalidWords)/sizeof(*invalidWords);i++){
		if(s.find(invalidWords[i])!=string::npos){
			return true;
		}
	}
	return false;
}

/* fjerner alle URL som IKKE har tine i seg || stringCheck */
void onlyKeepUsefulTineLinks(vector<string>&vektorAlias){
	for(vector<string>::iterator it = vektorAlias.end()-1; it!=vektorAlias.begin()-1;it--){
		string tempUrl = *it;
		if(tempUrl.find("tine")==string::npos || stringCheck(tempUrl)){
			vektorAlias.erase(it);
		}
	}
}

/* henter en vektor med URLS for hver URL i initialList */
vector<string> Tine::getThirdIterationNodeList(){
	vector<string> v;
	for(size_t i=0;i<thirdIterationNodeSet.size();i++){
		vector<string> tempVektor = getContentFromNodeSet(thirdIterationNodeSet.at(i));
		v.insert(v.end(),tempVektor.begin(),tempVektor.end());
	}
	removeDuplicateStringsFromVector(v);
	fiksURLs(v);
	onlyKeepUsefulTineLinks(v);
	return v;
}

/* henter en vektor med URLS for hver URL i initialList */
vector<string> Tine::getSecondIterationNodeList(){
	vector<string> v;
	for(size_t i=0;i<secondIterationNodeSet.size();i++){
		vector<string> tempVektor = getContentFromNodeSet(secondIterationNodeSet.at(i));
		v.insert(v.end(),tempVektor.begin(),tempVektor.end());
	}
	removeDuplicateStringsFromVector(v);
	fiksURLs(v);
	onlyKeepUsefulTineLinks(v);
	return v;
}

vector<string> Tine::getInitialNodeList(){
	vector<string> v = getContentFromNodeSet(initalNodeSet);
	removeDuplicateStringsFromVector(v);
	fiksURLs(v);
	onlyKeepUsefulTineLinks(v);
	return v;
}
//void Tine::fjern