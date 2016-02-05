#include "stdafx.h"
#include "TermStructure.h"
#include "Bond.h"
#include "iostream"


TermStructure::TermStructure(vector<Bond> Bonds)
{
	//initialize bonds
	this->Bonds = Bonds;

	//initialize arraies of spot rates, forward rates and discount factors
	double spotRates[20] = { 0.0};
	double forward6mRates[20] = { 0.0 };
	double discountFactors[20] = { 0.0 };
}

//couple of functions to read individual term structure data computed with our function
double TermStructure::getSpotRates(int i) const
{
	return spotRates[i];
}
double TermStructure::getForward6mRates(int i) const
{
	return forward6mRates[i];
}
double TermStructure::getDiscountFactors(int i) const
{
	return discountFactors[i];
}

void TermStructure::computeSpotRates()
{
	//creating a iterator within Bonds, to interate through every bond
	vector<Bond>::iterator i;

	//start our computation
	for (i = Bonds.begin(); i != Bonds.end(); i++)
	{
		//read Bond name first, match with corresponding spot rate(s)
		//then compute and save corresponding spot rate(s) into spotRates array
		if (i->getName() == "6m")
		{
			spotRates[0] = i->computeYTM();
		}
		else if (i->getName() == "1y")
		{
			i->bootstrap(spotRates, 0, 1);
		}
		else if (i->getName() == "2y")
		{
			i->bootstrap(spotRates, 1, 3);
		}
		else if (i->getName() == "3y")
		{
			i->bootstrap(spotRates, 3, 5);
		}
		else if (i->getName() == "5y")
		{
			i->bootstrap(spotRates, 5, 9);
		}
		else if (i->getName() == "7y")
		{
			i->bootstrap(spotRates, 9, 13);
		}
		else if (i->getName() == "10y")
		{
			i->bootstrap(spotRates, 13, 19);
		}
	}
}

//function to compute discount factor directly using computed spotRates
void TermStructure::computeDiscountFactors()
{
	for (int i = 0; i < 20; i++)
	{
		discountFactors[i] = pow(1.0 + spotRates[i] / 100.0 / 2.0, -(i + 1) / 2.0);
	}
}

//function to compute 6m forward rate directly using computed spotRates
void TermStructure::computeForward6mRates()
{
	for (int i = 0; i < 19; i++)
	{
		forward6mRates[i] = (pow((pow(1.0 + spotRates[i + 1] / 100.0 / 2.0, (i + 2) / 2.0) /
			pow(1.0 + spotRates[i] / 100.0 / 2.0, (i + 1) / 2.0)), 1.0 / 0.5) - 1.0) * 100.0 * 2.0;
	}
}