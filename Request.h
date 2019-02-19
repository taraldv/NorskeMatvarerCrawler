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

class Request{
/* alt brukes av extended classes */
protected:
	char *url;
	//size_t grow_buffer(void *contents, size_t sz, size_t nmemb, void *ctx);
	htmlDocPtr getXMLDocFromURL(char*urlpointer);
	xmlNodeSetPtr getRegexNodes(xmlChar*regex,char*urlpointer);
	std::vector<std::string> getURLsFromNodeSet(xmlNodeSetPtr set);
	//vector<string> getURLsFromVector(vector<string> inputVektor);
	Request(char *url);
};

#endif