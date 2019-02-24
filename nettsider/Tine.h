#ifndef TINE_H
#define TINE_H

#include "Nettside.h"

class Tine : public Nettside {
  private:
    bool stringCheck(std::string s);
    std::vector<std::string *> getTableData(Parser *parser);

  public:
    Tine();
    char *getTitle(Parser *parser);
};

#endif