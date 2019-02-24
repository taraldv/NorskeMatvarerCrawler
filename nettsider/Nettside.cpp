#include "Nettside.h"

using namespace std;

bool Nettside::alreadyVisited(string url) {
    for (size_t x = 0; x < visitedLinks.size(); x++) {
        /* compare returnerer 0 hvis like */
        if (!url.compare(visitedLinks.at(x))) {
            return true;
        }
    }
    return false;
}

vector<string> Nettside::getContentFromNodeSet(xmlNodeSetPtr set) {
    vector<string> vektor;
    if (set) {
        for (int i = 0; i < set->nodeNr; i++) {
            const xmlNode *node = set->nodeTab[i]->xmlChildrenNode;
            string tempString = (string)((char *)node->content);
            vektor.push_back(tempString);
        }
    }
    return vektor;
}

/*
        2 typer feil:
        relative urls som mangler www.tine.no fremst
        urls som ender opp med to // fremst
*/
void Nettside::fiksURLs(vector<string> &vektorAlias) {
    for (size_t i = 0; i < vektorAlias.size(); i++) {
        string tempString = vektorAlias.at(i);
        try {
            if (relativeURL(tempString)) {
                vektorAlias.at(i) = baseURL + tempString;
            } else if (malformedURL(tempString)) {
                vektorAlias.at(i) = tempString.substr(2);
            }
        } catch (const out_of_range &oor) {
            continue;
        }
    }
}

/* fjerner alle URL som IKKE har tine i seg || stringCheck */
void Nettside::onlyKeepUsefulLinks(vector<string> &vektorAlias) {
    for (vector<string>::iterator it = vektorAlias.end() - 1;
         it != vektorAlias.begin() - 1; it--) {
        string tempUrl = *it;
        if (tempUrl.find(base) == string::npos || stringCheck(tempUrl)) {
            vektorAlias.erase(it);
        }
    }
}

bool Nettside::relativeURL(string url) { return (url.at(0) == '/' && url.at(1) != '/'); }

bool Nettside::malformedURL(string url) { return (url.at(0) == '/' && url.at(1) == '/'); }

/* returnerer NULL hvis tittel ikke finnes */
char* Nettside::getTitle(Parser parser) {
    xmlNodeSetPtr h1Title =
        parser.getRegexNodes((xmlChar *)"//h1[@class='title']");
    if (h1Title) {
        xmlNodePtr *arr = h1Title->nodeTab;
        if (h1Title->nodeNr) {
            xmlNode *titleNode = arr[0]->xmlChildrenNode;
            /* xmlNodeGetContent kan også bli NULL */
            return (char *)xmlNodeGetContent(titleNode);
        }
    }
    return NULL;
}

string *Nettside::getTableRowCellContent(xmlNode *row) {
    /* heap alloc */
    string *data = new string[2];
    xmlNode *rowTDNode = row->children;
    // skal printe tr
    // cout << row->name << endl;
    int index = 0;
    while (rowTDNode != row->last) {
        if (rowTDNode->type == 1 && index < 2) {
            data[index] = (char *)xmlNodeGetContent(rowTDNode);
            index++;
        }
        rowTDNode = rowTDNode->next;
    }
    delete row;
    return data;
}

vector<string *> Nettside::getTableData(Parser parser) {
    xmlNodeSetPtr tableRows =
        parser.getRegexNodes((xmlChar *)"//tr[@class='nutrient-table__row']");
    vector<string *> vArr;
    if (tableRows) {
        xmlNodePtr *rowArray = tableRows->nodeTab;
        for (int i = 0; i < tableRows->nodeNr; i++) {
            // xmlNode *rowNode = ->xmlChildrenNode;
            /* string arr med størrelse 2, forklaring og verdi */
            string *k = getTableRowCellContent(rowArray[i]);
            if (k) {
                vArr.push_back(k);
            }
            // cout << k[0] << endl;
            // cout << k[1] << endl;
            //delete[] k;
        }
        delete rowArray;
    }
    delete tableRows;
    return vArr;
}

vector<Table> Nettside::getTables() {
    cout << newLinks.size() << endl;
    vector<Table> tables;
    for (size_t i = 0; i < newLinks.size(); i++) {
        string tempURL = newLinks.at(i);
        Request req(tempURL);
        htmlDocPtr doc = req.getXMLDoc();
        if (doc) {
            Parser par(doc);
            char *text = getTitle(par);

			/* sjekker om siden har en tittel */
            if (text) {
                string tittel = (string)text;
                delete text;
                vector<string *> data = getTableData(par);

				/* sjekker om siden har en tabell */
                if (data.size() > 0) {
                    Table table(tittel, data);
                    tables.push_back(table);
                }
            }
        }
    }
    return tables;
}

void Nettside::removeDuplicateStrings(vector<string> &vektorAlias) {
    sort(vektorAlias.begin(), vektorAlias.end());
    vektorAlias.erase(unique(vektorAlias.begin(), vektorAlias.end()),
                      vektorAlias.end());
}

void Nettside::runCrawler(int numberOfIterations) {
    for (int i = 0; i < numberOfIterations; i++) {
        vector<string> sumNyeLinks;
        while (!newLinks.empty()) {
            string tempURL = newLinks.back();
            /* sletter og går til neste link hvis den allerede er besøkt */
            if (alreadyVisited(tempURL)) {
                newLinks.pop_back();
                continue;
            }
            /* hvis ikke besøkt, henter html dokument, legges til besøkt liste
             * og sletter */
            Request req(tempURL);
            htmlDocPtr doc = req.getXMLDoc();
            Parser par(doc);
            xmlNodeSetPtr set = par.getRegexNodes((xmlChar *)"//a/@href");
            visitedLinks.push_back(tempURL);
            newLinks.pop_back();

            vector<string> newURLs = getContentFromNodeSet(set);

            /* fikser links før de legges til sum*/
            removeDuplicateStrings(newURLs);
            fiksURLs(newURLs);
            onlyKeepUsefulLinks(newURLs);
            sumNyeLinks.insert(sumNyeLinks.end(), newURLs.begin(),
                               newURLs.end());
        }
        removeDuplicateStrings(sumNyeLinks);
        /* her skal newLinks være tom, så vi legger til sumNyeLinks */
        newLinks.insert(newLinks.end(), sumNyeLinks.begin(), sumNyeLinks.end());
    }
}

vector<string> Nettside::getvisitedLinks() { return visitedLinks; };
vector<string> Nettside::getNewLinks() { return newLinks; };