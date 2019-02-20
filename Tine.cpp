#include "Tine.h"

using namespace std;


/* sender url til Request konstruktor */
Tine::Tine(){
	newLinks.push_back("http://www.tine.no");
}

bool Tine::alreadyVisited(string url){
	for(size_t x = 0;x<visitedLinks.size();x++){
		/* compare returnerer 0 hvis like seff */
		if(!url.compare(visitedLinks.at(x))){
			return true;
		}
	}
	return false;
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
	"hjelp-og-brukerstotte","sunt-kosthold","fagprat","aktuelt","helse","om-oss","oppskrifter","medlem","english","sponsing","smartidrettsmat"};
	for(size_t i=0;i<sizeof(invalidWords)/sizeof(*invalidWords);i++){
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


void Tine::getTables(){
	for(size_t i = 0;i<newLinks.size();i++){
		string tempURL = newLinks.at(i);
		xmlNodeSetPtr set = getRegexNodes(tableRegex,tempURL);
		if(set){
			cout << (xmlNode*)set.type << endl;
			//const xmlNode *node = set->nodeTab[0]->xmlChildrenNode;
			//string tempString = (string)((char*)node->content);
			//cout << tempString << endl;
		} else {
			cout << "didnt find table?" << endl;
		}
	}
}

void Tine::runCrawler(int numberOfIterations){
	for(int i=0;i<numberOfIterations;i++){
		vector<string> sumNyeLinks;
		while(!newLinks.empty()){
			string tempURL = newLinks.back();
			/* sletter og går til neste link hvis den allerede er besøkt */
			if(alreadyVisited(tempURL)){
				newLinks.pop_back();
				continue;
			}
			/* hvis ikke besøkt, henter html dokument, legges til besøkt liste og sletter */
			xmlNodeSetPtr set = getRegexNodes(urlRegex,tempURL);
			visitedLinks.push_back(tempURL);
			newLinks.pop_back();

			vector<string> newURLs = getContentFromNodeSet(set);

			/* fikser links før de legges til sum*/
			removeDuplicateStringsFromVector(newURLs);
			fiksURLs(newURLs);
			onlyKeepUsefulTineLinks(newURLs);
			sumNyeLinks.insert(sumNyeLinks.end(),newURLs.begin(),newURLs.end());
		}
		removeDuplicateStringsFromVector(sumNyeLinks);
		/* her skal newLinks være tom, så vi legger til sumNyeLinks */
		newLinks.insert(newLinks.end(),sumNyeLinks.begin(),sumNyeLinks.end());
	}
}

vector<string> Tine::getvisitedLinks(){
	return visitedLinks;
};
vector<string> Tine::getNewLinks(){
	return newLinks;
};