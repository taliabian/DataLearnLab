#include <iostream>
#include <algorithm>
#include <io.h>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include "NaiveBayesian.h"

#include "NLPIR.h"
#ifndef OS_LINUX
#pragma comment(lib, "NLPIR.lib")
#else
#include<pthread.h>
#endif

using namespace std;
using namespace NavBay;

string Ignore_Key_file_string( string filename, string *StopWords, int len );
string Ignore_Key_string( string splitstr, string *StopWords, int len );

int main()
{
	/// NLPIR 初始化
	if( !NLPIR_Init() )
	{
		cout << " Init Fails \n";
		return -1;
	}
	/// 数据分类信息读取
	string path = "SougoMini\\ClassList.txt";
	ifstream file;
	file.open( path, ifstream::in );
	if( !file.good() )
	{
		cout << "open file fail "<<endl;
		return -1;
	}
	vector<string> str_c;
	string strtmp1, strtmp2;
	while( !file.eof() )
	{
		file >> strtmp1;
		str_c.push_back( strtmp1 );
	}
	str_c.pop_back( );
	file.close();
	/// 对分类样本分词、过滤
	path = "SougoMini\\Sample\\";	
	/// 停用词设定
	string StopWords[] = {"的","我们","要","自己","之","将","“","”", 
						  "，","（","）","后","应","到","某","个","是",
						  "位","新","一","两","在","中","或","有","更",
						  "好", "：","？","。"};
	stringstream ss;
	string str, filestr, filestr1;
	
	vector<vector<string> > x( str_c.size()/2 ); //训练样本数据
	vector<string> c; // 训练样本分类数据
	string xx; // 测试样本数据

	/// 测试样本数据
	file.open( path+"test.txt", ios::in);
	if( !file.good() )
	{
		cout << "cannot open the test file \n";
		return -1;
	}
	file >> xx;
	xx = NLPIR_ParagraphProcess( xx.c_str() ,0); // 测试样本分词
	int len = sizeof(StopWords)/sizeof(string);
	xx = Ignore_Key_string( xx, StopWords, len ); // 测试样本过滤 
	file.close();
	string s;
	filestr.clear();
 	for( int i=0; i<str_c.size(); i = i+2)
	{
		c.push_back( str_c[i] );
		for( int j=0; j<10; j++)
		{
			ss.clear();
			ss << 10+j;
			ss >> str;
			path =  path + str_c[i] + "\\" + str +  ".txt";
			file.open( path, ifstream::in);
			if( !file.good() )
			{
				cout << "cannot open the test file \n";
				return -1;
			}
			while(!file.eof())
			{
				file >> filestr1;
			    filestr = filestr + filestr1;
			}
			s = NLPIR_ParagraphProcess( filestr.c_str(),0); // 训练样本分词
			s = Ignore_Key_string( s, StopWords, len ); // 训练样本过滤 
			x[i/2].push_back( s );
			file.close();
			filestr.clear();
			path = "SougoMini\\Sample\\";	
		}
	}
	NLPIR_Exit();
	NavBay_Basic nb( x, c, xx);
	string feature = nb.Classify();
	cout << feature ;
	system("pause");
	return 0;
}
/// 字符串过滤
string Ignore_Key_string( string splitstr, string *StopWords, int len )
{
	string str1, str2, str3;
	str1 = splitstr;
	for( int j=0; j<len; j ++)
	{
		for( int i=0; i<str1.size()-1;)
		{
				int size = StopWords[j].size();
				int m=0;
				while( m <size && (i+m) < str1.size())
				{
					if( (unsigned char )str1[i+m] < 0x81)
					{
						m++;
						break;
					}
					m++;
				}
				if( m == size )  //中文个数匹配
				{
					str3 = str1.substr( i, m );
					if( str3.compare( StopWords[j]) != 0) //中文未匹配上,进入新string
						str2 = str2 + str3;
					i = i + m;
				}
				else{
						str2 = str2 + str1.substr(i,m);
						i = i+m;
					}
		}
		str1 = str2;
		str2.clear();
	}
	return str1;
}
/// 本文本过滤
string Ignore_Key_file_string( string filename, string *StopWords, int len )
{
	ifstream file;
	file.open( filename, ifstream::in );
	string strtmp1, strtmp2, strtmp3;	
	unsigned char ch1, ch2;
	int flag = 0;
	while( !file.eof() )
	{
		file >> strtmp1;
		for( int j=0; j < len; j++) //过滤第j类词
		{
			for( int i=0; i < strtmp1.size(); )
			{
				// 判断是否为中文
				ch1 = strtmp1[i];
				if( i+1 < strtmp1.size() )
					ch2 = strtmp1[i+1];
				if( ch1>0x80 && ch2>0x80) // 中文
				{
					flag = 1;
					int n = 0, m = 0;
					for( m =0 ; m < StopWords[j].size(); m = m+2)
					{
						strtmp2 = strtmp1.substr( i, 2 ); 
						if( strtmp2.compare( StopWords[j].substr(m,2)) == 0 ) //判断是否匹配
						{
							n = n+2;
							i = i+2;
						}
					}
					if( n != 0 ) // 匹配
						i = i-2;
					else// 未匹配上,加入新字符串
						strtmp3 = strtmp3 + strtmp1.substr( i ,2);
				}
				else // 不是中文也加入新字符串
				{
					strtmp3 = strtmp3 + strtmp1.substr( i,1);
				}
				if( 1 == flag )
					i = i+2;
				else 
					i = i+1;
				flag = 0;
			}
			strtmp1 = strtmp3;
			strtmp3.clear();
		}
	}
	file.close();
	return strtmp1;
}