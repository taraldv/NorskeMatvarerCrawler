#ifndef MENY_H
#define MENY_H

#include "Nettside.h"
#include <iostream>
#include <iterator>
#include <string>
#include <regex>

class Meny : public Nettside {
  private:
    bool stringCheck(std::string s);
    std::vector<std::string *> getTableData(Parser *parser);
	std::string *getTableRowCellContent(xmlNode *row);

  public:
    Meny();
    char *getTitle(Parser *parser);
};

#endif