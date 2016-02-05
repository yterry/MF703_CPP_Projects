// Project 1.1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "Bond.h"
#include "string"

using namespace std;

int main()
{
	//creating a bond with bondName, coupon, issueDate, maturity and annualCompoundingFrequency
	string bondName = "3year";
	double coupon = 8.0 +5.0/8.0;
	long issueDate = 19970301;
	long maturity = 20310301;
	int annualCompoundingFrequency = 2;
	Bond* bond = new Bond(bondName,coupon,issueDate,maturity,annualCompoundingFrequency);

	//set the price of bond (percentage of faceValue)
	bond->setPrice(106.5);

	//ask user to choose a method;
	int choice;
	bool t = true;
	while (t)
	{
		cout << "Please choose a method for computation:\n1. Bisection Method\n2. Newton-Raphson Method\n\nEnter 1 or 2:";
		cin >> choice;

		if (choice == 1)
		{
			//compute YTM using bisection method and print to screen
			cout << "\nThe yield-to-maturity of given bond computed by bisection method is:" << bond->BisecComputeYTM() << endl;
			break;
		}
		else if (choice == 2)
		{
			//compute YTM using Newton-Raphson method and print to screen
			cout << "\nThe yield-to-maturity of given bond computed by Newton-Raphson method is:" << bond->NewtonComputeYTM() << endl;
			break;
		}
		else
		{
			cout << "Wrong input. " << endl;
		}
	}

	return 0;
}