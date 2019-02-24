#ifndef TINE_H
#define TINE_H

#include "Request.h"
#include "Parser.h"
#include "Table.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

class Tine{
private:
	std::vector<std::string> visitedLinks;
	std::vector<std::string> newLinks;
	bool alreadyVisited(std::string url);
public:
	Tine();
	void runCrawler(int iterations);
	std::vector<std::string> getvisitedLinks();
	std::vector<std::string> getNewLinks();
	char* getTitle(Parser parser);
	std::vector<std::string *> getTableData(Parser parser);
	std::vector<Table> getTables();
	std::string* getTableRowCellContent(xmlNode *row);
};

#endif