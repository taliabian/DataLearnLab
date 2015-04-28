
template <typename Type>
Perceptron<Type>::Perceptron()
: b0(Type(0)), b(Type(0)), dual(0), tnum(0), fnum(0)
{
	w0 = vector<Type>(0);
	w = vector<Type>(0);
}
template <typename Type>
Perceptron<Type>::Perceptron( vector<Type> _w0, Type _b0, bool _dual, int _tnum, int _fnum )
{
	w0 = _w0;
	b0 = _b0;
	dual = _dual;
	w = w0;
	b = b;
	tnum = _tnum;
	fnum = _fnum;
}
template <typename Type>
Perceptron<Type>::~Perceptron()
{

}

// 
template <typename Type>
vector<Type> Perceptron<Type>:: getw0() const
{
	return w0;
}
template <typename Type>
Type Perceptron<Type>::getb0() const
{
	return b0;
}
template <typename Type>
vector<Type> Perceptron<Type>::getw() const
{
	return w;
}
template <typename Type>
Type Perceptron<Type>::getb() const
{
	return b;
}
template <typename Type>
void Perceptron<Type>::setw0( vector<Type> _w0)
{
	w0 = _w0;
}
template <typename Type>
void Perceptron<Type>::setb0( Type _b0)
{
	b0 = _b0;
}
template <typename Type>
bool Perceptron<Type>::basic_Perceptron( Matrix<Type> x, vector<int> y )
{
	int tsize = y.size();
	int fsize = x.cols();
	vector<Type> tmp(tsize);
	int i,j;
	while( 1 )
	{
		for( i=0; i<tsiz; i++)
		{
			tmp = w0*tr(x[i]) +¡¡b;
		}
	}
}