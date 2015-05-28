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
		/** �Բ����������з���*/
		string Classify( );
	protected:
		/** ����������� P(c)*/
		double CalPc( vector<string> c, string ci );
		/** ������������ P( x | c)*/
		double CalPxc( vector<vector<string> > x, vector<string> c, string xi, string cj );
		/** ���������� P( cj | x)*/
		double CalProd( vector<vector<string> > x, vector<string> c, string xi, string cj );
		/** ��string��ƥ��string*/
		bool FindStringInString( string st1, string st2);
	private:
		/** ѵ������*/
		vector<vector<string> > _x;
		vector<string> _c;
		/** ��������*/
		string _xx;
	};
}

#endif