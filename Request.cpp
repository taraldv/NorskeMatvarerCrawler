#include "Request.h"

using namespace std;

typedef struct {
	char *buf;
	size_t size;
} memory;

void Request::removeDuplicateStringsFromVector(vector<string>&vektorAlias){
	sort(vektorAlias.begin(),vektorAlias.end());
	vektorAlias.erase(unique(vektorAlias.begin(),vektorAlias.end()),vektorAlias.end());
}

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

htmlDocPtr getXMLDocFromURL(string url){
	char *urlpointer = &url[0u];
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, urlpointer);

  /* memory magic, kopi fra crawler eksempel*/
	memory *mem = (memory*)malloc(sizeof(memory));
	mem->size = 0;
	mem->buf = (char*)malloc(1);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, grow_buffer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, mem);
	curl_easy_perform(curl);

	int opts = HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET;
	htmlDocPtr doc = htmlReadMemory(mem->buf, mem->size, urlpointer, NULL, opts);
	curl_easy_cleanup(curl);
	free(mem->buf);
	free(mem);
	return doc;
}

/* xml magic, finner alle valgt nodes og lager en ny nodeset med alle i */
xmlNodeSetPtr Request::getRegexNodes(xmlChar *regex,string url){
	htmlDocPtr doc = getXMLDocFromURL(url);
	xmlChar *xpath = regex;
	xmlXPathContextPtr context = xmlXPathNewContext(doc);
	xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	return result->nodesetval;
}

vector<string> Request::getContentFromNodeSet(xmlNodeSetPtr set){
	vector<string> vektor;
	if(set){
		for(int i = 0;i<set->nodeNr;i++){
			const xmlNode *node = set->nodeTab[i]->xmlChildrenNode;
			string tempString = (string)((char*)node->content);
			vektor.push_back(tempString);
		}
	}
	return vektor;
}

/* putter content fra hver node i set i en vektor */
/*vector<string> Request::getContentFromNodeSet(vector<xmlNodeSetPtr> nodeSetVektor){
	vector<string> vektor;
	for(size_t i=0;i<nodeSetVektor.size();i++){
		xmlNodeSetPtr set = nodeSetVektor.at(i);
		vector<string> tempVektor;
		for(int j = 0;j<set->nodeNr;j++){
			const xmlNode *node = set->nodeTab[j]->xmlChildrenNode;
			string tempString = (string)((char*)node->content);
			tempVektor.push_back(tempString);
		}
		vektor.insert(vektor.end(),tempVektor.begin(),tempVektor.end());
	}
	return vektor;
}*/



Request::Request(){}