#ifndef TINE_H
#define TINE_H

class TabellData{
private:
	char *navnPtr;
	char *arrDataPtr;
	char *arrHeaderPtr;
	TabellData(char *navnPtr, char *arrDataPtr, char *arrHeaderPtr);
	~TabellData();
};

#endif