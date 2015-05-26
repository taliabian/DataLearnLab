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
		/** �Բ����������з���*/
		int Classify( );
	protected:
		/** ����������� P(c)*/
		double CalPc( vector<int> c, int ci );
		/** ������������ P( x | c)*/
		double CalPxc( vector<vector<string> > x, vector<int> c, vector<string> xi, int cj );
		/** ���������� P( cj | x)*/
		double CalProd( vector<vector<string> > x, vector<int> c, vector<string> xi, int cj );

	private:
		/** ѵ������*/
		vector<vector<string> > _x;
		vector<int> _c;
		/** ��������*/
		vector<string> _xx;
	};
}

#endif