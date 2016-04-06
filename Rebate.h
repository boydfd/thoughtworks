#pragma once
#include <map>
#include <memory>
enum class RebateType
{
	GIVE = 1,
	DISCOUNT = 2,
	MAX = GIVE | DISCOUNT
};
class Rebate
{
public:
	Rebate(){}
	virtual ~Rebate(){}

	virtual double RealPrice(double price, double amount) = 0;
	virtual void SetRebate(int rebate1 = -1, int rebate2 = -1) = 0;
	static bool TypeCheck(RebateType type, RebateType rtype) 
	{ 
		return static_cast<int>(type) & static_cast<int>(rtype) &&
			static_cast<int>(type) <= static_cast<int>(RebateType::MAX); 
	}
	virtual int GetRebateNum(int num) { return 0; }
};

//题意应该是买3减1，如果是买2赠1应该是买5个给7个才对。
class GiveRebate : public Rebate
{
public:
	GiveRebate(){}
	virtual ~GiveRebate(){}

	virtual double RealPrice(double price, double amount);
	virtual void SetRebate(int rebate1 = -1, int rebate2 = -1);
	virtual int GetRebateNum(int num) 
	{ 
		return request_num_ >= 0 && give_num_ >= 0 ? num / request_num_ * give_num_ : 0; 
	}
private:
	int request_num_, give_num_;
};

class DiscountRebate : public Rebate
{
public:
	DiscountRebate(){}
	virtual ~DiscountRebate(){}

	virtual double RealPrice(double price, double amount);
	virtual void SetRebate(int rebate1 = -1, int rebate2 = -1);
	
private:
	double discount_;
};

//Flyweight pattern
class RebateFactory
{
public:
	~RebateFactory(){}
	std::shared_ptr<Rebate> GetRebate(RebateType type);
	void SetRebate(RebateType type, std::shared_ptr<Rebate> rebate);
	static RebateFactory* GetInstance();
	bool CheckType(RebateType type);
private:
	RebateFactory(){}

	static RebateFactory* instance_;
	std::map<RebateType, std::shared_ptr<Rebate>> rebate_;
};
Rebate* RebateCreate(const RebateType type, int rebate1 = -1, int rebate2 = -1);
