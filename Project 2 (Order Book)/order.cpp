#include "stdafx.h"
#include "order.h"
#include "string.h"

order::order(string t, string s, string ty, int q, double l, string c)
{
	ticker = t;
	side = s;
	type = ty;
	qty = q;
	limprice = l;
	counterparty = c;
}

string order::getTicker(void) 
{
	return ticker;
}

string order::getSide(void)
{
	return side;
}

string order::getType(void)
{
	return type;
}

int order::getQty(void)
{
	return qty;
}

void order::setQty(int Quantity)
{
	qty = Quantity;
}

double order::getLimprice(void)
{	
	return limprice;
}

string order::getCounterparty(void)
{
	return counterparty;
}
