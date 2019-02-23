#ifndef REQUEST_H
#define REQUEST_H

#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <cstring>
#include <string>


class Request{
private:
	htmlDocPtr doc;
	//size_t grow_buffer(void *contents, size_t sz, size_t nmemb, void *ctx);
public:
	Request(std::string url);
	htmlDocPtr getXMLDoc();
};

#endif