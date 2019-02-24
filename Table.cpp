#include "Table.h"
#include <iostream>

using namespace std;

Table::Table(std::string title,std::vector<std::string*> data)
:title(title){
	//string *temp0 = new string[length];
	//string *temp1 = new string[length];
	for(size_t i=0;i<data.size();i++){	
		dataVektor.push_back(data.at(i)[1]);
		forkVektor.push_back(data.at(i)[0]);
	}
	//forklaring = temp0;
	//tableData = temp1;
	//pr();
};

vector<string> Table::getDataVektor(){
	return dataVektor;
};

vector<string> Table::getForkVektor(){
	return forkVektor;
};

string Table::getTitle(){
	return title;
}