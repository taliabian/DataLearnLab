#ifndef PERCEPTRON_H
#define PERCEPTRON_H


#include <vector>
#include "matrix.h"

using namespace std;
using namespace matrixlab;

template <typename Type>
class Perceptron
{
public:
	// constructor and disconstructor
	Perceptron();
	Perceptron( vector<Type> _w0 , Type _b0, int _tnum, int _fnum, Type _rate );
	Perceptron( vector<Type> _w0 , Type _b0, int _tnum, int _fnum, Type _rate, vector<Type> _alph0 );
	~Perceptron();

	// 
	vector<Type> getw0()const;
	vector<Type> getalph0()const;
	Type getb0()const;
	vector<Type> getw()const;
	vector<Type> getalph()const;
	Type getb()const;
	Type getrate()const;
	void setw0( vector<Type> _w0);
	void setalph0( vector<Type> _alph0);
	void setb0( Type _b0); 
	void setrate( Type _rate);
	
	//
	void basic_Perceptron( Matrix<Type> x, vector<int> y );
	//
	void dual_Perceptron( Matrix<Type> x, vector<int> y, vector<Type> _alph );
private:
	vector<Type> w0;
	vector<Type> alph0;
	Type b0;
	vector<Type> w;
	vector<Type> alph;
	Type b;
	int tnum;
	int fnum;
	Type rate;
};

#include "Perceptron_imp.h"

#endif