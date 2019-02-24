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
    string invalidWords[] = {"Min-Qide", "Ernaering", "Nyheter", "Kontakt","inspirasjon","Ideer"
                             "Lag-Din-Skyr", "twitter" , "facebook" , "pinterest" ,"Oppskrifter","login"};
    for (size_t i = 0; i < sizeof(invalidWords) / sizeof(*invalidWords); i++) {
        if (s.find(invalidWords[i]) != string::npos) {
            return true;
        }
    }
    return false;
}
