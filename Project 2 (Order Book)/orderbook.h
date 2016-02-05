#pragma once
#include <iostream>
#include "order.h"
#include <map>
#include "stdafx.h"
#include <vector>
#include <functional>
#include <string>

class orderbook
{
public:
	orderbook();
	void match(order);
	void insert(order);
	map<double,vector<order>> getAsk();
	map<double, vector<order>, greater<double>> getBid();
private:
	map<double, vector<order>,greater<double>> Bid;
	map<double, vector<order>> Ask;
	void executeBuyMarket(order);
	void executeSellMarket(order);
	void executeBuyLimit(order);
	void executeSellLimit(order);
};