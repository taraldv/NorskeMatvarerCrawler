#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>

class Table {
  private:
	std::string title;
	std::vector<std::string> forkVektor;
	std::vector<std::string> dataVektor;
	
  public:
    Table(std::string tittel,std::vector<std::string*> data);
	std::vector<std::string> getDataVektor();
	std::vector<std::string> getForkVektor();
	std::string getTitle();
};

#endif