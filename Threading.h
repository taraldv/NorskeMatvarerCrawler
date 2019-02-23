#ifndef THREADING_H
#define THREADING_H

#include <libxml/HTMLparser.h>
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

class Threading{
private:

	int numberOfThreads;
	std::vector<std::string> list;
	//void doStuff(int id, std::string url);
public:
	Threading(std::vector<std::string> list);
	void executeThreads();
	std::vector<htmlDocPtr> getDocList();
};

#endif