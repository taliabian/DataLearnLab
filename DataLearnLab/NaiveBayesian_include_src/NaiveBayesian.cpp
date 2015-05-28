#include "NaiveBayesian.h"
using namespace NavBay;

/** ����������� P(c)*/
/// \param : c ѵ��������������
/// \param : ci ѵ����������ĳ�� 
/// \return : ���ci��������Num(ci) / ѵ����������Num(c)
double NavBay_Basic::CalPc( vector<string> c, string ci)
{
	long numci = 0;
	
	for( long i=0; i<c.size(); i++)
	{
		if( ci == c.at(i) )
			numci++;
	}
	double tmp = double(numci)/c.size();
	return tmp;
}
/** ������������ P( x | c) = (N( X = xi, C = cj) + 1)|( N( C = cj) + M + V)*/
/// \param : x ѵ�������ı�
/// \param : c ѵ����������ĳ�� 
/// \param : xi ���������ı�
/// \param : cj ѵ����������ĳ�� 
/// \return : 
double NavBay_Basic::CalPxc( vector<vector<string> > x, vector<string> c, string xi, string cj )
{
	double tmpP = 1.0f;
	int num1=0, num2=0, num3=0;
	vector<double> num;//(xi.size());
	unsigned char ch1, ch2;
	int indx;
	int j = 0;
	for( int m = 0; m < x.size(); m++)	//��m��ѵ���ı�
	{
		if( c[m] == cj) 
		{
			num1 += x[m].size();// ��m��ѵ���ı�������cj, ��cj�ı����ۻ� N( C = cj) 
			for( int i = 0; i < xi.size()-1;) // ���������ĵ�i��string����
			{
				indx = i;
				while( xi.at(indx) != 0x20 )
				{
					ch1 = xi.at(indx);
					ch2 = xi.at(indx+1);
					if( ch1 < 0x81 )
					{
						i = i + 1;
						indx = i;
						continue;
					}
					if( ch1 == 0x81 && ch2 == 0x81 )
					{
						i = i+3;
						break;
					}
					else if( ch1 >= 0x80 && ch2 >= 0x80 )
					{
						indx = indx + 2; //����
					}
					if( indx > xi.size()-1 )
						break;
				}
				j = j+1;
				int nn = 0;
				if( indx > xi.size()-1 )
					break;
				for( int n = 0; n < x[m].size(); n++ ) // �������cj��ѵ���ı� ���в��������ĵ�n��string(����)
				{
					bool find = FindStringInString( x[m][n], xi.substr(i, indx-i));
					if( find == 1)
					{
						nn++;//num.push_back;//[j] ++ ;
					}
				}
				num.push_back(nn);
				i = indx + 1;
			}
		}
	}
	for( int i=0; i<num.size(); i++)
	{
		tmpP *= ( num[i] + 1 )/( num1 + 2); //������˹ƽ��
	}

	return tmpP;
}
bool NavBay_Basic::FindStringInString( string st1, string st2)
{
	int size = st2.size();
	bool flag = 0;
	for( int i=0; i<st1.size()-size; i++)
	{
		if( st2 == st1.substr(i,size) )
		{
			flag = 1;
			break;
		}
	}
	return flag;
}
/** ���������� P( cj | x)*/
/// \param : x ѵ�������ı�
/// \param : c ѵ����������ĳ�� 
/// \param : xi ���������ı�
/// \param : cj ѵ����������ĳ�� 
double NavBay_Basic::CalProd( vector<vector<string> > x, vector<string> c, string xi, string cj )
{
	double tmp;
	double Pc = CalPc( c, cj);
	double Pxc = CalPxc( x, c, xi, cj );
	tmp = Pxc*Pc;
	return tmp;
}
/** �Բ����������з���*/
string NavBay_Basic::Classify( )
{
	vector<double> p(_c.size());
	int indx = 0;
	double max = 0;
	for( int i = 0; i < _c.size(); i++ )
	{
		p[i] = CalProd( _x, _c, _xx, _c[i]);
		if( max < p[i] )
		{
			max = p[i];
			indx = i;
		}
	}
	return _c[indx];
}