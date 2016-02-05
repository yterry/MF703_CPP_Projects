// Reading Input from File: reads from file, prints to screen, prints to file
//
// file: fileIO.cpp
#include <stdafx.h>
#include <iostream>
#include <iomanip>
#include <fstream>
 
using namespace std;

int main()
{
	ifstream infile; //input file stream object
	infile.open("data.txt");

	ofstream outfile; //output file stream object
	outfile.open("output.txt");

	double x; //number to be read in

	bool more = true; //more numbers to be read in
	while (more) 
	{
		infile >> x;

		if (infile)//input stream has not failed (no eof or character); equivalent to(!infile.fail())
		{
			cout << x << '\t';
			outfile << x + 1;
			outfile.put('\t');
		}
		else 
		{ 
			more = false;
			if  (infile.eof())
				 cout << "\nEnd of data";
			else 
				cout << "\nBad input data";
		}
	}


	infile.close();
	outfile.close();
	
	
	cin.ignore();

	cin.get();

	return 0;
}