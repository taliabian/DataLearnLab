#include "NaiveBayesian.h"
using namespace NavBay;

/** 计算先验概率 P(c)*/
/// \param : c 训练样本分类向量
/// \param : ci 训练样本属于某类 
/// \return : 类别ci的样本数Num(ci) / 训练样本总数Num(c)
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
/** 计算条件概率 P( x | c) = (N( X = xi, C = cj) + 1)|( N( C = cj) + M + V)*/
/// \param : x 训练样本文本
/// \param : c 训练样本属于某类 
/// \param : xi 测试样本文本
/// \param : cj 训练样本属于某类 
/// \return : 
double NavBay_Basic::CalPxc( vector<vector<string> > x, vector<string> c, string xi, string cj )
{
	double tmpP = 1.0f;
	int num1=0, num2=0, num3=0;
	vector<double> num;//(xi.size());
	unsigned char ch1, ch2;
	int indx;
	int j = 0;
	for( int m = 0; m < x.size(); m++)	//第m类训练文本
	{
		if( c[m] == cj) 
		{
			num1 += x[m].size();// 第m个训练文本属于类cj, 类cj文本数累积 N( C = cj) 
			for( int i = 0; i < xi.size()-1;) // 测试样本的第i个string在类
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
						indx = indx + 2; //中文
					}
					if( indx > xi.size()-1 )
						break;
				}
				j = j+1;
				int nn = 0;
				if( indx > xi.size()-1 )
					break;
				for( int n = 0; n < x[m].size(); n++ ) // 属于类别cj的训练文本 含有测试样本的第n个string(中文)
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
		tmpP *= ( num[i] + 1 )/( num1 + 2); //拉普拉斯平滑
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
/** 计算后验概率 P( cj | x)*/
/// \param : x 训练样本文本
/// \param : c 训练样本属于某类 
/// \param : xi 测试样本文本
/// \param : cj 训练样本属于某类 
double NavBay_Basic::CalProd( vector<vector<string> > x, vector<string> c, string xi, string cj )
{
	double tmp;
	double Pc = CalPc( c, cj);
	double Pxc = CalPxc( x, c, xi, cj );
	tmp = Pxc*Pc;
	return tmp;
}
/** 对测试样本进行分类*/
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