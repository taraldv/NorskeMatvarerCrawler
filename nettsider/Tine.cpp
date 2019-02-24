#include "Tine.h"

using namespace std;

Tine::Tine() {
    base = "tine";
    baseURL = "www.tine.no";

    newLinks.push_back("http://www.tine.no");
    newLinks.push_back("https://www.tine.no/merkevarer/cultura");
}

/* fjerner URL som inneholder disse ordene */
bool Tine::stringCheck(string s) {
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