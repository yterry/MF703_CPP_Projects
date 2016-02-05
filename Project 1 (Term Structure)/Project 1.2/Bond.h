#pragma once

#include <string>
#include <vector>

using namespace std;

class Bond
{
public:
	Bond(string bondName, double coupon, long issueDate, long maturity, int annualCompoundingFrequency);
	void setPrice(double price, double faceValue);
	void setPrice(double price);
	double computeYTM();
	double computePrice(double faceValue, double coupon, double YTM, int numberOfPayments);
	double computePrice(double faceValue, double coupon, double spotRates[], int numberOfPayments);
	double bootstrap(double spotRates[20], int start, int end);
	string getName() const;

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