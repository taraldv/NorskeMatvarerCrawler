#ifndef QMEIERIENE_H
#define QMEIERIENE_H

#include "QMeieriene.h"
#include "Nettside.h"

class QMeieriene : public Nettside {
  private:
    bool stringCheck(std::string s);

  public:
    QMeieriene();
};

#endif