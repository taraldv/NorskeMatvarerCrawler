#ifndef TINE_H
#define TINE_H

#include "Request.h"
#include "Parser.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>


class Tine{
private:
	std::vector<std::string> links;
public:
	Tine();
	void runCrawler(int iterations);
	std::vector<std::string> getLinks();
	char* getTitle(Parser parser);
	void getTableData(Parser parser);
	void nyTest();
	std::string* getTableRowCellContent(xmlNode *row);
};

#endif