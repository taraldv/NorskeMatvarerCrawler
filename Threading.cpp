#include "Threading.h"
#include "Request.h"

Threading::Threading(std::vector<std::string> list)
:numberOfThreads((int)list.size()),list(list){}

std::vector<htmlDocPtr> docList;
std::mutex listLock;

void doStuff(int id, std::string url){
	Request req(url);
	htmlDocPtr tempDoc = req.getXMLDoc();
	std::lock_guard<std::mutex> lock(listLock);
	docList.push_back(tempDoc);
}

void Threading::executeThreads(){
	std::thread threads[numberOfThreads];

	for(int i=0;i<numberOfThreads;i++){
		threads[i] = std::thread(doStuff,i,list.at(i));
	}

	for(int j=0;j<numberOfThreads;j++){
		threads[j].join();
	}
}

std::vector<htmlDocPtr> Threading::getDocList(){
	return docList;
}