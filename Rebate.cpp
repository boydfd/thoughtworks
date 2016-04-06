#include <limits>

#include "Rebate.h"

double BiggerEquald(double d1, double d2)
{
	return d1 - d2 > -0.0000001;
}

Rebate* RebateCreate(const RebateType type, int rebate1, int rebate2)
{
	Rebate* result;
	if (type == RebateType::GIVE)
	{
		result = new GiveRebate;
	}
	else if (type == RebateType::DISCOUNT)
	{
		result = new DiscountRebate;
	}
	else
	{
		return nullptr;
	}
	result->SetRebate(rebate1, rebate2);
	return result;
}

////////////////////////////////GiveRebate
double GiveRebate::RealPrice(double price, double amount)
{
	double result = 0;
	double part = price *(request_num_ - give_num_);
	while (BiggerEquald(amount, request_num_))
	{
		result += part;
		amount -= request_num_;
	}
	result += amount * price;
	return result;
}

void GiveRebate::SetRebate(int rebate1, int rebate2)
{
	//if rebate1 is an unavailable number, it will be set at a max number
	request_num_ = rebate1 < 0 ? std::numeric_limits<int>::max() : rebate1;
	//if rebate1 is an unavailable number, it will be set at 0
	give_num_ = rebate2 < 0 || rebate2 > rebate1 ? 0 : rebate2;
}


////


////////////////////////////////DiscountRebate
double DiscountRebate::RealPrice(double price, double amount)
{
	return price * amount * discount_;
}

void DiscountRebate::SetRebate(int rebate1, int rebate2)
{
	//if rebate1 is an unavailable number, it will be set at 100
	rebate1 = rebate1 < 0 || rebate1 > 100 ? 100 : rebate1;
	discount_ = static_cast<double>(rebate1) / 100;
}
////


////////////////////////////////RebateFactory
//static RebateFactory::RebateFactory* instance_ = nullptr;
std::shared_ptr<Rebate> RebateFactory::GetRebate(RebateType type)
{
	if (rebate_[type].get() == nullptr)
	{
		throw std::exception("none-set RebateType");
		
	}
	else
	{
		return GetInstance()->rebate_[type];
	}
}

void RebateFactory::SetRebate(RebateType type, std::shared_ptr<Rebate> rebate)
{
	rebate_[type] = std::move(rebate);
}

RebateFactory* RebateFactory::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new RebateFactory;
	}
	return instance_;
}
RebateFactory* RebateFactory::instance_ = nullptr;
//std::map<RebateType, std::shared_ptr<Rebate>> RebateFactory::rebate_;
bool RebateFactory::CheckType(RebateType type)
{
	for (auto v : rebate_)
	{
		if (Rebate::TypeCheck(type, v.first))
		{
			return true;
		}
	}
	return false;
}


////