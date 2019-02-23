#ifndef REQUEST_H
#define REQUEST_H

#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

class Request{
private:
	std::vector<htmlDocPtr> docList;
	void emptyHandle(CURLM *multi_handle);
	//size_t grow_buffer(void *contents, size_t sz, size_t nmemb, void *ctx);
public:
	Request(std::vector<std::string> urlList);
	std::vector<htmlDocPtr> getDocList();
};

#endif