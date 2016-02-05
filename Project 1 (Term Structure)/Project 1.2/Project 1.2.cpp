// Project 1.2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "iomanip"
#include "vector"
#include "Bond.h"
#include "string"
#include "fstream"
#include "TermStructure.h"

using namespace std;

int main()
{
	//initialize a vector of class bond
	//read raw data of bonds
	vector<Bond> Bonds;
	ifstream infile;
	infile.open("data.txt");

	//define variables that we are going to use when reading file
	bool more = true;
	string name;
	double coupon;
	long issueDate;
	long maturityDate;
	int compoundingFrequencyPerAnnum;
	double price;

	while (more)
	{
		infile >> name;
		if (infile)
		{
			infile >> coupon;
			infile >> issueDate;
			infile >> maturityDate;
			infile >> compoundingFrequencyPerAnnum;
			infile >> price;
			Bond* bond = new Bond(name, coupon, issueDate, maturityDate, compoundingFrequencyPerAnnum);
			//creat a bond with given data
			bond->setPrice(price);	//set price of data
			Bonds.push_back(*bond);	//push the bond to the end of Bonds vector 
		}
		else {
			more = false;
		}
	}
	
	//creat a term structure with bonds
	TermStructure* termStructure = new TermStructure(Bonds);

	//compute term structure of the bonds
	termStructure->computeSpotRates();
	termStructure->computeDiscountFactors();
	termStructure->computeForward6mRates();

	//creat array of tenors
	static string TENORS[20] = { "6m", "1y", "18m", "2y", "2.5y", "3y", "3.5y", "4y", "4.5y", "5y", "5.5y", "6y", "6.5y", "7y", "7.5y", "8y", "8.5y", "9y", "9.5y", "10y" };

	//print results to screen
	cout << "Tenor" << "\t" << setw(10) << "Spot(%)" << "\t" << setw(10) << "DF(%)" << "\t" << setw(10) << "6mFwd(%)" << endl;
	for (int i = 0; i < 20; ++i)
	{
		cout << TENORS[i]
			<< "\t" << fixed << setw(10) << setprecision(5) << termStructure->getSpotRates(i)
			<< "\t" << fixed << setw(10) << setprecision(5) << termStructure->getDiscountFactors(i)
			<< "\t" << fixed << setw(10) << setprecision(5) << termStructure->getForward6mRates(i)
			<< endl;
	}

}