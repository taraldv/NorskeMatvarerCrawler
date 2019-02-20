#ifndef TINE_H
#define TINE_H

#include "Request.h"
#include <iostream>

class Tine: public Request{
private:
	std::vector<std::string> visitedLinks;
	std::vector<std::string> newLinks;
	bool alreadyVisited(std::string url);
public:
	Tine();
	void runCrawler(int iterations);
	std::vector<std::string> getvisitedLinks();
	std::vector<std::string> getNewLinks();
	void getTables();
};

#endif