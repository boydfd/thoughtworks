#include <iostream>

#include "Rebate.h"
#include "Shop.h"

int main()
{

	Shop shop("enen");
	shop.ConfigGoodsByFile("D:\\config.json");
	shop.ParseBarcodeByFile("D:\\barcode.json");
	shop.Print();


	std::cin.get();
	return 0;
}



