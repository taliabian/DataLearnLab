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
		file >> strtmp2;
		str_c.push_back( strtmp1 );
	}
	str_c.pop_back( );
	file.close();
	/// 对分类样本分词、过滤
	path = "SougoMini\\Sample\\";	
	/// 停用词设定
	string StopWords[] = {"的","我们","要","自己","之","将","“","”", 
						  "，","（","）","后","应","到","某","个","是",
						  "位","新","一","两","在","中","或","有","更","好"," "};
	stringstream ss;
	string str, filestr;
	for( int i=0; i<str_c.size(); i++)
	{
		for( int j=0; j<10; j++)
		{
			ss << 10+j;
			ss >> str;
			path = path + str_c[i] + "\\" + str +  ".txt";
			file.open( path, ifstream::in);
			while(!file.eof())
			{
				file >> filestr;
				const char * ch = NLPIR_ParagraphProcess( filestr.c_str(),0); // 样本分词
				cout << ch <<endl;
				string s = ch;
				int len = sizeof(StopWords)/sizeof(string);
				s = Ignore_Key_string( s, StopWords, len );
				
				cout << s<<endl;
				/*cout <<s.substr(0,2)<<endl;
				cout <<s.substr(2,2)<<endl;
				cout <<s.substr(4,2)<<endl;*/
			}

		}
	}
/*
	path = "Sogoumini\\test.txt";	
	file.open( path, ifstream::in );
	
	/// 停用词设定
	string StopWords[] = {"的","我们","要","自己","之","将","“","”", 
						  "，","（","）","后","应","到","某","后","个",
						  "是","位","新","一","两","在","中","或","有",
						  "更","好"," "};
	/// 测试样本文本过滤
	int len = sizeof(StopWords)/sizeof(string);
	string nTest_str = equalsIgnoreCase( path, StopWords, len );
	cout << nTest_str <<endl;*/
	
	//file.close();
	//path = "Sogoumini\\test1.txt";	
	//file.open( path, ifstream::in );
	//while( !file.eof() )
	//{
	//	file >> strtmp1;
	//	cout << strtmp1<<endl;
	//}
	//file.close();
	NLPIR_Exit();
	system("pause");
	return 0;
}
/// 字符串过滤
string Ignore_Key_string( string splitstr, string *StopWords, int len )
{
	string strtmp1, strtmp2, strtmp3;	
	unsigned char ch1, ch2;
	int flag = 0;
	strtmp1 = splitstr;
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
	return strtmp1;
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
	//cout << strtmp1<<endl;
	file.close();
	return strtmp1;
}