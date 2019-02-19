#include "Tine.h"

using namespace std;

/* sender url til Request konstruktor */
Tine::Tine():Request((char*)"https://www.tine.no"){
	initalNodeSet = getRegexNodes((xmlChar*)"//a/@href",url);
}
vector<string> Tine::getInitialNodeList(){
	return getURLsFromNodeSet(initalNodeSet);
}
