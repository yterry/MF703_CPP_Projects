#pragma once
#include <iostream>
#include <string.h>

using namespace std;

class order
{
private:
	string ticker;
	string side;
	string type;
	int qty;
	double limprice;
	string counterparty;
public:
	order(string, string, string, int, double, string);
	string getTicker();
	string getSide();
	string getType();
	int getQty();
	double getLimprice();
	string getCounterparty();
	void setQty(int quantity);
};