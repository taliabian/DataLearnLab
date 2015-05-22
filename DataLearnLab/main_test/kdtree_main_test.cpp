#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <assert.h>
#include "kdtree.h"

using namespace std;
using namespace KDTree;

int main()
{
	int size = 6, dim = 2;
	int type = 1;
	vector<vector<double> > data(6, vector<double>(2));
	/// 读文件数据
	ifstream file("data.txt");
	assert( file != NULL );
	double tmp;
	int row = 0, col = 0;
	while( !file.eof() )
	{
		for( col = 0; col < dim; col++)
		{
			file>>tmp;
			data[row][col] = tmp;
		}
		row++;
		if( row == size )
			break;
	}
	file.close();
	for( row = 0; row<size; row++)
	{
		for( col = 0; col<dim; col++)
			cout << data[row][col]<<"\t";
		cout << endl;
	}

	KDtree kdtree;
	kdtree.Create( data );
	cout << " Input target point :" <<endl;
	vector<double> target(dim);
	for( col=0; col<dim ; col++)
		cin >> target[col];

	pair<vector<double>, double> res;
	res = kdtree.findNearest( target, type );

	for( col=0; col<dim; col++)
		cout<< res.first[col]<<"\t"; 
	cout<<endl;
	cout<<" distance is " << res.second<<endl;
	vector<pair<vector<double>, double>> res_result;
	int num = kdtree.findNearest( target, 4, res_result, type );
	cout<<" range nearest : "<<endl;
	for( int i=0; i<num; i++)
	{
		for( col=0; col<dim; col++)
		{
			cout<< res_result[i].first[col]<<"\t";
		}
		cout<<endl;
		cout<<" distance is " << res_result[i].second<<endl;
	}
	system("pause");
	return 0;
}