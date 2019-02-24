#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "Request.h"
#include "Table.h"
#include "Tine.h"

using namespace std;

void writeVector(vector<string> vektor, string filNavn) {
    ofstream outputFile;
    outputFile.open(filNavn);
    for (size_t i = 0; i < vektor.size(); i++) {
        outputFile << vektor.at(i) << endl;
    }
    outputFile.close();
}

void writeTables(vector<Table> tables, string filNavn) {
    ofstream outputFile;
    outputFile.open(filNavn);
    for (size_t i = 0; i < tables.size(); i++) {
        vector<string> data = tables.at(i).getDataVektor();
        vector<string> fork = tables.at(i).getForkVektor();
        outputFile << "tittel: " << tables.at(i).getTitle() << endl;
        for (size_t j = 0; j < data.size(); j++) {
            outputFile << fork.at(j) << ": " << data.at(j) << endl;
        }
		outputFile << endl;
    }
    outputFile.close();
}

int main(void) {
    Tine t;
    //t.runCrawler(1);
    vector<Table> tables = t.getTables();
	cout << tables.size() << endl;
    writeTables(tables, "tinetabell.txt");

     //vector<string> visited = t.getvisitedLinks();
     //vector<string> newLinks = t.getNewLinks();
    // cout << vektor.size() << endl;
    // writeVector(visited,"2IterasjonVisited.txt");
    // writeVector(newLinks,"2IterasjonNewLinks.txt");

    /*
      char *baseURL = (char*)"https://www.tine.no/produkter";

      xmlNodeSetPtr initialNodeSet =
     getRegexNodes((xmlChar*)"//a/@href",baseURL); vector<string> big =
     getURLsFromVector(getURLsFromNodeSet(initialNodeSet));

      removeUselessURLS(big);
     





      writeVector(big,"urls.txt");

      for(unsigned int i=0;i<big.size();i++){
        xmlNodeSetPtr tableNode =
     getRegexNodes((xmlChar*)"//table",&big.at(i)[0u]); cout << tableNode <<
     endl;
      }

      curl_global_cleanup();
    */
    return 0;
}
