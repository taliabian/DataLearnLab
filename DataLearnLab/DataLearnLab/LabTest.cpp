#include <stdio.h>
#include <iostream>
#include <vector>
#include "include\matrix.h"
#include "include\Perceptron.h"

using namespace std;
using namespace matrixlab;

typedef double  Type;

int main()
{
	int tsize(0);
	int fsize(0);
	int i, j;

	cout << "1st: enter the size of train data and the feature size: ";
	cin >> tsize >> fsize;
	cout << "2st: enter the train data of x: "<<endl;
	Matrix<Type> x(tsize, fsize);
	for( i=0; i<tsize; i++)
		for( j=0; j<fsize; j++)
		{
			cin >> x[i][j];
		}
	cout << "x "<< x << endl;

	cout << "3st: enter the train data of y: "<<endl;
	vector<int> y(tsize);
	for( i=0; i<tsize; i++)
		cin >> y[i];
	cout << "y "<< y << endl;

	cout << "4st: enter the w0, b0 and learning rate: "<<endl;
	vector<Type> w0(fsize);
	Type b0;
	Type rate;
	for( i=0; i<fsize; i++)
		cin >> w0[i];
	cin >> b0 >> rate;
	
	Perceptron<Type> Per( w0, b0, tsize, fsize, rate);
	Per.basic_Perceptron( x, y);
	cout << "w: "<< Per.getw();
	cout << "b: "<< Per.getb() <<endl;
	vector<Type> alph0( tsize );
	cout << "5st: enter the alph0: "<<endl;
	for( i=0; i<tsize; i++)
		cin >> alph0[i];
	Per = Perceptron<Type>( w0, b0, tsize, fsize, rate, alph0 );
	Per.setalph0( alph0 );
	Per.dual_Perceptron( x, y, alph0);
	cout << "alph: "<< Per.getalph();
	cout << "w: "<< Per.getw();
	cout << "b: "<< Per.getb() <<endl;
	return 0;
}