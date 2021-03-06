#ifndef NETTSIDE_H
#define NETTSIDE_H

#include "../http/Parser.h"
#include "../http/Request.h"
#include "../http/Table.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Nettside {
  protected:
    std::vector<std::string> newLinks;
    std::string base;
    std::string baseURL;
    virtual std::string *getTableRowCellContent(xmlNode *row);

  private:
    std::vector<std::string> visitedLinks;
    bool malformedURL(std::string url);
    bool relativeURL(std::string url);

    bool alreadyVisited(std::string url);

    std::vector<std::string> getContentFromNodeSet(xmlNodeSetPtr set);
    void fiksURLs(std::vector<std::string> &vektorAlias);
    void onlyKeepUsefulLinks(std::vector<std::string> &vektorAlias);
    void removeDuplicateStrings(std::vector<std::string> &vektorAlias);

    virtual bool stringCheck(std::string s) = 0;
    virtual std::vector<std::string *> getTableData(Parser *parser) = 0;

  public:
    void runCrawler(int iterations);
    virtual char *getTitle(Parser *parser) = 0;
    std::vector<Table> getTables();

    /* for debugging */
    std::vector<std::string> getvisitedLinks();
    std::vector<std::string> getNewLinks();
};

#endif