// Homework2.3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "vector"

using namespace std;

//function reverseDigits
//takes an integer value and returns the number with its digits reversed
int reverseDigits(long int num) {
	long int n = 1;//counts number of digits
	long int deno = 10;//denominator
	long int quotient;
	long int i;
	long int reverse=0;
	vector<int> v;//stores every digit
	bool t = true;//boolean value
	while (t) {
		quotient = num / deno;
		deno = deno * 10;
		if (quotient == 0) {
			t = false;
		}
		else {
			n += 1;
		}
	}
	//store each digit to vector v
	for (i = 0; i < n;i++) {
		quotient = num / pow(10, n - i - 1);
		v.push_back(quotient);
		num = num - quotient*pow(10, n - i - 1);
	}
	//rearrange the digits
	for (i = 0; i < n; i++) {
		reverse += v[i] * pow(10, i);
	}
	return reverse;
}

int driver()
{	
	//ask the user to input number to be reversed
	bool t = true;
	while (t) {
		long int num;
		long int b;
		cout << "Please enter a number to be reversed: ";
		cin >> num;
		cout << "\n";
		cout << "This is the reversed number :" << reverseDigits(num) << "\n"; 
		cout << "Do you want to compute another number ? (-1 to exit): ";
		cin >> b;
		cout << "\n";
		if (b == -1)t = false;
	}
    return 0;
}

int main() {
	driver();
	return 0;
}
