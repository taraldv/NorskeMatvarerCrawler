#include <stdio.h>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "Request.h"
#include "Tine.h"

using namespace std;

void writeVector(vector<string> vektor,string filNavn){
	ofstream outputFile;
	outputFile.open(filNavn);
	for(unsigned int i=0;i<vektor.size();i++){
		outputFile << vektor.at(i) << endl;
	}
	outputFile.close();
}

void removeUselessURLS(vector<string>&vektorAlias){
  /* erase tar bare iterator? så brukte det istedet for size() */
	for(vector<string>::iterator it = vektorAlias.end()-1; it!=vektorAlias.begin()-1;it--){
		string tempUrl = *it;
		if(tempUrl.find("produkter")==string::npos){
			vektorAlias.erase(it);
		}
	}
}

int main(void){
	Tine t;
	t.runCrawler(3);

	vector<string> links = t.getLinks();
	//cout << vektor.size() << endl;
	writeVector(links,"all.txt");
	//t.nyTest();

/*
  char *baseURL = (char*)"https://www.tine.no/produkter";

  xmlNodeSetPtr initialNodeSet = getRegexNodes((xmlChar*)"//a/@href",baseURL);
  vector<string> big = getURLsFromVector(getURLsFromNodeSet(initialNodeSet));

  removeUselessURLS(big);
 

  writeVector(big,"urls.txt");

  for(unsigned int i=0;i<big.size();i++){
    xmlNodeSetPtr tableNode = getRegexNodes((xmlChar*)"//table",&big.at(i)[0u]);
    cout << tableNode << endl;
  }

  curl_global_cleanup();
*/
	return 0;
}

