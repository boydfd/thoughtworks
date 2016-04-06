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
			give_list_ << "���� : " << goods.GetName() << ", ���� : "
			<< goods.GetRebateNum() << goods.GetCategory() << std::endl;
	}
	else if (Rebate::TypeCheck(goods.GetRebateType(), RebateType::DISCOUNT))
	{
		discount_aux << " , ��ʡ" << goods.GetSumRebatePrice() << "(Ԫ)";
	}
	char buf[100] = { 0 };
	sprintf_s(buf, "���� : %s, ���� : %.2f%s, ���� : %.2f(Ԫ), С�� : %.2f(Ԫ)",
		goods.GetName().c_str(), goods.GetNum(), goods.GetCategory().c_str(), goods.GetPrice(), goods.GetSumRealPrice());
	
	list_ << buf << discount_aux.str()<<std::endl;
}

void PrintControl::AddSum(double sum, double rebate_sum)
{
	sum_list_ << "�ܼ� : " << sum << "(Ԫ)" << std::endl;
	sum_list_ << "��ʡ : " << rebate_sum << "(Ԫ)" << std::endl;
}

void PrintControl::SetFormat(std::stringstream& sstream)
{
	sstream.precision(2);
	sstream.setf(std::ios::fixed);
}
