// BlackLittermanModel.cpp : Defines the entry point for the console application.
// Project 4.2 : The Black Litterman Model

#include "stdafx.h"
#include <armadillo>
#include<stdio.h>
#include <vector>
#include <fstream>

using namespace std;
using namespace arma;

int main()
{
	double rf = 0.00008;		//Assumed daily risk-free rate
	double tau = 0.025;			//Arbitrary number assumed according to Berkley Paper
	int delta = 1;				//Risk aversion parameter. Assume it to be 1, but it can be calulated from (E(rm) -rf)/market variance
	mat returnsMatrix;			//matrix of returns
	mat meanReturn;				//matrix of mean return
	vector<string> AssetList;	//list of assets
	mat data;					//stock price data matrix
	mat dataLag;				//stock price t-1
	mat Pi;						// Implied Equilibrium Excess Returns
	mat Q;						// Views of the Asset Excess Returns 
	mat covarianceMatrix;		//Variance-Covariance Matrix of Assets
	mat Omega;					// Covariance Matrix of the Views (Confidence (uncertainty) Level of Views)
	mat P;						// Linker Matrix that links postivity and negativity of assets with views

	//input stock price data; each row represents an observation; number of column is number of assets.
	data.load("stock prices.csv");
	data.shed_row(0);

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

	//input views matrix P; each row represents an view; number of column is number of assets.
	P.load("P.csv");

	//input views of the Asset Excess Returns matrix Q; each row represents an view; number of column is number of assets.
	Q.load("Q.csv");

	//make change to data to compute return
	dataLag = data;
	dataLag.shed_row(data.n_rows - 1);			//delete last row to get data(t-1)
	data.shed_row(0);							//delete first row to get data(t)
	
	//compute returns matrix with stock price data: Returns = [data(t)-data(t-1)]/data(t-1) 
	returnsMatrix = (data - dataLag) / dataLag;

	//compute mean return matrix with returns data
	meanReturn = (sum(returnsMatrix) / returnsMatrix.n_rows).t();

	//compute covariance matrix with returns data
	mat E(returnsMatrix.n_rows, 1);
	E.fill(1.0);//Unit Matrix used to compute covariance matrix
	covarianceMatrix = ((returnsMatrix - (1 / returnsMatrix.n_rows)*E*E.t()*returnsMatrix).t())*(returnsMatrix - (1 / returnsMatrix.n_rows)*E*E.t()*returnsMatrix);
	
	//compute Omega which is uncertainty of views using P matrix and variance-covariance matrix
	Omega.resize(P.n_rows,P.n_rows);
	for (int i = 0; i < P.n_rows; i++)
	{
		mat tempRow = P.row(i);
		mat temp = tau*tempRow*covarianceMatrix*tempRow.t();
		double temp2 = temp(0, 0);
		Omega(i, i) = temp2;
	}

	//Weights according to Market Capitalization for each asset class, used to calculate implied excess returns
	double N = P.n_cols;
	mat marketCapWeights(1, N);
	marketCapWeights.fill(1 / N);//assume the equilibrium market portfolio to be equally weighted

	//compute implied equilibrium excess returns Pi
	Pi = delta*covarianceMatrix*marketCapWeights.t();

	//compute Expected Returns using Black-Litterman model
	mat mu = (((tau*covarianceMatrix).i() + P.t()*(Omega)*P).i())*(((tau*covarianceMatrix).i())*Pi + P.t()*(Omega)*Q);
	
	//compute Excess Returns
	mat ExcessReturns = mu - rf;

	//compute the optimal portfolio weight with views
	mat OptimalWeights = ((delta*covarianceMatrix).i())*ExcessReturns;
	OptimalWeights = OptimalWeights.t();
	mat Sum = sum(OptimalWeights,1);
	double sumScalar = Sum(0, 0);
	OptimalWeights = OptimalWeights / sumScalar;

	//print the computation results and optimal weights to the console
	cout << "\nThis Program Computes Optimal Portfolio Weights Using Black-Litterman Model" << endl;
	cout << "\nHere are computed characteristic data for the assets in the portfolio:\n\n\nMean Return of the Assets are : \n\n";
	for (int i = 0; i < AssetList.size(); i++) { cout << "  " << AssetList[i] << "  "; }
	meanReturn.t().print("\n");
	cout << "\n\nVariance-Covariance Matrix of the Assets are:\n\n";
	for (int i = 0; i < AssetList.size(); i++) { cout << "  " << AssetList[i] << "  "; }
	covarianceMatrix.print("\n");
	cout << "\n\nMarket Equilibrium Weights of the Assets are:\n\n";
	for (int i = 0; i < AssetList.size(); i++) { cout << "  " << AssetList[i] << "  "; }
	marketCapWeights.print("\n");
	cout << "\n\nBlack-Litterman Optimal Portfolio Weights are:\n\n";
	for (int i = 0; i < AssetList.size(); i++) { cout << "  " << AssetList[i] << "  "; }
	OptimalWeights.print("\n");
	cout << "\n\n" << endl;
	return 0;
}

