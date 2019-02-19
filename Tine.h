#ifndef TINE_H
#define TINE_H

#include "Request.h"
#include <iostream>

class Tine: public Request{
private:
	xmlNodeSetPtr initalNodeSet;
	std::vector<xmlNodeSetPtr> secondIterationNodeSet;
	std::vector<xmlNodeSetPtr> thirdIterationNodeSet;
public:
	Tine();
	std::vector<std::string> getInitialNodeList();
	std::vector<std::string> getSecondIterationNodeList();
	std::vector<std::string> getThirdIterationNodeList();
};

#endif