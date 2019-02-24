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

