/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Web crawler based on curl and libxml2.
 * Copyright (C) 2018 Jeroen Ooms <jeroenooms@gmail.com>
 * License: MIT
 *
 * To compile:
 *   gcc crawler.c $(pkg-config --cflags --libs libxml-2.0 libcurl)
 *
 */
/* <DESC>
 * Web crawler based on curl and libxml2 to stress-test curl with
 * hundreds of concurrent connections to various servers.
 * </DESC>
 */

/* Parameters */
int max_con = 200;
int max_total = 20000;
int max_requests = 500;
int max_link_per_page = 5;
int follow_relative_links = 0;

#include <cstdlib>
#include <ctime>
#include <numeric>
#include <cmath>
#include <sstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <mutex>
#include <vector>
#include <string>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/uri.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>
#include <signal.h>
#include <iostream>

using namespace std;

/* resizable buffer */
typedef struct {
  char *buf;
  size_t size;
} memory;

size_t grow_buffer(void *contents, size_t sz, size_t nmemb, void *ctx)
{
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

/* HREF finder implemented in libxml2 but could be any HTML parser */
vector<string> newLinks(CURLM *multi_handle, memory *mem, char *url){
  vector<string> output;
  int opts = HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | \
  HTML_PARSE_NOWARNING | HTML_PARSE_NONET;
  htmlDocPtr doc = htmlReadMemory(mem->buf, mem->size, url, NULL, opts);
  if(!doc){
    cout << "htmlDocPtr"  << endl;
    return output;
  }
  xmlChar *xpath = (xmlChar*) "//a/@href";
  xmlXPathContextPtr context = xmlXPathNewContext(doc);
  xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
  xmlXPathFreeContext(context);
  if(!result){
    cout << "xmlXPathObjectPtr"<< endl;
    return output;
  }
  xmlNodeSetPtr nodeset = result->nodesetval;
  if(xmlXPathNodeSetIsEmpty(nodeset)) {
    xmlXPathFreeObject(result);
    cout << "xmlXPathNodeSetIsEmpty"<< endl;
    return output;
  }

  for(int i = 0; i < nodeset->nodeNr; i++) {
    //cout << "node: " << i << endl;
    const xmlNode *node = nodeset->nodeTab[i]->xmlChildrenNode;
    xmlChar *href = xmlNodeListGetString(doc, node, 1);

    xmlChar *orig = href;
    href = xmlBuildURI(href, (xmlChar *) url);
    xmlFree(orig);

    char *link = (char *) href;

    if(link){
      output.push_back((string)link);
    }
    xmlFree(link);
  }
  xmlXPathFreeObject(result);
  return output;
}


bool relativeURL(string url){
  return (url.at(0) == '/' && url.at(1) != '/');
}

bool malformedURL(string url){
  return (url.at(0)== '/' && url.at(1) == '/');
}

/* 
  2 typer feil:
  relative urls som mangler www.tine.no fremst
  urls som ender opp med to // fremst
*/
void fiksURLs(vector<string>&vektorAlias){
  for(size_t i = 0;i<vektorAlias.size();i++){
    string tempString = (string)vektorAlias.at(i);
    try{
      if(relativeURL(tempString)){
        vektorAlias.at(i) = "www.tine.no"+tempString;
      } else if(malformedURL(tempString)){
        vektorAlias.at(i) = tempString.substr(2);
      }
    }catch(const out_of_range& oor){
      continue;
    }
  }
}


CURLM * makeMultiHandle(vector<string> urlList){
  curl_global_init(CURL_GLOBAL_DEFAULT);
  CURLM *multi_handle = curl_multi_init();
  curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, max_con);
  curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, 6L);

  for(int i=0;i<urlList.size();i++){

    curl_multi_add_handle(multi_handle, make_handle(&urlList.at(i)[0u]));
  }

  return multi_handle;

}

vector<string> getURLsFromMultiHandle(CURLM *multi_handle){
  vector<string> list;
  int msgs_left;
  //int pending = 0;
  //int complete = 0;
  int still_running = 1;
  while(still_running) {
    int numfds;
    curl_multi_wait(multi_handle, NULL, 0, 1000, &numfds);
    curl_multi_perform(multi_handle, &still_running);

    /* See how the transfers went */
    CURLMsg *m = NULL;
    while((m = curl_multi_info_read(multi_handle, &msgs_left))) {
      if(m->msg == CURLMSG_DONE) {
        CURL *handle = m->easy_handle;
        char *url;
        memory *mem;

        /*  */
        curl_easy_getinfo(handle, CURLINFO_PRIVATE, &mem);
        curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &url);
        vector<string> temp;
        cout << url << endl;
        temp = newLinks(multi_handle, mem, url);
        list.insert(list.end(), temp.begin(), temp.end());
        /*if(m->data.result == CURLE_OK) {
          long res_status;
          curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &res_status);
          if(res_status == 200) {
            char *ctype;
            curl_easy_getinfo(handle, CURLINFO_CONTENT_TYPE, &ctype);
            printf("[%d] HTTP 200 (%s): %s\n", complete, ctype, url);
            if(is_html(ctype) && mem->size > 100) {
              if(pending < max_requests && (complete + pending) < max_total) {
                pending += follow_links(multi_handle, mem, url);
                still_running = 1;
              }
            }
          }else {
            printf("[%d] HTTP %d: %s\n", complete, (int) res_status, url);
          }
        } else {
          printf("[%d] Connection failure: %s\n", complete, url);
        }*/
        curl_multi_remove_handle(multi_handle, handle);
        curl_easy_cleanup(handle);
        free(mem->buf);
        free(mem);
      }
    }
  }
  curl_multi_cleanup(multi_handle);
  curl_global_cleanup();
  return list;
}


int main(void){
  vector<string> initialList;
  initialList.push_back("https://www.tine.no");
  CURLM * initalHandle = makeMultiHandle(initialList);

  vector<string> secondList = getURLsFromMultiHandle(initalHandle);
  //fiksURLs(secondList);

  cout << secondList.size() << endl;
  int startTime = clock();

  CURLM * secondHandles = makeMultiHandle(secondList);

  vector<string> thirdList = getURLsFromMultiHandle(secondHandles);

  int endTime = clock();

  cout << "urls: " << thirdList.size() << " seconds: " << (endTime - startTime)/double(CLOCKS_PER_SEC)  << endl;

  return 0;
}
