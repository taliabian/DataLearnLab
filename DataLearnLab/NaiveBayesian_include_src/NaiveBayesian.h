#ifndef NAIVEBAYESIAN_H
#define NAVIVEBAYESIAN_H

#include <vector>
#include <algorithm>
#include <string>

using namespace std;

namespace NavBay
{
	class NavBay_Basic{
	public:
		NavBay_Basic(){}
		NavBay_Basic( vector<vector<string> > x, vector<string> c , string xx ): _x( x ), _c( c ), _xx( xx ){}
		~NavBay_Basic( ){}
		/** 对测试样本进行分类*/
		string Classify( );
	protected:
		/** 计算先验概率 P(c)*/
		double CalPc( vector<string> c, string ci );
		/** 计算条件概率 P( x | c)*/
		double CalPxc( vector<vector<string> > x, vector<string> c, string xi, string cj );
		/** 计算后验概率 P( cj | x)*/
		double CalProd( vector<vector<string> > x, vector<string> c, string xi, string cj );
		/** 在string里匹配string*/
		bool FindStringInString( string st1, string st2);
	private:
		/** 训练样本*/
		vector<vector<string> > _x;
		vector<string> _c;
		/** 测试样本*/
		string _xx;
	};
}

#endif