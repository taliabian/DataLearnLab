#include "NaiveBayesian.h"
using namespace NavBay;

/** ����������� P(c)*/
/// \param : c ѵ��������������
/// \param : ci ѵ����������ĳ�� 
/// \return : ���ci��������Num(ci) / ѵ����������Num(c)
double NavBay_Basic::CalPc( vector<int> c, int ci)
{
	long numci = 0;
	for( long i=0; i<c.size(); i++)
	{
		if( ci == c.at(i) )
			numci++;
	}
	return double(numci)/c.size();
}
/** ������������ P( x | c) = (N( X = xi, C = cj) + 1)|( N( C = cj) + M + V)*/
/// \param : x ѵ�������ı�
/// \param : c ѵ����������ĳ�� 
/// \param : xi ���������ı�
/// \param : cj ѵ����������ĳ�� 
/// \return : 
double NavBay_Basic::CalPxc( vector<vector<string> > x, vector<int> c, vector<string> xi, int cj )
{
	double tmpP = 1.0f;
	int num1=0, num2=0, num3=0;
	vector<int> num(xi.size());

	for( int m = 0; m < x.size(); m++)	//��m��ѵ���ı�
	{
		if( c[m] == cj) 
		{
			num1 += x[m].size();// ��m��ѵ���ı�������cj, ��cj�ı����ۻ� N( C = cj) 
			for( int i = 0; i < xi.size(); i++) // ���������ĵ�i��string����
			{
				for( int n = 0; n < x[m].size(); n++ ) // �������cj��ѵ���ı��ĺ��в��������ĵ�n��string
				{
					if( x[m][n] == xi[i] )// ���������ĵ�i��string�����cj�µ���Ŀ�ۻ� N( X = xi, C = cj)
					{
						num[i] ++ ;
						break;
					}
				}
			}
		}
	}
	for( int i=0; i<xi.size(); i++)
	{
		tmpP *= ( num[i] + 1 )/( num1 + 2); //������˹ƽ��
	}

	return tmpP;
}
/** ���������� P( cj | x)*/
/// \param : x ѵ�������ı�
/// \param : c ѵ����������ĳ�� 
/// \param : xi ���������ı�
/// \param : cj ѵ����������ĳ�� 
double NavBay_Basic::CalProd( vector<vector<string> > x, vector<int> c, vector<string> xi, int cj )
{
	double tmp;
	double Pc = CalPc( c, cj);
	double Pxc = CalPxc( x, c, xi, cj );
	tmp = Pxc*Pc;
	return tmp;
}
/** �Բ����������з���*/
int NavBay_Basic::Classify( )
{
	vector<double> p;
	int indx = 0;
	int max = 0;
	for( int i = 0; i < _c.size(); i++ )
	{
		p[i] = CalProd( _x, _c, _xx, _c[i]);
		if( max < p[i] )
		{
			max = p[i];
			indx = i;
		}
	}

	return indx;
}