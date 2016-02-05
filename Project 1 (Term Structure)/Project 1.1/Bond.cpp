#include "stdafx.h"
#include "Bond.h"
#include "cmath"
#include "iostream"

//define the attributes of a bond
Bond::Bond(string bondName, double coupon, long issueDate,long maturity, int annualCompoundingFrequency)
{
	this->bondName = bondName;
	this->coupon = coupon;
	this->issueDate = issueDate;
	this->maturity = maturity;
	this->annualCompoundingFrequency = annualCompoundingFrequency;
	this->numberOfPayments = this->numberOfPayments = bondName == "6m" ? 1 : (maturity - issueDate) / 10000 * annualCompoundingFrequency;
}
//setPrice function of Bond to set the price
void Bond::setPrice(double price, double faceValue) 
{
	this->price = price;
	this->faceValue = faceValue;
}
//if no facevalue provided, then set face value as 1000
void Bond::setPrice(double price)
{
	this->price = price;
	this->faceValue = 1000.0;
}
//function to compute a bond's price using facevalue, coupon, YTM and number of payments
double Bond::computePrice(double faceValue, double coupon, double YTM, int numberOfPayments)
{
	double price = 0.0;
	//discount coupon payments
	for (int i = 0; i < numberOfPayments; i++) 
	{
		price += coupon*faceValue / pow((YTM + 1.0), i + 1);
	}
	//discount face value
	price += faceValue / pow(YTM + 1.0, numberOfPayments);

	return price;
}

double Bond::computePriceDerivative(double faceValue, double coupon, double YTM, int numberOfPayments)
{
	double price = 0.0;

	for (int i = 0; i < numberOfPayments; i++)
	{
		price += -(i+1)*coupon*faceValue / pow((YTM + 1.0), i + 2);
	}
	price += -(numberOfPayments)*faceValue / pow((YTM + 1.0), numberOfPayments + 1);
	return price;
}

//Bisection method to find YTM
double Bond::BisecComputeYTM()
{
	double YTM = 0.0;
	double epsilon = 0.0000001;
	//YTM should be between 0 and 100, staring range is [0,100], c is the intermediary value
	double a = 0.0;
	double b = 100.0;
	double c = 0.0;
	bool t = true;

	//start computing till price computed using a, b or c is indifferent from real price within epsilon
	while (t)
	{
		//compute the difference
		double aDiff = computePrice(faceValue, coupon/100.0/annualCompoundingFrequency, a/100.0/annualCompoundingFrequency, numberOfPayments) - price/100.0*faceValue;
		double bDiff = computePrice(faceValue, coupon/100.0/annualCompoundingFrequency, b/100.0/annualCompoundingFrequency, numberOfPayments) - price/100.0*faceValue;

		//check the difference
		if (abs(aDiff) <= epsilon)
		{
			t = false;
			YTM = a;
		}
		else if(abs(bDiff)<=epsilon)
		{
			t = false;
			YTM = b;
		}
		else if (aDiff*bDiff < 0)
		{
			//bisect the range
			c = (a + b) / 2.0;
			double cDiff = computePrice(faceValue, coupon/100.0/annualCompoundingFrequency, c/100.0/annualCompoundingFrequency, numberOfPayments) - price/100.0*faceValue;
			if (abs(cDiff) <= epsilon)
			{
				YTM = c;
				t = false;
			}
			else if (aDiff*cDiff < 0)
			{
				b = c;
			}
			else 
			{
				a = c;
			}
		}

		else
		{
			cout << "Sorry, there is no such YTM."<<endl;
			return -1.0;
		}
	}
	return YTM;
}

//Newton-Raphson Method to find YTM
double Bond::NewtonComputeYTM()
{
	double YTM = coupon;//initial guess
	double YTMold = 0;//intermediary value
	double epsilon = 0.00000001;
	bool t = true;
	//start computing till the additive value is smaller than epsilon
	while (t)
	{
		//compute f(x)
		double fx = computePrice(faceValue, coupon/100.0/annualCompoundingFrequency, YTM/100.0/annualCompoundingFrequency, numberOfPayments) - price/100.0*faceValue;
		
		//compute f'(x)
		double fpx = computePriceDerivative(faceValue, coupon / 100.0 / annualCompoundingFrequency, YTM / 100.0 / annualCompoundingFrequency, numberOfPayments);
		
		//compute next value and check if fx is smaller than epsilon
		if (abs(YTM-YTMold) < epsilon) 
		{
			t = false;
		}
		else
		{
			YTMold = YTM;
			YTM = YTMold - fx / fpx;
		}
	}
	return YTM;
}