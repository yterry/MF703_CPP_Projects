#pragma once
#include "vector"
#include "string"
#include "Bond.h"

class TermStructure
{
public:
	TermStructure(vector<Bond>);

	void computeSpotRates();
	void computeDiscountFactors();
	void computeForward6mRates();

	double getSpotRates(int i) const;
	double getDiscountFactors(int i) const;
	double getForward6mRates(int i) const;

private:
	vector<Bond> Bonds;
	double spotRates[20];
	double discountFactors[20];
	double forward6mRates[20];
};

