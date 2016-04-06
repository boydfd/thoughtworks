#pragma once
#include <string>
#include <sstream>

#include "Rebate.h"
class Goods;
class PrintControl
{
public:
	PrintControl();
	~PrintControl();

	void Add(const Goods& goods);
	void AddSum(double sum, double rebate_sum);
	std::string GetPrintString();
	std::string PrintControl::GetList(){ return list_.str(); }
	std::string PrintControl::GetGiveList(){ return give_list_.str(); }
	std::string PrintControl::GetSum(){ return sum_list_.str(); }

private:
	void SetFormat(std::stringstream& sstream);
private:

	std::string print_string_;
	std::stringstream list_, give_list_, sum_list_;
};

