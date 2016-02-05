// Project 4.1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <armadillo>

using namespace std;
using namespace arma;

int main()
{
	mat Returns;//return matrix
	mat Return;//mean return matrix
	mat Covariance;//covariance matrix
	mat data;//stock price data matrix
	mat data_lag;//stock price t-1
	mat w;//weight matrix
	double r = 0.01;//investor's desired expectated return of portfolio
	vector<string> AssetList;	//list of assets

	//intermediate variables
	mat A;
	mat B;
	mat C;
	mat D;
	mat g;
	mat h;

	//input stock price data; each row represents an observation; number of column is number of assets.
	data.load("stock prices.csv");
	data.shed_row(0);//delete first row which is company name
	data_lag = data;
	data_lag.shed_row(data.n_rows - 1);//delete last row to get data(t-1)
	data.shed_row(0);//delete first row to get data(t)

	//read asset list from price data first row
	ifstream infile; infile.open("stock prices.csv"); string temp0;
	if (infile.good())
	{
		infile >> temp0;
		infile.close();
	}
	stringstream ss(temp0); string temp1;
	while (getline(ss, temp1, ',')) {
		AssetList.push_back(temp1);
	}

	//compute returns matrix with stock price data: Returns = [data(t)-data(t-1)]/data(t-1) 
	Returns = (data - data_lag) / data_lag;

	//compute mean return matrix with returns data
	Return = (sum(Returns) / Returns.n_rows).t();

	//compute covariance matrix with returns data
	mat E(Returns.n_rows, 1);
	E.fill(1.0);//Unit Matrix used to compute covariance matrix
	Covariance = ((Returns - (1 / Returns.n_rows)*E*E.t()*Returns).t())*(Returns - (1 / Returns.n_rows)*E*E.t()*Returns);

	E.set_size(Return.n_rows, 1);
	E.fill(1.0);//reset unit matrix used in computation

	//several matrix used in mean-variance computation
	A = E.t()*inv(Covariance)*Return;
	B = Return.t()*inv(Covariance)*Return;
	C = E.t()*inv(Covariance)*E;
	D = B*C - A*A;
	g = 1.0 / D*(B*E.t() - A*Return.t())*inv(Covariance);
	h = 1.0 / D*(C*Return.t() - A*E.t())*inv(Covariance);

	//compute weight with the matrices computed above 
	w = g + h*r;

	//print optimized portfolio weight 
	cout << "\nThis Program Computes Optimal Portfolio Weights Using Mean-Variance Model" << endl;
	cout << "\nHere are computed characteristic data for the assets in the portfolio:\n\n\nMean Return of the Assets are : \n\n";
	for (int i = 0; i < AssetList.size(); i++) { cout << "  " << AssetList[i] << "  "; }
	Return.t().print("\n");
	cout << "\n\nVariance-Covariance Matrix of the Assets are:\n\n";
	for (int i = 0; i < AssetList.size(); i++) { cout << "  " << AssetList[i] << "  "; }
	Covariance.print("\n");
	cout << "\n\nInverstor's Expected Return of the Portfolio is:" << r << "\n";
	cout << "\nMean-Variance Optimal Portfolio Weights are:\n\n";
	for (int i = 0; i < AssetList.size(); i++) { cout << "  " << AssetList[i] << "  "; }
	w.print("\n");
	cout << "\n\n" << endl;
	return 0;
}