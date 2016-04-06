#include <fstream>
#include <cassert>
#include <iostream>

#include "json\json.h"

#include "Shop.h"
#include "PrintControl.h"

//题意应该是买3减1，如果是买2赠1应该是买5个给7个才对,所以这里使用的是REACH 3 RETURN 1。
#define REACH 10
#define RETURN 1

#define SHOW_ERROR	
using std::ifstream;
using std::string;


Json::Value GetRootFromJson(const string& json_filename)
{
	ifstream ifs;
	ifs.open(json_filename);
	assert(ifs.is_open());
	Json::Reader reader;
	Json::Value root;
	assert(reader.parse(ifs, root, false) && "fail to parse the json file in GetRootFromJson function");
	return root;
}

//////////////////////////////////public function
Shop::Shop(const std::string& name) :name_(name),  
	pattern_(R"((ITEM\d{6})(-(\d+[.\d]*))?)") //there  we can change the pattern of barcode
{

	RebateFactory::GetInstance()->SetRebate(RebateType::GIVE, std::shared_ptr<Rebate>(RebateCreate(RebateType::GIVE, REACH, RETURN)));
	RebateFactory::GetInstance()->SetRebate(RebateType::DISCOUNT, std::shared_ptr<Rebate>(RebateCreate(RebateType::DISCOUNT, 30)));
}

Shop::~Shop()
{
}

void Shop::BindBarcodeToGoods(const string& barcode, const Goods& goods)
{
	goods_[barcode] = goods;
}

/*
	the format of Goods:
	[
		{
			"barcode": string,
			"name": string,
			"category": string,
			"price": num,
			"rebate_type":num	(1: buy 2 then give 1, 2: 5% discount, 3: both)
		},
		{ ... },
		...
	]

*/
void Shop::ConfigGoodsByFile(const string& json_filename)
{
	auto root = GetRootFromJson(json_filename);
	for (unsigned size = root.size(), i = 0; i < size; ++i)
	{
		Goods goods
		{ 
			root[i]["barcode"].asString(), 
			root[i]["name"].asString(), 
			root[i]["category"].asString(), 
			root[i]["price"].asDouble(), 
			static_cast<RebateType>(root[i]["rebate_type"].asInt())
		};
		BindBarcodeToGoods(goods.GetBarcode(), goods);
	}
}

void Shop::ParseBarcodeByFile(const string& json_filename)
{
	auto root = GetRootFromJson(json_filename);
	for (unsigned size = root.size(), i = 0; i < size; ++i)
	{
		ScanBarcode(root[i].asString());
	}
}

void Shop::ParseBarcodeByStr(const std::string& json_str)
{
	Json::Reader reader;
	Json::Value root;
	assert(reader.parse(json_str, root, false) && "fail to parse the json data in Shop::ParseBarcodeByStr function");
	for (unsigned size = root.size(), i = 0; i < size; ++i)
	{
		ScanBarcode(root[i].asString());
	}
}



void Shop::Print()
{
	PrintControl prt_crtl;
	double sum = 0, rebate_sum = 0;
	for (auto it = goods_.begin(); it != goods_.end(); ++it)
	{
		auto& goods = it->second;
		if (!Check(goods))
			continue;

		prt_crtl.Add(goods);

		sum += goods.GetSumRealPrice();
		rebate_sum += goods.GetSumRebatePrice();
	}
	prt_crtl.AddSum(sum, rebate_sum);

	string split = "----------------------\n";
	std::cout << "***<" << name_ << ">购物清单***\n";
	std::cout << prt_crtl.GetList();
	std::cout << split;
	if (prt_crtl.GetGiveList().compare(""))
	{
		std::cout << "买二赠一商品:" << std::endl;
		std::cout << prt_crtl.GetGiveList();
		std::cout << split;
	}
	std::cout << prt_crtl.GetSum();
	std::cout << "**********************" << std::endl;
	#ifdef SHOW_ERROR
		std::cout << "error information:\n" << error_.str();
	#endif
}


/////////////////////////////////////////private function

//return： first: name, second: num
std::pair<string, double> Shop::ParseBarcode(const string& barcode)
{
	std::smatch result;
	if (std::regex_search(barcode, result, pattern_))
	{
		if (result.size() > 1 && result[1].matched)
		{
			if (result.size() > 3 && result[3].matched)
				return std::make_pair(result.str(1), std::stod(result.str(3)));
			else
				return std::make_pair(result.str(1), 1);
			for (auto s : result)
			{
				auto ss =s.str();
			}
			auto s = result[0].str();
		}
	}
	else
		return std::make_pair(string(), 0);

}

void Shop::ScanBarcode(const string& barcode)
{
	auto goods = ParseBarcode(barcode);
	if (goods_.find(goods.first) != goods_.end())
		goods_[goods.first].SetNum(goods_[goods.first].GetNum() + goods.second);
	else //if the data is in the database, there could be a "else if" which performs the SQL statements
		error_ << "there is no this barcode which called \"" << goods.first << "\" in our Shop\n";
}



bool Shop::Check(const Goods& goods)
{
	if (!RebateFactory::GetInstance()->CheckType(goods.GetRebateType()))
	{
		error_ << "you haven't set the type " << static_cast<int>(goods.GetRebateType())
			<< " which you use in \"" << goods.GetName() << "\"\n";
		return false;
	}
	if (goods.GetNum() <= 0)
		return false;
}




////
