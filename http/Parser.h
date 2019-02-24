#ifndef PARSER_H
#define PARSER_H

#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/uri.h>

class Parser{
private:
	//xmlChar* urlRegex = (xmlChar*)"//a/@href";
	htmlDocPtr doc;
public:
	Parser(htmlDocPtr doc);
	xmlNodeSetPtr getRegexNodes(xmlChar* regex);
	~Parser();
};

#endif