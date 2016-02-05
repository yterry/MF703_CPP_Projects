// Homework2.2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "exception"

using namespace std;

//computes f(x)
double fx(double x,int n,double a) {
	return pow(x, n) - a;
}

//computes f'(x)
double fpx(double x, int n, double a) {
	return n*pow(x, n - 1);
}

//computes nth root
double root(int n,double a) {
	double x = 1;//initial guess
	double m = 0;//intermediate value
	double epsilon = 0.00000001;
	bool t = true;
	while (t) {
		m = x - fx(x, n, a)/fpx(x,n,a);
		if (abs(m - x) < epsilon) {
			x = m;
			t = false;
		}
		else (x = m);
	}
	return x;
}

//main function to test if this Newton method program works
int main()
{
	int n;
	double a;
	int b;
	bool t = true;
	while (t) {
		//couple of instructions for users to input values
		cout << "This program computes nth root of a, with precision epsilon = 0.00000001";
		cout << "\nPlease enter n (as in nth root): ";
		cin >> n;
		cout << "\nThis is your input n: " << n;
		cout << "\n\nPlease enter a (to compute its nth root): ";
		cin >> a;
		//print user's input to screen
		cout << "\nThis is your input a: " << a;
		//print results to screen
		cout << "\n\nThis is the nth root of a: " << root(n, a) << "\n\n";

		//ask if user want to compute another root, or enter -1 to exit
		cout << "Do you want to compute a root ? (-1 to exit): ";
		cin >> b;
		cout <<"\n";
		if (b == -1)t = false;
	}
	return 0;
}

