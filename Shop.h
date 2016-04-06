#pragma once
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <regex>

#include "Rebate.h"
#include "Goods.h"

struct Goods1
{
	
};


class Shop
{
public:
	//come up with a name for your shop
	explicit Shop(const std::string& name);
	~Shop();
	
	//you can bind the goods to barcode manually
	void BindBarcodeToGoods(const std::string& barcode, const Goods& goods);

	//this function can bind the goods to barcode automatically by the given json file
	void ConfigGoodsByFile(const std::string& json_filename);

	//this function can parse the data from file
	void ParseBarcodeByFile(const std::string& json_filename);

	//this is the interface which can be invoke by cash register
	void ParseBarcodeByStr(const std::string& json_str);

	//the last print
	void Print();

private:
	std::pair<std::string, double> ParseBarcode(const std::string& barcode);
	void ScanBarcode(const std::string& barcode);
	void SetTypeByRebate(RebateType rebate_type, std::shared_ptr<Rebate> p_rebate);
	bool Check(const Goods& goods);
private:
	std::map<std::string, Goods> goods_;

	
	std::string name_;
	std::stringstream error_;
	std::regex pattern_;
};
