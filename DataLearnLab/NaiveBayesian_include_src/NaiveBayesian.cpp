#include "NaiveBayesian.h"
using namespace NavBay;

/** 计算先验概率 P(c)*/
/// \param : c 训练样本分类向量
/// \param : ci 训练样本属于某类 
/// \return : 类别ci的样本数Num(ci) / 训练样本总数Num(c)
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
/** 计算条件概率 P( x | c) = (N( X = xi, C = cj) + 1)|( N( C = cj) + M + V)*/
/// \param : x 训练样本文本
/// \param : c 训练样本属于某类 
/// \param : xi 测试样本文本
/// \param : cj 训练样本属于某类 
/// \return : 
double NavBay_Basic::CalPxc( vector<vector<string> > x, vector<int> c, vector<string> xi, int cj )
{
	double tmpP = 1.0f;
	int num1=0, num2=0, num3=0;
	vector<int> num(xi.size());

	for( int m = 0; m < x.size(); m++)	//第m个训练文本
	{
		if( c[m] == cj) 
		{
			num1 += x[m].size();// 第m个训练文本属于类cj, 类cj文本数累积 N( C = cj) 
			for( int i = 0; i < xi.size(); i++) // 测试样本的第i个string在类
			{
				for( int n = 0; n < x[m].size(); n++ ) // 属于类别cj的训练文本的含有测试样本的第n个string
				{
					if( x[m][n] == xi[i] )// 测试样本的第i个string在类别cj下的数目累积 N( X = xi, C = cj)
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
		tmpP *= ( num[i] + 1 )/( num1 + 2); //拉普拉斯平滑
	}

	return tmpP;
}
/** 计算后验概率 P( cj | x)*/
/// \param : x 训练样本文本
/// \param : c 训练样本属于某类 
/// \param : xi 测试样本文本
/// \param : cj 训练样本属于某类 
double NavBay_Basic::CalProd( vector<vector<string> > x, vector<int> c, vector<string> xi, int cj )
{
	double tmp;
	double Pc = CalPc( c, cj);
	double Pxc = CalPxc( x, c, xi, cj );
	tmp = Pxc*Pc;
	return tmp;
}
/** 对测试样本进行分类*/
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