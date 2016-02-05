#include "stdafx.h"
#include "Bond.h"
#include "cmath"
#include "iostream"

//define the attributes of a bond
Bond::Bond(string bondName, double coupon, long issueDate, long maturity, int annualCompoundingFrequency)
{
	this->bondName = bondName;
	this->coupon = coupon;
	this->issueDate = issueDate;
	this->maturity = maturity;
	this->annualCompoundingFrequency = annualCompoundingFrequency;
	this->numberOfPayments = bondName == "6m"?1:(maturity - issueDate) / 10000 * annualCompoundingFrequency;
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

//getName function to read a bond's name
string Bond::getName()const
{
	return bondName;
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

//Bisection method to find YTM
double Bond::computeYTM()
{
	double YTM = 0.0;
	double epsilon = 0.00000001;
	//YTM should be between 0 and 100, staring range is [0,100], c is the intermediary value
	double a = 0.0;
	double b = 100.0;
	double c = 0.0;
	bool t = true;

	//start computing till price computed using a, b or c is indifferent from real price within epsilon
	while (t)
	{
		//compute the difference
		double aDiff = computePrice(faceValue, coupon / 100.0 / annualCompoundingFrequency, a / 100.0 / annualCompoundingFrequency, numberOfPayments) - price / 100.0*faceValue;
		double bDiff = computePrice(faceValue, coupon / 100.0 / annualCompoundingFrequency, b / 100.0 / annualCompoundingFrequency, numberOfPayments) - price / 100.0*faceValue;

		//check the difference
		if (abs(aDiff) <= epsilon)
		{
			t = false;
			YTM = a;
		}
		else if (abs(bDiff) <= epsilon)
		{
			t = false;
			YTM = b;
		}
		else if (aDiff*bDiff < 0)
		{
			//bisect the range
			c = (a + b) / 2.0;
			double cDiff = computePrice(faceValue, coupon / 100.0 / annualCompoundingFrequency, c / 100.0 / annualCompoundingFrequency, numberOfPayments) - price / 100.0*faceValue;
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
			cout << "Sorry, there is no such YTM." << endl;
			return -1.0;
		}
	}
	return YTM;
}

//function to compute a bond's price using facevalue, coupon, vector of short rates and number of payments
double Bond::computePrice(double faceValue, double coupon, double spotRates[], int numberOfPayments) 
{
	double price = 0.0;
	//discount cash flows with short rates
	for (int i = 0; i < numberOfPayments; i++)
	{
		price += coupon*faceValue / pow(1.0 + spotRates[i], i + 1);
	}
	price += faceValue / pow(1.0 + spotRates[numberOfPayments - 1], numberOfPayments);

	return price;
}

//function to compute short rates using bootstrap
double Bond::bootstrap(double spotRates[20], int start, int end)
{
	//bisection method
	double YTM = 0.0;
	const double epsilon = 0.00001;

	//three initial guess of the spot rate of last cash flow of the bond, 
	//then used to draw the linear term structure line
	double a = 0.0;
	double b = 100.0;
	double c = 0.0;
	bool t = true;


	while (t)
	{
		//three vectors used to guess, based on what we already known along with linear computation 
		double aRates[20];
		double bRates[20];
		double cRates[20];

		//for spot rates already known, copy them from spot rates vector
		for (int i = 0; i <= start;i++)
		{
			aRates[i] = spotRates[i];
			bRates[i] = spotRates[i];
			cRates[i] = spotRates[i];
		}

		//for spot rates unkown, but to be comoputed, 
		//we use a, b to linear compute the spot rates between last known spot rate and maturity spot rate
		//slope is (a-spotRates[start])/(end-start) or (b-spotRates[start])/(end-start)
		for (int i = start + 1; i <= end; i++)
		{
			aRates[i] = (spotRates[start] + (i - start)*(a - spotRates[start]) / (end - start));
			bRates[i] = (spotRates[start] + (i - start)*(b - spotRates[start]) / (end - start));
		}

		for (int i = 0; i < 20; ++i)
		{
			aRates[i] = aRates[i] / 100.0 / annualCompoundingFrequency;
			bRates[i] = bRates[i] / 100.0 / annualCompoundingFrequency;
		}
		
		//use these guessed spot rates vectors to compute price difference with real price
		double fa = computePrice(faceValue, coupon / 100.0 / annualCompoundingFrequency, aRates, numberOfPayments) - price/100.0*faceValue;
		double fb = computePrice(faceValue, coupon / 100.0 / annualCompoundingFrequency, bRates, numberOfPayments) - price/100.0*faceValue;

		//check difference with precision
		if (abs( fa ) < epsilon)
		{
			for (int i = start + 1; i <= end; i++)
			{
				spotRates[i] = aRates[i] * 100.0 * annualCompoundingFrequency;
			}
			t = false;

		}
		else if ( abs( fb ) < epsilon)
		{
			for (int i = start + 1; i <= end; i++)
			{
				spotRates[i] = bRates[i] * 100.0 * annualCompoundingFrequency;
			}
			t = false;
		}
		else if (fa*fb < 0)
		{
			//make c midpoint of (a,b), compute and check fc with epsilon
			c = (a + b) / 2;
			for (int i = start + 1; i <= end; i++)
			{
				cRates[i] = (spotRates[start] + (i - start)*(c - spotRates[start]) / (end - start));
			}
			for (int i = 0; i < 20; ++i)
			{
				cRates[i] = cRates[i] / 100.0 / annualCompoundingFrequency;
			}
			//compute fc
			double fc = computePrice(faceValue, coupon / 100.0 / annualCompoundingFrequency, cRates, numberOfPayments) - price/100.0*faceValue;
			if (abs(fc) < epsilon)
			{
				for (int i = start + 1; i <= end; i++)
				{
					spotRates[i] = cRates[i] * 100.0 * annualCompoundingFrequency;
				}
				t = false;

			}
			//check root in (a,c) or (c,b)
			else if (fa*fc < 0)
			{
				b = c;
			}
			else a = c;
		}
		else
		{
			cout << "Sorry, there is no such solution." << endl;
			t=false;
		}
	}
	return 0;
}