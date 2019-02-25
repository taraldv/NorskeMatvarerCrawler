#include "Meny.h"

using namespace std;

Meny::Meny() {
    base = "meny";
    baseURL = "meny.no";
    newLinks.push_back("https://meny.no/varer");
    newLinks.push_back("https://meny.no/");
    newLinks.push_back(
        "https://meny.no/varer/bakeri/hamburgerbrod/burgerbrod-7041614000829");
}

/* fjerner URL som inneholder disse ordene */
bool Meny::stringCheck(string s) {
    string invalidWords[] = {"instagram",   "Om-MENY",  "twitter",
                             "oppskrifter", "facebook", "pinterest"};
    for (size_t i = 0; i < sizeof(invalidWords) / sizeof(*invalidWords); i++) {
        if (s.find(invalidWords[i]) != string::npos) {
            return true;
        }
    }
    return false;
}

string *Meny::getTableRowCellContent(xmlNode *row) {
    /* heap alloc */
    string *data = new string[2];

    int index = 0;
    for (xmlNode *rowTDNode = row->children; rowTDNode;
         rowTDNode = rowTDNode->next) {
        if (rowTDNode->type == 1 && index < 2) {
            data[index] = (char *)xmlNodeGetContent(rowTDNode);
            index++;
        }
    }
    return data;
}

void parseJJSON(char *json) {
    string s = (string)json;

    /* ved hjelp av https://regexr.com/ C++ bruker ECMA(javascript) */
    std::regex word_regex("(\"nutritionalContent\":\\[)([^\\]]*)");
    auto words_begin = std::sregex_iterator(s.begin(), s.end(), word_regex);
    smatch match = *words_begin;
    string matchString = match.str();
    string validJSON = matchString.substr(22);
    /* validJSON skal nå være en samling av JSON objekter hvor alle har keys:
            name,unit,amount og displayName*/
    cout << matchString << endl;
    cout << endl;
    cout << validJSON << endl;
}

vector<string *> Meny::getTableData(Parser *parser) {
    xmlNodeSetPtr menyData =
        parser->getRegexNodes((xmlChar *)"//div/@data-prop-product");
    vector<string *> vArr;
    if (menyData) {
        xmlNodePtr *rowArray = menyData->nodeTab;
        if (rowArray) {
            char *json = (char *)xmlNodeGetContent(rowArray[0]);
            parseJJSON(json);
            // cout << json << endl;
        } /*for (int i = 0; i < menyData->nodeNr; i++) {
             //cout << xmlNodeGetContent(rowArray[i]) << endl;
             // xmlNode *rowNode = ->xmlChildrenNode;
           










             string *k = getTableRowCellContent(rowArray[i]);
             if (k) {
                 // cout << k[0] << endl;
                 // cout << k[1] << endl;
                 vArr.push_back(k);
             }
         }*/
    }
    xmlXPathFreeNodeSet(menyData);
    return vArr;
}

/* returnerer NULL hvis tittel ikke finnes */
char *Meny::getTitle(Parser *parser) {
    xmlNodeSetPtr titleDiv =
        parser->getRegexNodes((xmlChar *)"//div[@class='cw-product__over']");
    if (titleDiv) {
        xmlNodePtr *arr = titleDiv->nodeTab;
        if (arr) {
            xmlNode *h1Node = arr[0];
            string temp = "";
            if (h1Node) {
                char *h1NodeTekst = (char *)xmlNodeGetContent(h1Node);
                if (h1NodeTekst) {
                    xmlXPathFreeNodeSet(titleDiv);
                    return h1NodeTekst;
                }
            }
        }
    }
    return NULL;
}