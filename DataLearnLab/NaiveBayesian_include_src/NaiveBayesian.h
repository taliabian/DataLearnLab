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
		NavBay_Basic();
		NavBay_Basic( vector<vector<string> > x, vector<int> c , vector<string> xx ): _x( x ), _c( c ), _xx( xx ){}
		~NavBay_Basic( );
		/** 对测试样本进行分类*/
		int Classify( );
	protected:
		/** 计算先验概率 P(c)*/
		double CalPc( vector<int> c, int ci );
		/** 计算条件概率 P( x | c)*/
		double CalPxc( vector<vector<string> > x, vector<int> c, vector<string> xi, int cj );
		/** 计算后验概率 P( cj | x)*/
		double CalProd( vector<vector<string> > x, vector<int> c, vector<string> xi, int cj );

	private:
		/** 训练样本*/
		vector<vector<string> > _x;
		vector<int> _c;
		/** 测试样本*/
		vector<string> _xx;
	};
}

#endif