#ifndef NETTSIDE_H
#define NETTSIDE_H

#include "Parser.h"
#include "Request.h"
#include "Table.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Nettside {
  protected:
    std::vector<std::string> visitedLinks;
    std::vector<std::string> newLinks;

    bool alreadyVisited(std::string url);
    std::vector<std::string *> getTableData(Parser parser);
    std::string *getTableRowCellContent(xmlNode *row);
    std::vector<std::string> getContentFromNodeSet(xmlNodeSetPtr set);
    bool relativeURL(std::string url);
    void fiksURLs(std::vector<std::string> &vektorAlias);
    bool malformedURL(std::string url);
    virtual bool stringCheck(std::string s)=0;
    void onlyKeepUsefulTineLinks(std::vector<std::string> &vektorAlias);
    void removeDuplicateStringsFromVector(std::vector<std::string> &vektorAlias);

  public:
    void runCrawler(int iterations);
    char *getTitle(Parser parser);
    std::vector<Table> getTables();

    /* for debugging */
    std::vector<std::string> getvisitedLinks();
    std::vector<std::string> getNewLinks();
};

#endif