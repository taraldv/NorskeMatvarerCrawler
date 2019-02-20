#include "Tine.h"

using namespace std;


/* sender url til Request konstruktor */
Tine::Tine(){
	newLinks.push_back("https://www.tine.no/");
}

bool Tine::alreadyVisited(string url){
	for(size_t x = 0;x<visitedLinks.size();x++){
		if(visitedLinks.at(x).compare(url)){
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

/* må visst had https i tillegg */
void addHTTPS(vector<string>&vektorAlias){
	for(size_t i = 0;i<vektorAlias.size();i++){
		/* hvis https ikke finnes i string */
		if(vektorAlias.at(i).find("https")==string::npos){
			vektorAlias.at(i) = "https://"+vektorAlias.at(i);
		}
	}
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


void Tine::runCrawler(size_t numberOfIterations){
	for(size_t i=0;i<numberOfIterations;i++){
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
			//xmlNodeSetPtr set = getRegexNodes();
			visitedLinks.push_back(tempURL);
			newLinks.pop_back();

			vector<string> newURLs = getContentFromNodeSet(set);

			/* fikser links før de legges til sum*/
			removeDuplicateStringsFromVector(newURLs);
			fiksURLs(newURLs);
			onlyKeepUsefulTineLinks(newURLs);
			addHTTPS(newURLs);
			sumNyeLinks.insert(sumNyeLinks.end(),newURLs.begin(),newURLs.end());
		}
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