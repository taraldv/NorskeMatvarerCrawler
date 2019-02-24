#include "Table.h"

using namespace std;

string Table::fixString(string s) {
    const char *search = "()abcdefghijklmnopqrstuvwxyzøæå0123456789-";
    size_t startIndex = s.find_first_of(search);
    size_t lastIndex = s.find_last_of(search);
    if (startIndex != string::npos && lastIndex != string::npos) {
        return s.substr(startIndex, lastIndex + 1 - startIndex);
    } else {
        return "";
    }
}

Table::Table(std::string title, std::vector<std::string *> data)
    : title(title) {
    for (size_t i = 0; i < data.size(); i++) {
        string *arr = data.at(i);
        if (arr && !arr[0].empty() && !arr[1].empty()) {
            string dataString = fixString(arr[1]);
            string forkString = fixString(arr[0]);
            if (!dataString.empty() && !forkString.empty()) {
                dataVektor.push_back(dataString);
                forkVektor.push_back(forkString);
            }
        }
        delete[] arr;
    }
};

vector<string> Table::getDataVektor() { return dataVektor; };

vector<string> Table::getForkVektor() { return forkVektor; };

string Table::getTitle() { return title; }