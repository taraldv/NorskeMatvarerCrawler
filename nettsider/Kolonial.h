#ifndef KOLONIAL_H
#define KOLONIAL_H

#include "Nettside.h"

class Kolonial : public Nettside {
  private:
    bool stringCheck(std::string s);
    std::vector<std::string *> getTableData(Parser *parser);

  public:
    Kolonial();
    char *getTitle(Parser *parser);
};

#endif