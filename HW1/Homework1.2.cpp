// Reading Input from File: reads from file, prints to screen, prints to file
//
// file: fileIO.cpp
#include <stdafx.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
	//explain what the program does, and ask user to input filename
	cout << "This program computes mean, standard deviation and smallest & largest number of a dataset.\n " << endl;
	
	//creat vector to store read in data
	vector<double> M;
	double x; //number to be read in
	int n = 0;
	bool more = true; //more numbers to be read in
	while (more)
	{
		cout << "please enter your No." << n + 1<<"\tNumber: (enter 7848 to quit)";
		cin >> x;
		if(x != 7848){
			cout << x << '\n';
			M.push_back(x);//add read in data to the end of vector
			n += 1;
		}
		else
		{
			more = false;
		}
	}

	cin.ignore();

	//Compute and print Mean
	double sum = 0;
	int i;
	double mean;
	for (i = 0; i < n; i++) {
		sum += M[i];
	}
	mean = sum / (double)n;
	cout << "Mean:" << mean << endl;

	//Compute and print Standard Deviation
	double sum2 = 0;
	double std;
	for (i = 0; i < n; i++) {
		sum2 += pow(M[i] - mean, 2);
	}
	std = sqrt(sum2 / ((double)n - 1));
	cout << "Standard Deviation:" << std << endl;

	//Compute and print smallest & largest number
	double smallest = M[0];
	double largest = M[0];
	for (i = 0; i < n; i++) {
		if (M[i] < smallest) smallest = M[i];
		if (M[i] > largest) largest = M[i];
	}

	cout << "Smallest Number:" << smallest << endl;
	cout << "Largest Number:" << largest << endl;


	return 0;
}
