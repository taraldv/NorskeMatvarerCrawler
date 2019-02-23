#include "TabellData.h"

TabellData::TabellData(char *navnPtr,char *arrDataPtr, char *arrHeaderPtr)
:navnPtr(navnPtr),arrDataPtr(arrDataPtr),arrHeaderPtr(arrHeaderPtr){}

TabellData::~TabellData(){
	delete navnPtr;
	delete arrDataPtr;
	delete arrHeaderPtr;
}