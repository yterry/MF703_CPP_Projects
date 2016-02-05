#include "stdafx.h"
#include "orderbook.h"
#include <iterator>
#include <vector>
#include <map>

using namespace std;

orderbook::orderbook()
{

}

map<double, vector<order>> orderbook::getAsk()
{
	return this->Ask;
}

map<double, vector<order>, greater<double>> orderbook::getBid()
{
	return this->Bid;
}

void orderbook::insert(order currentOrder)
{
	double price = currentOrder.getLimprice();
	if (currentOrder.getSide() == "buy")
	{
		cout << "[Message] Order inserted." << endl;
		Bid[price].push_back(currentOrder);
	}
	else
	{
		cout << "[Message] Order inserted." << endl;
		Ask[price].push_back(currentOrder);
	}
}

void orderbook::match(order currentOrder)
{
	if (currentOrder.getSide() == "buy")
	{
		if (currentOrder.getType() == "market")
		{
			if (Ask.size() != 0)
			{
				//cout << "Ask.size()!=0"<<endl;
				if ((Ask.begin()->second[0]).getQty() != 0)
				{
					cout << "[Message] Trying to execute..." << endl;
					executeBuyMarket(currentOrder);	
				}
			}
			else
			{
				cout << "[Message] Order could not be filled\n";//do nothing
			}
		}
		else
		{
			double price = currentOrder.getLimprice();

			if (Ask.size() == 0)
			{
				insert(currentOrder);
			}
			else if (price < Ask.begin()->first)	//might have bug
			{
				insert(currentOrder);
			}											//might have bug
			else //if ((Ask.begin()->second[0]).getQty() != 0)
			{
				cout << "[Message] Trying to execute..." << endl;
				executeBuyLimit(currentOrder);
			}
		}
	}
	else
	{
		if (currentOrder.getType() == "market")
		{
			if (Bid.size() != 0)
			{
				if ((Bid.begin()->second[0]).getQty() != 0)
				{
					cout << "[Message] Trying to execute..." << endl;
					executeSellMarket(currentOrder);
				}
			}
			else
			{
				cout<<"[Message] Order could not be filled";//do nothing
			}
		}
		else
		{
			double price = currentOrder.getLimprice();
			if (Bid.size() == 0)
			{
				insert(currentOrder);
			}
			else if (price > (Bid.begin())->first)
			{
				insert(currentOrder);
			}
			else// if ((Bid.begin()->second[0]).getQty() != 0)
			{
				cout << "[Message] Trying to execute..." << endl;
				executeSellLimit(currentOrder);
			}
		}
	}
}

void orderbook::executeBuyMarket(order currentOrder)
{
	for (map < double, vector<order>>::iterator iter = Ask.begin(); iter != Ask.end();)
	{
		int i = 0;
		while (iter->second.size()!=0)
		{
			if (iter->second[i].getQty() <= currentOrder.getQty())
			{
				currentOrder.setQty(currentOrder.getQty() - iter->second[i].getQty());
				//need to send a message to both sides
				cout <<"[Message] Transaction made. Price: "<<iter->first
						<<"\tQuantity: " << iter->second[i].getQty()
						<<"\tCounterparty: "<<iter->second[i].getCounterparty()<< endl;
				iter->second[i].setQty(0);
				if (iter->second.size() != 0) iter->second.erase(iter->second.begin());
				if ((iter->second.size() == 1 && iter->second[0].getQty() == 0) || (iter->second.size() == 0))
				{
					Ask.erase(iter++);
					break;
				}
				else
				{
					++iter;
				}
			}
			else
			{
				cout << "[Message] Transaction made. Price: " << iter->first
					<< "\tQuantity: " << currentOrder.getQty()
					<< "\tCounterparty: " << iter->second[i].getCounterparty() << endl;
				iter->second[i].setQty(iter->second[i].getQty()-currentOrder.getQty());
				//need to send a message to both sides
				currentOrder.setQty(0);
				break;
			}
		}
		if (currentOrder.getQty() == 0) break;
	}
}

void orderbook::executeSellMarket(order currentOrder)
{
	for (map < double, vector<order>>::iterator iter = Bid.begin(); iter != Bid.end(); )
	{
		int i = 0;
		while (iter->second.size() != 0)
		{
			if (iter->second[i].getQty() <= currentOrder.getQty())
			{
				currentOrder.setQty(currentOrder.getQty() - iter->second[i].getQty());
				//need to send a message to both sides
				cout << "[Message] Transaction made. Price: " << iter->first
					<< "\tQuantity: " << iter->second[i].getQty()
					<< "\tCounterparty: " << iter->second[i].getCounterparty() << endl;
				iter->second[i].setQty(0);
				if (iter->second.size() != 0) iter->second.erase(iter->second.begin());
				if ((iter->second.size() == 1 && iter->second[0].getQty() == 0)||(iter->second.size()==0))
				{
					Bid.erase(iter++);
					break;
				}
				else
				{
					++iter;
				}
			}
			else
			{
				cout << "[Message] Transaction made. Price: " << iter->first
					<< "\tQuantity: " << currentOrder.getQty()
					<< "\tCounterparty: " << iter->second[i].getCounterparty() << endl;
				iter->second[i].setQty(iter->second[i].getQty() - currentOrder.getQty());
				//need to send a message to both sides
				currentOrder.setQty(0);
				break;
			}
		}
		if (currentOrder.getQty() == 0) break;
	}
}

void orderbook::executeBuyLimit(order currentOrder)
{
	for (map < double, vector<order>>::iterator iter = Ask.begin(); iter != Ask.end(); )
	{
		if (iter->first <= currentOrder.getLimprice())
		{
			int i = 0;
			while (iter->second.size() != 0)
			{
				if (iter->second[i].getQty() <= currentOrder.getQty())
				{
					currentOrder.setQty(currentOrder.getQty() - iter->second[i].getQty());
					//message needed
					cout << "[Message] Transaction made. Price: " << iter->first
						<< "\tQuantity: " << iter->second[i].getQty()
						<< "\tCounterparty: " << iter->second[i].getCounterparty() << endl;					//need to send a message to both sides
					iter->second[i].setQty(0);
					if (iter->second.size() != 0) iter->second.erase(iter->second.begin());
					if ((iter->second.size() == 1 && iter->second[0].getQty() == 0) || (iter->second.size() == 0))
					{
						Ask.erase(iter++);
						break;
					}
					else
					{
						++iter;
					}
				}
				else
				{
					cout << "[Message] Transaction made. Price: " << iter->first
						<< "\tQuantity: " << currentOrder.getQty()
						<< "\tCounterparty: " << iter->second[i].getCounterparty() << endl;
					iter->second[i].setQty(iter->second[i].getQty() - currentOrder.getQty());
					//need to send a message to both sides
					currentOrder.setQty(0);
					break;
				}
			}
			if (currentOrder.getQty() == 0) break;
		}
		else
		{
			insert(currentOrder);
			break;
		}
	}
}

void orderbook::executeSellLimit(order currentOrder)
{
	for (map < double, vector<order>>::iterator iter = Bid.begin(); iter != Bid.end();)
	{
		if (iter->first >= currentOrder.getLimprice())
		{
			int i = 0;
			while (iter->second.size() != 0)
			{
				if (iter->second[i].getQty() <= currentOrder.getQty())
				{
					currentOrder.setQty(currentOrder.getQty() - iter->second[i].getQty());
					//need to send a message to both sides
					cout << "[Message] Transaction made. Price: " << iter->first
						<< "\tQuantity: " << iter->second[i].getQty()
						<< "\tCounterparty: " << iter->second[i].getCounterparty() << endl;
					iter->second[i].setQty(0);
					if(iter->second.size()!=0) iter->second.erase(iter->second.begin());
					if ((iter->second.size() == 1 && iter->second[0].getQty() == 0) || (iter->second.size() == 0))
					{
						Bid.erase(iter++);
						break;
					}
					else
					{
						++iter;
					}
				}
				else
				{
					cout << "[Message] Transaction made. Price: " << iter->first
						<< "\tQuantity: " << currentOrder.getQty()
						<< "\tCounterparty: " << iter->second[i].getCounterparty() << endl;					
					iter->second[i].setQty(iter->second[i].getQty() - currentOrder.getQty());
					//need to send a message to both sides
					currentOrder.setQty(0);
					break;
				}
			}
			if (currentOrder.getQty() == 0) break;
		}
		else
		{
			insert(currentOrder);
			break;
		}
	}
}