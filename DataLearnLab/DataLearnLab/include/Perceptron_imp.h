
template <typename Type>
Perceptron<Type>::Perceptron()
: b0(Type(0)), b(Type(0)), dual(0), tnum(0), fnum(0), rate(Type(1))
{
	w0 = vector<Type>(0);
	alph0 = vector<Type>(0);
	w = vector<Type>(0);
	alph = vector<Type>(0);
}
template <typename Type>
Perceptron<Type>::Perceptron( vector<Type> _w0 , Type _b0 , int _tnum, int _fnum, Type _rate ) //,vector<Type> _alph0 )
{
	w0 = _w0;
	b0 = _b0;
	w = w0;
	b = b0;
	tnum = _tnum;
	fnum = _fnum;
	rate = _rate;

}
template <typename Type>
Perceptron<Type>::Perceptron( vector<Type> _w0 , Type _b0, int _tnum, int _fnum, Type _rate,  vector<Type> _alph0 )
{
	w0 = _w0;
	b0 = _b0;
	w = w0;
	b = b0;
	tnum = _tnum;
	fnum = _fnum;
	rate = _rate;
	alph0 = _alph0;
	alph = alph0;
}
template <typename Type>
Perceptron<Type>::~Perceptron()
{

}

// 
template <typename Type>
inline vector<Type> Perceptron<Type>:: getw0() const
{
	return w0;
}
template <typename Type>
inline Type Perceptron<Type>::getb0() const
{
	return b0; 
}
template <typename Type>
inline vector<Type> Perceptron<Type>::getalph0()const
{
	return alph0;
}
template <typename Type>
inline vector<Type> Perceptron<Type>::getw() const
{
	return w;
}
template <typename Type>
inline vector<Type> Perceptron<Type>::getalph()const
{
	return alph;
}
template <typename Type>
inline Type Perceptron<Type>::getb() const
{
	return b;
}
template <typename Type>
inline Type Perceptron<Type>::getrate()const
{
	return rate;
}
template <typename Type>
void Perceptron<Type>::setw0( vector<Type> _w0)
{
	int size = _w0.size();
	for( int i=0; i<size; i++)
		w0[i] = _w0[i];
}
template <typename Type>
void Perceptron<Type>::setalph0( vector<Type> _alph0)
{
	int size = _alph0.size();
	for( int i=0; i<size; i++)
		alph0[i] = _alph0[i];
}
template <typename Type>
void Perceptron<Type>::setb0( Type _b0)
{
	b0 = _b0;
}
template <typename Type>
void Perceptron<Type>::setrate( Type _rate)
{
	rate = _rate;
}
/// 原始感知器
template <typename Type>
void Perceptron<Type>::basic_Perceptron( Matrix<Type> x, vector<int> y )
{
	// 误分类点次序此处为1,2,3,1,2,3....
	Type tmp;
	int i;
	int lnum = 0;
	while( 1 )
	{
		for( i=0; i<tnum; i++)
		{
			vector<Type> t = x.getRow(i);
			tmp = trMult( w, x.getRow(i) ) + b;
			if( tmp*y[i] <= Type(0) )
				break;
		}
		if( i == tnum)
			break;
		w = w + rate*y[i]* x.getRow(i);
		b = b + rate*y[i];
		lnum++;
	}
	cout << "learning num : " << lnum <<endl;
}
 /// 对偶感知器
template <typename Type>
void Perceptron<Type>::dual_Perceptron( Matrix<Type> x, vector<int> y, vector<Type> _alph )
{
	// 误分类点次序此处为1,2,3,1,2,3....
	Type tmp;
	vector<Type> tmpy( tnum );
	int i, j;
	int lnum = 0;
	for( i=0; i<tnum; i++) // w初始化
	{
		w = w + alph[i]*x.getRow(i);
	}
	Matrix<Type> Gram( tnum, tnum );
	for( i=0; i<tnum; i++) // Gram初始化
	{
		for( j=i; j<tnum; j++)
		{
			Gram[i][j] = trMult( x.getRow(i), x.getRow(j) );
			if( i!=j )
				Gram[j][i] = Gram[i][j];
		}
	}
	while(1)
	{
		for( int m=0; m<(y.size()); m++)
			tmpy[m] = alph[m]*y[m];
		for( i=0; i<tnum; i++)
		{
			tmp = trMult( tmpy, Gram.getRow(i) ) + b;
			if( tmp*y[i] <= Type(0) )
				break;
		}
		if( i == tnum)
			break;
		alph[i] = alph[i] + rate;
		b = b + rate*y[i];
		lnum++;
	}
	for( i=0; i<tnum; i++)
	{
		w = w + y[i]*alph[i]*x.getRow(i);
	}
	cout << "learning num : " << lnum <<endl;
}