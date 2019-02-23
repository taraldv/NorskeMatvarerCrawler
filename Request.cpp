#include "Request.h"

using namespace std;

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

vector<htmlDocPtr> Request::getDocList(){
	return docList;
}

CURL *make_handle(char *url)
{
	CURL *handle = curl_easy_init();

  /* Important: use HTTP2 over HTTPS */
	curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
	curl_easy_setopt(handle, CURLOPT_URL, url);

  /* buffer body */
	memory *mem = (memory*)malloc(sizeof(memory));
	mem->size = 0;
	mem->buf = (char*)malloc(1);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, grow_buffer);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, mem);
	curl_easy_setopt(handle, CURLOPT_PRIVATE, mem);
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);

  /* For completeness */
	curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "");
	curl_easy_setopt(handle, CURLOPT_TIMEOUT, 5L);
	curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 10L);
	curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 2L);
	curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "");
	curl_easy_setopt(handle, CURLOPT_FILETIME, 1L);
	curl_easy_setopt(handle, CURLOPT_USERAGENT, "mini crawler");
	curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
	curl_easy_setopt(handle, CURLOPT_UNRESTRICTED_AUTH, 1L);
	curl_easy_setopt(handle, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
	curl_easy_setopt(handle, CURLOPT_EXPECT_100_TIMEOUT_MS, 0L);
	return handle;
} 

CURLM * makeMultiHandle(vector<string> urlList){
	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURLM *multi_handle = curl_multi_init();
	//curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, max_con);
	//curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, 6L);

	for(size_t i=0;i<urlList.size();i++){

		curl_multi_add_handle(multi_handle, make_handle(&urlList.at(i)[0u]));
	}

	return multi_handle;

}



htmlDocPtr getDocFromHandle(CURLM *multi_handle, memory *mem, char *url){
	int opts = HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET;
	return htmlReadMemory(mem->buf, mem->size, url, NULL, opts);
}

void Request::emptyHandle(CURLM *multi_handle){
	
	int msgs_left;
	int still_running = 1;
	while(still_running) {
		int numfds;
		curl_multi_wait(multi_handle, NULL, 0, 1000, &numfds);
		curl_multi_perform(multi_handle, &still_running);
		CURLMsg *m = NULL;
		while((m = curl_multi_info_read(multi_handle, &msgs_left))) {
			if(m->msg == CURLMSG_DONE) {
				CURL *handle = m->easy_handle;
				char *url;
				memory *mem;
				curl_easy_getinfo(handle, CURLINFO_PRIVATE, &mem);
				curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &url);
				docList.push_back(getDocFromHandle(multi_handle, mem, url));
				curl_multi_remove_handle(multi_handle, handle);
				curl_easy_cleanup(handle);
				free(mem->buf);
				free(mem);
			}
		}
	}
	curl_multi_cleanup(multi_handle);
	curl_global_cleanup();
}

Request::Request(std::vector<std::string> urlList){
	emptyHandle(makeMultiHandle(urlList));
}