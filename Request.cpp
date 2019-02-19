#include "Request.h"

using namespace std;

typedef struct {
	char *buf;
	size_t size;
} memory;

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

htmlDocPtr Request::getXMLDocFromURL(char*urlpointer){
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
  /* genererer xmlDoc fra memory */
	htmlDocPtr doc = htmlReadMemory(mem->buf, mem->size, urlpointer, NULL, opts);
	curl_easy_cleanup(curl);
	return doc;
}

xmlNodeSetPtr Request::getRegexNodes(xmlChar*regex,char*urlpointer){
  htmlDocPtr doc = getXMLDocFromURL(urlpointer);

  /* xml magic, finner alle valgt nodes og lager en ny nodeset med alle i */
  xmlChar *xpath = regex;
  xmlXPathContextPtr context = xmlXPathNewContext(doc);
  xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
  xmlXPathFreeContext(context);

  
  return result->nodesetval;

}

vector<string> Request::getURLsFromNodeSet(xmlNodeSetPtr set){
  vector<string> vektor;
  if(set){
    for(int i = 0;i<set->nodeNr;i++){
      const xmlNode *node = set->nodeTab[i]->xmlChildrenNode;
      string tempString = (string)((char*)node->content);
      /* hvis url er komplett, ellers legg til base url */
      if(tempString.find("www")!=string::npos || tempString.find("http")!=string::npos){
        vektor.push_back(tempString);
      } else {
        vektor.push_back("https://www.tine.no"+tempString);
      }
    }
  }
  return vektor;
}

/*
vector<string> Request::getURLsFromVector(vector<string> inputVektor){
  vector<string> outputVektor;
  /* for hver string i inputVektor lag et nodeSet og add alle nye URLs til outputVektor */
  /*for(unsigned int i = 0;i<inputVektor.size();i++){
    xmlNodeSetPtr tempNodeSet = getRegexNodes((xmlChar*)"//a/@href",&inputVektor.at(i)[0u]);
    vector<string> tempVector = getURLsFromNodeSet(tempNodeSet);
    for(unsigned int j = 0;j<tempVector.size();j++){
     outputVektor.push_back(tempVector.at(j));
   }
 }
 return outputVektor;
}*/




Request::Request(char*url){
	this->url = url;
}