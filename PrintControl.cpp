#include "PrintControl.h"
#include "Goods.h"
PrintControl::PrintControl()
{
	SetFormat(list_);
	SetFormat(give_list_);
	SetFormat(sum_list_);
}


PrintControl::~PrintControl()
{
}

void PrintControl::Add(const Goods& goods)
{
	std::stringstream discount_aux;
	SetFormat(discount_aux);
	if (Rebate::TypeCheck(goods.GetRebateType(), RebateType::GIVE))
	{
		if (goods.GetRebateNum() != 0)
			give_list_ << "名称 : " << goods.GetName() << ", 数量 : "
			<< goods.GetRebateNum() << goods.GetCategory() << std::endl;
	}
	else if (Rebate::TypeCheck(goods.GetRebateType(), RebateType::DISCOUNT))
	{
		discount_aux << " , 节省" << goods.GetSumRebatePrice() << "(元)";
	}
	char buf[100] = { 0 };
	sprintf_s(buf, "名称 : %s, 数量 : %.2f%s, 单价 : %.2f(元), 小计 : %.2f(元)",
		goods.GetName().c_str(), goods.GetNum(), goods.GetCategory().c_str(), goods.GetPrice(), goods.GetSumRealPrice());
	
	list_ << buf << discount_aux.str()<<std::endl;
}

void PrintControl::AddSum(double sum, double rebate_sum)
{
	sum_list_ << "总计 : " << sum << "(元)" << std::endl;
	sum_list_ << "节省 : " << rebate_sum << "(元)" << std::endl;
}

void PrintControl::SetFormat(std::stringstream& sstream)
{
	sstream.precision(2);
	sstream.setf(std::ios::fixed);
}
