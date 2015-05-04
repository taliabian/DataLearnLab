/*!
* \file  matrix.h
* \brief Class template of matrix which is designed for basic linear algebra 
* \author Talia
* \version 1.1
* \date 2015-04-09 
*/

#ifndef MATRIX_H
#define MATRIX_H


#include <vector>
#include <complex>

using namespace std;

namespace matrixlab
{

    template <typename Type>
	struct Data_Pos
	{
		Type Data;
		int row;
		int col;
	};
    template <typename Type>
    class Matrix
    {

    public:
		/// constructor and deconstructor
        Matrix();
        Matrix( const Matrix<Type> &A );
        Matrix( int rows, int columns, const Type &x = Type(0) );
        Matrix( int rows, int columns, const Type *v );
        ~Matrix();
		/// assignment
        Matrix<Type>& operator=( const Matrix<Type> &A );
        Matrix<Type>& operator=( const Type &x );
		/// operator
        Type* operator[]( int i );
        const Type* operator[]( int i ) const;
        Type& operator()( int row, int column );
        const Type& operator()( int  row, int column ) const;

        operator Type*();
        operator const Type*() const;
		/// others caculate
        long size() const;
        int dim( int dimension ) const;
        int rows() const;
        int cols() const;
        Matrix<Type>& resize( int rows, int columns );
        vector<Type> getRow( int row ) const;
        vector<Type> getColumn( int column ) const;
		void setRow( const vector<Type> &v, int row );
        void setColumn( const vector<Type> &v, int column );
		void ReplaceByMatrix( const Matrix<Type> &A, int, int  );
		void SetData( Type, int, int);
		// operator
        Matrix<Type>& operator+=( const Type& );
        Matrix<Type>& operator+=( const Matrix<Type>& );
        Matrix<Type>& operator-=( const Type& );
        Matrix<Type>& operator-=( const Matrix<Type>& );
        Matrix<Type>& operator*=( const Type& );
        Matrix<Type>& operator*=( const Matrix<Type>& );
        Matrix<Type>& operator/=( const Type& );
        Matrix<Type>& operator/=( const Matrix<Type>& );

    private:

        Type *pv0;///< 矩阵'0'基指针
        Type **prow0;///<矩阵'0'基行指针
		Type **prow1;///<矩阵'1'基行指针

        int	 nRow;///< 矩阵行数
        int	 nColumn;///< 矩阵列数
        long nTotal;///< 矩阵总数

        void init( int rows, int columns );
        void copyFromArray( const Type *v );
        void setByScalar( const Type &x );
        void destroy();

    };

	// operator
    template<typename Type>
    ostream& operator<<( ostream&, const Matrix<Type>& );
	template<typename Type>
	ostream& operator<<( ostream&, const vector<Type>& );
	template<typename Type>
	ostream& operator<<( ostream&, const Data_Pos<Type>& );
    template<typename Type>
    istream& operator>>( istream&, Matrix<Type>& );

    template<typename Type>
    Matrix<Type> operator-( const Matrix<Type>& );
    template<typename Type>
    Matrix<Type> operator+( const Matrix<Type>&, const Type& );
    template<typename Type>
    Matrix<Type> operator+( const Type&, const Matrix<Type>& );
    template<typename Type>
    Matrix<Type> operator+( const Matrix<Type>&, const Matrix<Type>& );
	template<typename Type>
    vector<Type> operator+( const vector<Type>&, const vector<Type>& );
    template<typename Type>
    Matrix<Type> operator-( const Matrix<Type>&, const Type& );
    template<typename Type>
    Matrix<Type> operator-( const Type&, const Matrix<Type>& );
    template<typename Type>
    Matrix<Type> operator-( const Matrix<Type>&, const Matrix<Type>& );
	template<typename Type>
    vector<Type> operator-( const vector<Type>&, const vector<Type>& );
    template<typename Type>
    Matrix<Type> operator*( const Matrix<Type>&, const Type& );
    template<typename Type>
    Matrix<Type> operator*( const Type&, const Matrix<Type>& );
	template<typename Type>
    vector<Type> operator*( const Type&, const vector<Type>& );
    template<typename Type>
    Matrix<Type> operator*( const Matrix<Type>&, const Matrix<Type>& );
    template<typename Type>
    vector<Type> operator*( const Matrix<Type>&, const vector<Type>& );
    template<typename Type>
    Matrix<Type> operator/( const Matrix<Type>&, const Type& );
	template<typename Type>
	vector<Type> operator/( const vector<Type>&, const Type& );
    template<typename Type>
    Matrix<Type> operator/( const Type&, const Matrix<Type>& );

	template<typename Type>
	Matrix<Type> strcatMatrix( const Matrix<Type> &, const Matrix<Type> &);
	template<typename Type>
	Matrix<Type> strcatMatrix( const Matrix<Type> &, const vector<Type> &);
	template<typename Type>
	Matrix<Type> strcatMatrix( const vector<Type> &, const Matrix<Type> &);
	template<typename Type>
	Matrix<Type> strcatMatrix( const vector<Type> &, const vector<Type> &);
	template<typename Type>
	Matrix<Type> ExchangeRows( Matrix<Type> &, int, int );
	template<typename Type>
	Matrix<Type> ExchangeCols( Matrix<Type> &, int, int );
	template<typename Type>
	Matrix<Type> ExchangeRowData( Matrix<Type> &, int, int, int, int, int );
	template<typename Type>
	Matrix<Type> CopyFromMatrix( const Matrix<Type> &A, int row1, int col1, int row2, int col2 );
	template<typename Type>
	void Vector2Vector( const vector<Type> &v, vector<Type> &v1);

    template<typename Type>
    Matrix<Type>& optMult( const Matrix<Type>&, const Matrix<Type>&, Matrix<Type>& );
    template<typename Type>
    vector<Type>& optMult( const Matrix<Type>&, const vector<Type>&, vector<Type>& );
	
    template<typename Type>
    Matrix<Type> elemMult( const Matrix<Type>&, const Matrix<Type>& );
	template<typename Type>
    vector<Type> elemMult( const vector<Type>&, const vector<Type>& );
    template<typename Type>
    Matrix<Type> elemDivd( const Matrix<Type>&, const Matrix<Type>& );
    template<typename Type>
    Matrix<Type>& elemMultEq( Matrix<Type>&, const Matrix<Type>& );
    template<typename Type>
    Matrix<Type>& elemDivdEq( Matrix<Type>&, const Matrix<Type>& );

    template<typename Type> Matrix<Type> trT( const Matrix<Type>& );
    template<typename Type> Matrix<Type> trH( const Matrix<Type>& );

    template<typename Type>
    Matrix<Type> trMult( const Matrix<Type>&, const Matrix<Type>& );
    template<typename Type>
    vector<Type> trMult( const Matrix<Type>&, const vector<Type>& );
    template<typename Type>
    Matrix<Type> multTr( const Matrix<Type>&, const Matrix<Type>& );
    template<typename Type>
    Matrix<Type> multTr( const vector<Type>&, const vector<Type>& );
	template<typename Type>
	Type trMult( const vector<Type>&, const vector<Type>& );
    template<typename Type>
    Matrix<complex<Type> > trMult( const Matrix<complex<Type> >&, const Matrix<complex<Type> >& );
    template<typename Type>
    vector<complex<Type> > trMult( const Matrix<complex<Type> >&, const vector<complex<Type> >& );
    template<typename Type>
    Matrix<complex<Type> > multTr( const Matrix<complex<Type> >&, const Matrix<complex<Type> >& );
    template<typename Type>
    Matrix<complex<Type> > multTr( const vector<complex<Type> >&, const vector<complex<Type> >& );
	template<typename Type>
    complex<Type>          trMult( const vector<complex<Type> >&, const vector<complex<Type> >& );
    
	template<typename Type> Matrix<Type> eye( int, const Type& );
	template<typename Type> Matrix<Type> zeros( int, int );
    template<typename Type> vector<Type> diag( const Matrix<Type>& );
    template<typename Type> Matrix<Type> diag( const vector<Type>& );

    template<typename Type> Type norm( const Matrix<Type>& );
	template<typename Type> Type norm( const vector<Type> & );
    template<typename Type> Type norm( const Matrix<complex<Type> >& );
    template<typename Type> void swap( Matrix<Type>&, Matrix<Type>& );
    template<typename Type> vector<Type> sum( const Matrix<Type>& );
    template<typename Type> vector<Type> Min( const Matrix<Type>& );
    template<typename Type> vector<Type> Max( const Matrix<Type>& );
    template<typename Type> vector<Type> mean( const Matrix<Type>& );
	template<typename Type> Data_Pos<Type> FindMaxandPos( const Matrix<Type>& );
	template<typename Type> Data_Pos<Type> FindMinandPos( const Matrix<Type> & );
	template<typename Type> Matrix<Type> abs( const Matrix<Type>& );
    template<typename Type> Matrix<Type> abs( const Matrix<complex<Type> >& );
    template<typename Type> Matrix<Type> arg( const Matrix<complex<Type> >& );
    template<typename Type> Matrix<Type> real( const Matrix<complex<Type> >& );
    template<typename Type> Matrix<Type> imag( const Matrix<complex<Type> >& );
    template<typename Type>
    Matrix<complex<Type> > complexMatrix( const Matrix<Type>& );
	template<typename Type>
    Matrix<complex<Type> > complexMatrix( const Matrix<Type>&, const Matrix<Type>& );

	#include "matrix_impl.h"
}

#endif