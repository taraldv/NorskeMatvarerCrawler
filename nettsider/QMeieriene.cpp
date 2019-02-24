#include "QMeieriene.h"

using namespace std;

QMeieriene::QMeieriene() {
    base = "q-meieriene";
    baseURL = "www.q-meieriene.no";
    newLinks.push_back("https://www.q-meieriene.no/");
    newLinks.push_back("https://www.q-meieriene.no/Produktkategorier/"
                       "Skyr-Original-islandsk-yoghurt/Skyr-Blaabaer");
}

/* fjerner URL som inneholder disse ordene */
bool QMeieriene::stringCheck(string s) {
    string invalidWords[] = {"Min-Qide",
                             "Ernaering",
                             "Nyheter",
                             "Kontakt",
                             "inspirasjon",
                             "Ideer"
                             "Lag-Din-Skyr",
                             "twitter",
                             "facebook",
                             "pinterest",
                             "Oppskrifter",
                             "login"};
    for (size_t i = 0; i < sizeof(invalidWords) / sizeof(*invalidWords); i++) {
        if (s.find(invalidWords[i]) != string::npos) {
            return true;
        }
    }
    return false;
}

vector<string *> QMeieriene::getTableData(Parser *parser) {
    xmlNodeSetPtr tableRows =
        parser->getRegexNodes((xmlChar *)"//tr[@class='nutrient-table__row']");
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
char *QMeieriene::getTitle(Parser *parser) {
    xmlNodeSetPtr titleDiv =
        parser->getRegexNodes((xmlChar *)"//div[@class='page-header']");
    if (titleDiv) {
        xmlNodePtr *arr = titleDiv->nodeTab;
        xmlNode *pageHeaderDiv = arr[0];
        if (pageHeaderDiv) {
            // cout << pageHeaderDiv->name << endl;
            xmlNode *children = pageHeaderDiv->children;
            string temp = "";
            while (children != pageHeaderDiv->last) {
                if (children->type == 1) {
                    cout << children->name << endl;
                    char *tekst = (char *)xmlNodeGetContent(children);
                    temp += (string)tekst;
					delete tekst;
                }
                children = children->next;
            }
			return &temp[0u];
        }
    }
    return NULL;
}