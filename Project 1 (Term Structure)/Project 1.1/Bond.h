#pragma once

#include <string>

using namespace std;

class Bond
{
public:
	Bond(string bondName, double coupon, long issueDate, long maturity, int annualCompoundingFrequency);
	void setPrice(double price, double faceValue);
	void setPrice(double price);
	double BisecComputeYTM();
	double NewtonComputeYTM();
	double computePrice(double faceValue, double coupon, double YTM, int numberOfPayments);
	double computePriceDerivative(double faceValue, double coupon, double YTM, int numberOfPayments);

private:
	string bondName;
	double coupon;
	long issueDate;
	long maturity;
	int annualCompoundingFrequency;
	double price;
	double faceValue;
	double YTM;
	int numberOfPayments;
	
};