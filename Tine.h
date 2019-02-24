#ifndef TINE_H
#define TINE_H

#include "Nettside.h"

class Tine : public Nettside {
  private:
    bool stringCheck(std::string s);

  public:
    Tine();
};

#endif