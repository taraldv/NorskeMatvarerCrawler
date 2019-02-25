#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "http/Table.h"
#include "nettsider/QMeieriene.h"
#include "nettsider/Tine.h"

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

    /*vector<string> visited = q.getvisitedLinks();
    vector<string> newLinks = q.getNewLinks();
    writeVector(visited, "visited.txt");
    writeVector(newLinks, "new.txt");*/

    Tine t;
    t.runCrawler(5);
    vector<Table> tables = t.getTables();
    cout << "Tables: " << tables.size() << endl;
    writeTables(tables, "qTabell.txt");

    /* QMeieriene q;
       q.runCrawler(5);
       vector<Table> tables = q.getTables();
       cout << "Tables: " << tables.size() << endl;
       writeTables(tables, "qTabell.txt");*/

    return 0;
}
