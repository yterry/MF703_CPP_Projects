// Homework2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "fstream"
#include "vector"
#include "iostream"

using namespace std;

//compute sum of a vector
double summation(vector<double> vec, int n) {
	double s = 0;
	int i;
	for (i = 0; i < n; i++)s += vec[i];
	return s;
}

//compute mean
double SampleMean(vector<double> vec,int n)
{
	double m;
	m = summation(vec, n) /(double) n;
	return m;
}

//compute sampleStDev
double sampleStDev(vector<double> vec, int n) {
	int i;
	double sd;
	vector<double> DevSquared;
	for (i = 0; i < n; i++) {
		DevSquared.push_back( (vec[i] - SampleMean(vec, n))*(vec[i] - SampleMean(vec, n)));
	}
	sd = sqrt(summation(DevSquared, n)/(n-1.0));
	return sd;
}

//compute min
double mini(vector<double> vec, int n) {
	double mi;
	int i;
	mi = vec[0];
	for (i = 0; i < n; i++) {
		if (vec[i] < mi) mi = vec[i];
	}
	return mi;
}

//compute max
double maxi(vector<double> vec, int n) {
	double ma;
	int i;
	ma = vec[0];
	for (i = 0; i < n; i++) {
		if (vec[i] > ma) ma = vec[i];
	}
	return ma;
}

int driver()
{
	//open file
	ifstream infile;
	infile.open("SPXPricesFromCBOE.txt");
	ofstream outfile; //output file stream object
	char c;
	vector<double> M;
	double x; //number to be read in
	int n = 0;
	bool more = true; //more numbers to be read in
	while (more)
	{
		infile.ignore(11, ',') >> x;

		if (infile.ignore(11, ','))//input stream has not failed (no eof or character); equivalent to(!infile.fail())
		{
			M.push_back(x);//add read in data to the end of vector
			n += 1;
		}
		else
		{
			more = false;
		}
	}
	infile.close();//close file

	//print results to screen
	cout << "Mean: " << SampleMean(M, n);
	cout << "\nSample Standard Deviation: " << sampleStDev(M, n);
	cout << "\nMin: " << mini(M, n);
	cout << "\nMax: " << maxi(M, n) <<"\n";

	infile.close();

	return 0;
}

int main() {
	driver();
	return 0;
}


