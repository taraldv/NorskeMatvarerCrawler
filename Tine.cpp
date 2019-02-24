#include "Tine.h"

using namespace std;

Tine::Tine() {
    newLinks.push_back("http://www.tine.no");
    newLinks.push_back("https://www.tine.no/merkevarer/cultura");
}

bool Tine::alreadyVisited(string url) {
    for (size_t x = 0; x < visitedLinks.size(); x++) {
        /* compare returnerer 0 hvis like */
        if (!url.compare(visitedLinks.at(x))) {
            return true;
        }
    }
    return false;
}

vector<string> getContentFromNodeSet(xmlNodeSetPtr set) {
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

bool relativeURL(string url) { return (url.at(0) == '/' && url.at(1) != '/'); }

bool malformedURL(string url) { return (url.at(0) == '/' && url.at(1) == '/'); }

/*
        2 typer feil:
        relative urls som mangler www.tine.no fremst
        urls som ender opp med to // fremst
*/
void fiksURLs(vector<string> &vektorAlias) {
    for (size_t i = 0; i < vektorAlias.size(); i++) {
        string tempString = vektorAlias.at(i);
        try {
            if (relativeURL(tempString)) {
                vektorAlias.at(i) = "www.tine.no" + tempString;
            } else if (malformedURL(tempString)) {
                vektorAlias.at(i) = tempString.substr(2);
            }
        } catch (const out_of_range &oor) {
            continue;
        }
    }
}

/* fjerner URL som inneholder disse ordene */
bool stringCheck(string s) {
    string invalidWords[] = {"sporsmal",      "tine-handel",
                             "Driftsledelse", "tjenester",
                             "tine-ravare",   "nettsider",
                             "arsrapport",    "instagram",
                             "facebook",      "kontakt",
                             "presserom",     "twitter",
                             "pinterest",     "praktisk-informasjon",
                             "medlemsp",      "kalender",
                             "jobb-i-tine",   "hjelp-og-brukerstotte",
                             "sunt-kosthold", "fagprat",
                             "aktuelt",       "helse",
                             "om-oss",        "oppskrifter",
                             "medlem",        "english",
                             "sponsing",      "smartidrettsmat"};
    for (size_t i = 0; i < sizeof(invalidWords) / sizeof(*invalidWords); i++) {
        if (s.find(invalidWords[i]) != string::npos) {
            return true;
        }
    }
    return false;
}

/* fjerner alle URL som IKKE har tine i seg || stringCheck */
void onlyKeepUsefulTineLinks(vector<string> &vektorAlias) {
    for (vector<string>::iterator it = vektorAlias.end() - 1;
         it != vektorAlias.begin() - 1; it--) {
        string tempUrl = *it;
        if (tempUrl.find("tine") == string::npos || stringCheck(tempUrl)) {
            vektorAlias.erase(it);
        }
    }
}

/* returnerer NULL hvis tittel ikke finnes */
char *Tine::getTitle(Parser parser) {
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

string *Tine::getTableRowCellContent(xmlNode *row) {
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
	cout << "rows found: " << index << endl;
	delete row;
    return data;
}

vector<string *> Tine::getTableData(Parser parser) {
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
            // delete[] k;
        }
		delete rowArray;
    }
	delete tableRows;
    return vArr;
}

vector<Table> Tine::getTables() {
    cout << newLinks.size() << endl;
	vector<Table> tables;
    for (size_t i = 0; i < newLinks.size(); i++) {
        string tempURL = newLinks.at(i);
        Request req(tempURL);
        htmlDocPtr doc = req.getXMLDoc();
        if (doc) {
            Parser par(doc);
            char *text = getTitle(par);
            if (text) {
                string tittel = (string)text;
				delete text;
                vector<string *> data = getTableData(par);

                Table table (tittel, data);
				//cout << "table created" << endl;
                tables.push_back(table);
				//cout << "table push" << endl;
            }
        }
    }
	//cout << "noe galt her?";
   return tables;
}

/*void Tine::getTables(){
        for(size_t i = 0;i<newLinks.size();i++){
                string tempURL = newLinks.at(i);
                //cout << tempURL << " : ";
                htmlDocPtr doc = getXMLDocFromURL(tempURL);
                xmlNodeSetPtr set = getRegexNodes(urlRegex,doc);
                if(set){
                        if(set->nodeNr>0){
                                const xmlNode *tBody =
   set->nodeTab[0]->xmlChildrenNode; const xmlNode *tRow =
   tBody->xmlChildrenNode;

                                int rows = 0;
                                while(tRow!=tBody->last){
                                        cout << "rowNr: " << rows << endl;
                                        const xmlNode *tData =
   tRow->xmlChildrenNode; int tds = 0; while(tData!=tRow->last){ cout << "tdNr:
   " << tds << endl; cout << "type: " << tData->type << endl; const xmlNode
   *elem = (xmlNode*)tData; xmlChar* text = xmlNodeGetContent(elem); if(text){
                                                        string s =
   (string)((char*)text); if(s.length()>0){ cout << s << endl;
                                                        }
                                                }*/
/*cout << elem->type << endl;
xmlElementContent *stuff = elem->content;
if(stuff){
        cout << stuff->name << endl;
}*/
/*xmlChar* stuff = tData->content;
if(stuff){
        cout << (string)((char*)stuff) << endl;
}*/
/*tds++;
tData = tData->next;
}
rows++;
tRow = tRow->next;
}*/
// cout << "url: " << tempURL << endl;
// cout << "content: " << node->content << endl;
// cout << "type: " << node->type << endl;
// cout << "name: " << node->name << endl;
// string tempString = (string)((char*)node->content);
// cout << tempString << endl;
//}
// cout << set->nodeNr << endl;
// const xmlNode *node = set->nodeTab[0]->xmlChildrenNode;
// string tempString = (string)((char*)node->content);
// cout << tempString << endl;
/*} else {
        cout << "didnt find table?" << endl;
}
}
}*/

void removeDuplicateStringsFromVector(vector<string> &vektorAlias) {
    sort(vektorAlias.begin(), vektorAlias.end());
    vektorAlias.erase(unique(vektorAlias.begin(), vektorAlias.end()),
                      vektorAlias.end());
}

void Tine::runCrawler(int numberOfIterations) {
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
            removeDuplicateStringsFromVector(newURLs);
            fiksURLs(newURLs);
            onlyKeepUsefulTineLinks(newURLs);
            sumNyeLinks.insert(sumNyeLinks.end(), newURLs.begin(),
                               newURLs.end());
        }
        removeDuplicateStringsFromVector(sumNyeLinks);
        /* her skal newLinks være tom, så vi legger til sumNyeLinks */
        newLinks.insert(newLinks.end(), sumNyeLinks.begin(), sumNyeLinks.end());
    }
}

vector<string> Tine::getvisitedLinks() { return visitedLinks; };
vector<string> Tine::getNewLinks() { return newLinks; };