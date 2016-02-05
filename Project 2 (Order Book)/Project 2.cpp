// Project 2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "order.h"
#include "orderbook.h"

using namespace std;

int main()
{
	string ticker;
	string side;
	string type;
	int qty;
	double limprice;
	string counterparty;

	orderbook OrderBook=orderbook();

	ifstream infile;
	infile.open("input.txt");
	bool more = true;
	while (more) 
	{
		if (infile.good())
		{
			infile >> ticker;
			infile >> side;
			infile >> type;
			infile >> qty;
			infile >> limprice;
			infile >> counterparty;
			order currentOrder = order(ticker, side, type, qty, limprice, counterparty);
			cout << "\n************   New Order Received   ************\n" <<"Type: "
				<< currentOrder.getSide()<<" "<<currentOrder.getType() <<"\tPrice: " << currentOrder.getLimprice()
				<< "\tQuantity: " << currentOrder.getQty()
				<< "\tFrom: " << currentOrder.getCounterparty() << endl <<"\n";
			OrderBook.match(currentOrder);
			cout << "************************************************"<<endl;
		}
		else { break; }
	}
	//generate iterator in Bid and Ask maps, both starting from greatest price
	map<double, vector<order>> Ask = OrderBook.getAsk();
	map<double, vector<order>,greater<double>> Bid = OrderBook.getBid();
	map<double, vector<order>>::reverse_iterator iter1 = Ask.rbegin();
	map<double, vector<order>>::iterator iter2 = Bid.begin();

	//print current orderbook to the screen
	cout << "\n\n\t\t\t\t\t\t******CURRENT ORDERBOOK******" << endl;
	cout << "\t\t\t\t\tPrice\t\t\t\t\t" << "Quantity" << endl;
	for (iter1; iter1 != (Ask.rend()); iter1++)
	{
		double quantity = 0;
		for (int i = 0; i < iter1->second.size(); i++) quantity = quantity + iter1->second[i].getQty();
		cout << "\t\t\t\t\t" << iter1->first << "\t\t\t\t\t" << quantity << endl;
	}
	cout << "\n\t\t\t\t\t¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü   ASK   ¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü" << endl;
	cout << "\t\t\t\t\t¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý   BID   ¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý\n" << endl;
	for (iter2; iter2 != Bid.end(); iter2++)
	{
	double quantity = 0;
	for (int i = 0; i < iter2->second.size(); i++) quantity = quantity + iter2->second[i].getQty();
	cout << "\t\t\t\t\t" << iter2->first << "\t\t\t\t\t" << quantity << endl;
	}
    return 0;
}