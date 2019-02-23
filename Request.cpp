#include "Request.h"


typedef struct {
	char *buf;
	size_t size;
} memory;
/* kopiert fra crawler eksempel libcurl */
size_t grow_buffer(void *contents, size_t sz, size_t nmemb, void *ctx){
	size_t realsize = sz * nmemb;
	memory *mem = (memory*) ctx;
	char *ptr = (char*)realloc(mem->buf, mem->size + realsize);
	if(!ptr) {
    /* out of memory */ 
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}
	mem->buf = ptr;
	memcpy(&(mem->buf[mem->size]), contents, realsize);
	mem->size += realsize;
	return realsize;
}

htmlDocPtr Request::getXMLDoc(){
	return doc;
}


Request::Request(std::string url){
	char *urlpointer = &url[0u];
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, urlpointer);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  /* kopi fra crawler eksempel*/
	memory *mem = (memory*)malloc(sizeof(memory));
	mem->size = 0;
	mem->buf = (char*)malloc(1);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, grow_buffer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, mem);
	curl_easy_perform(curl);

	int opts = HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET;
	doc = htmlReadMemory(mem->buf, mem->size, urlpointer, NULL, opts);
	curl_easy_cleanup(curl);
	free(mem->buf);
	free(mem);
}