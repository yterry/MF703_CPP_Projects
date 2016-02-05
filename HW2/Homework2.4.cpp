// Homework2.4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cmath"
#include "iostream"

using namespace std;

double exponential(long double x, long double thresh) {
	long double sum = 0;//series sum
	long double expo = 0;//numeritor
	long double fact = 1;//factorial
	int n = 0;
	bool t = true;
	//computes the series
	while (t) {
		expo = pow(x, n);
		if (n == 0) {
			fact = 1;
		}
		else {
			fact = fact*n;
		}
		n += 1;
		sum += expo / (double)fact;
		if (fabs(expo / (double)fact < thresh))t = false;//if the added number is smaller than thresh, computation ends
	}
	return sum;
}

double driver() {
	long double x;
	long double thresh;
	bool t = true;
	int b;
	while (t) {
		cout << "This program computes exponential function through tyler series.\n";
		cout << "Please enter the power number x: ";
		cin >> x;
		cout << "\nPlease enter the threshold (precision): ";
		cin >> thresh;
		cout << "The result is: " << exponential(x, thresh) << "\n";
		cout << "Do you want to compute another exponential ? (-1 to exit): ";
		cin >> b;
		cout << "\n";
		if (b == -1)t = false;
	}
	return 0;
}

int main()
{
	driver();
    return 0;
}

