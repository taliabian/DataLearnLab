#ifndef PERCEPTRON_H
#define PERCEPTRON_H


#include <vector>
#include "matrix.h"

using namespace std;

template <typename Type>
class Perceptron
{
public:
	// constructor and disconstructor
	Perceptron();
	Perceptron( vector<Type> _w0, Type _b0, bool _dual, int _tnum, int _fnum );
	~Perceptron();

	// 
	vector<Type> getw0();const
	Type getb0();const
	vector<Type> getw();const
	Type getb();const
	void setw0( vector<Type> _w0);
	void setb0( Type _b0); 
	
	//
	bool basic_Perceptron( Matrix<Type> x, vector<int> y );
private:
	vector<Type> w0;
	Type b0;
	vector<Type> w;
	Type b;
	bool dual;
	int tnum;
	int fnum;
};

#include "Perceptron_imp.h"

#endif