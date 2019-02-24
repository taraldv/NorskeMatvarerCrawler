#ifndef QMEIERIENE_H
#define QMEIERIENE_H

#include "Nettside.h"
#include "QMeieriene.h"

class QMeieriene : public Nettside {
  private:
    bool stringCheck(std::string s);
    std::vector<std::string *> getTableData(Parser *parser);

  public:
    QMeieriene();
    char *getTitle(Parser *parser);
};

#endif