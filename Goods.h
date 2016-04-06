#pragma once
#include <string>
#include "Rebate.h"


class Goods
{
public:
	Goods(){}
	Goods(std::string barcode, std::string name, std::string category, double price, RebateType rebate_type)
		:barcode_(barcode), name_(name), category_(category), price_(price), rebate_type_(rebate_type), num_(0), sum_(-1)
	{
	}
	~Goods();

public:
	std::string GetBarcode()		const { return barcode_; }
	std::string GetName()			const { return name_; }
	std::string GetCategory()		const { return category_; }
	double		GetPrice()			const { return price_; }
	double		GetNum()			const { return num_; }
	RebateType	GetRebateType()		const { return rebate_type_; }
	double		GetSumRealPrice()	const { return RebateFactory::GetInstance()->GetRebate(rebate_type_)->RealPrice(price_, num_); }
	double		GetSumRebatePrice()	const { return price_ * num_ - GetSumRealPrice(); }
	double		GetSumNomalPrice()	const { return price_ * num_; }
	int			GetRebateNum()		const { return RebateFactory::GetInstance()->GetRebate(rebate_type_)->GetRebateNum(num_); }

	void SetNum(double num)	{ num_ = num; }
private:
	std::string barcode_, name_, category_;
	double price_, num_, sum_;
	RebateType rebate_type_;

};

