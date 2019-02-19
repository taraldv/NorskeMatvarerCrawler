#ifndef TINE_H
#define TINE_H

#include "Request.h"

class Tine: public Request{
private:
	xmlNodeSetPtr initalNodeSet;
public:
	Tine();
	//xmlNodeSetPtr initialAnchorNodeSet();
	std::vector<std::string> getInitialNodeList();
};

#endif