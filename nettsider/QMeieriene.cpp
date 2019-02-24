#include "QMeieriene.h"

using namespace std;

QMeieriene::QMeieriene() {
	base = "q-meieriene";
	baseURL = "www.q-meieriene.no";
    newLinks.push_back("https://www.q-meieriene.no/");
    newLinks.push_back("https://www.tine.no/merkevarer/cultura");
}

/* fjerner URL som inneholder disse ordene */
bool QMeieriene::stringCheck(string s) {
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

