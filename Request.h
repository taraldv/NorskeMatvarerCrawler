#ifndef REQUEST_H
#define REQUEST_H

#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/uri.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

class Request{
protected:
	xmlChar* urlRegex = (xmlChar*)"//a/@href";
	xmlNodeSetPtr getRegexNodes(xmlChar* regex,htmlDocPtr doc);
	htmlDocPtr getXMLDocFromURL(std::string url);
	//xmlNodeSetPtr getRegexNodes();
	std::vector<std::string> getContentFromNodeSet(xmlNodeSetPtr set);
	void removeDuplicateStringsFromVector(std::vector<std::string>&vektorAlias);
	Request();
};

#endif