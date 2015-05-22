#include "kdtree.h"

using namespace KDTree;

void KDTreeNode::create( KDTreeNode *left_child, KDTreeNode *right_child, 
						KDTreeNode *parent, int split_dim, vector<double> feat, double split_mid, int num_feature )
{
	this->_left_child = left_child;
	this->_right_child = right_child;
	this->_parent = parent;
	this->_split_dim = split_dim;
	this->_feature = feat;
	this->_split_mid = split_mid;
	this->_num_feature = num_feature;
}

KDTreeNode::KDTreeNode( const KDTreeNode &rhs )
{
	this->_left_child = rhs._left_child;
	this->_right_child = rhs._right_child;
	this->_parent = rhs._parent;
	this->_split_dim = rhs._split_dim;
	this->_feature = rhs._feature;
	this->_split_mid = rhs._split_mid;
	this->_num_feature = rhs._num_feature;
}

KDTreeNode& KDTreeNode::operator = ( const KDTreeNode &rhs )
{
	if( this == &rhs)
		return *this;
	this->_left_child = rhs._left_child;
	this->_right_child = rhs._right_child;
	this->_parent = rhs._parent;
	this->_split_dim = rhs._split_dim;
	this->_feature = rhs._feature;
	this->_split_mid = rhs._split_mid;
	this->_num_feature = rhs._num_feature;
	return *this;
}
/// 创建kd树
void KDtree::Create( const vector<vector<double> > &feat )
{
	_root = CreateKDtree( feat );
}
KDTreeNode* KDtree::CreateKDtree( const vector<vector<double> > &feat )
{
	if( feat.empty() )
		return NULL ;
	vector<vector<double> > featcopy = feat;
	int dims = featcopy.at(0).size(); // 数据特征维数
	int size = featcopy.size();				  // 数据点数
	
	// 计算每个维数的方差和方差最大的维
	double var_max = 0;
	double avg, var;
	int dim_max_var=0;
	vector<double> tmp(size);
	for( int i=0; i<dims; i++)
	{
		avg = 0;
		var = 0;
		for( int j=0; j<size; j++)
			avg += featcopy[j][i];
		avg /= size;
		for( int j=0; j<size; j++)
			var += ( featcopy[j][i] - avg)*( featcopy[j][i] - avg);
		var /= size;
		if( var > var_max )
		{
			 var_max = var;
			 dim_max_var = i;  //方差最大维
		}
	}
	
	for( int i=0; i<size; i++)
		tmp.at(i) = featcopy[i][dim_max_var]; // 方差最大维的特征点集
	// 找中位数
	sort( tmp.begin(), tmp.end() );
	int idx;
	double split_mid = tmp.at(size/2);
	for( int i=0; i<tmp.size(); i++)
		if( featcopy[i][dim_max_var] == split_mid ) //中位数对应特征点
		{
			idx = i;
			break;
		}
	vector<double> split = featcopy.at( idx );

	if( idx >=0 && idx < size)
		featcopy.erase( featcopy.begin() + idx);//去除根节点
	else
		return NULL;
	// 确定左右节点
	vector<vector<double> > feat_left(0, dims);
	vector<vector<double> > feat_right(0, dims);

	int size_new = featcopy.size();
	vector<double> temp(dims);
	for( int i=0; i<size_new; i++)
	{
		temp = featcopy[i];
		if( temp[dim_max_var] < split_mid )
			feat_left.push_back( temp );
		else
			feat_right.push_back( temp );
	}

	KDTreeNode *pNewNode = new KDTreeNode(0, 0, 0, dim_max_var, split, split_mid, size-1);
	// 左节点kd树
	pNewNode->_left_child = CreateKDtree( feat_left );
	if(pNewNode->_left_child != NULL)
		pNewNode->_left_child->_parent = pNewNode;
	// 右节点kd树
	pNewNode->_right_child = CreateKDtree(feat_right);
	if(pNewNode->_right_child != NULL)
		pNewNode->_right_child->_parent = pNewNode;
	
	return pNewNode;

}
void KDtree::destroy( )
{
	destroyKDtree(_root);

}
void KDtree::destroyKDtree( KDTreeNode *root)
{
	if( root!= NULL)
	{
		destroyKDtree( root->_left_child );
		destroyKDtree( root->_right_child );
		delete root;
	}
}
/// 查找最近邻点
pair<vector<double>, double> KDtree::findNearest( vector<double> target, int type )
{
	std::pair<vector<double>, double> res;
	if( _root == NULL)
	{
		res.second = std::numeric_limits<double>::infinity();
		return res;
	}
	return findNearest_i( _root, target, type );
}
int KDtree::findNearest( vector<double> target, double range, vector<pair<vector<double>, double>> & res_nearest, int type )
{
	return findNearest_range( _root, target, range, res_nearest, type);
}

pair<vector<double>, double> KDtree::findNearest_i( KDTreeNode *root, vector<double> target, int type )
{
	KDTreeNode *pSearch = root;
	vector<KDTreeNode*> search_path;	// 堆栈,保存搜索路径
	vector<double> nearest;
	double max_dist;
	
	while(pSearch != NULL)
	{
		// 根节点入栈
		search_path.push_back( pSearch );
		int s = pSearch->getSplitDim();
		if( target[s] <= pSearch->getFeature()[s])// 目标点特征值小于节点特征值
			pSearch = pSearch->_left_child; // 左节点入栈
		else 
			pSearch = pSearch->_right_child;// 右节点入栈
	}
	nearest = search_path.back()->getFeature();// 最近点
	max_dist = Distance_exm( nearest, target, type );// 距离

	search_path.pop_back();// 去除当前最近点
	double d1, d2;
	// 回溯搜索路径
	while( !search_path.empty() )
	{
		KDTreeNode *pBack = search_path.back();//栈顶点
		search_path.pop_back();// 出栈

		if( pBack->_left_child == NULL && pBack->_right_child == NULL )
		{
			nearest = pBack->getFeature();
			max_dist = Distance_exm( nearest, target, type );
		}
		else
		{
			int s = pBack->getSplitDim(); 
			if( abs( pBack->getFeature()[s] - target[s]) < max_dist ) // 该点和目标点的特征维距离小于目前最近的最大距离
			{
				d1 = Distance_exm( nearest, target, type );// 最近点和目标点的距离
				d2 = Distance_exm( pBack->getFeature(), target, type );// 当前点和目标点的距离
				if( d1 > d2 )
					max_dist = d2;// 当前点更近
				if( target[s] <= pBack->getFeature()[s])  
					pSearch = pBack->_right_child;
				else
					pSearch = pBack->_left_child;
				if( pSearch != NULL )
					search_path.push_back( pSearch );
			}
		}
	}

	pair<vector<double>, double> res( nearest, max_dist );
	return res;

}
int KDtree::findNearest_range( KDTreeNode *root, vector<double> target, double range, 
								vector<pair<vector<double>, double>> &res_nearest, int type )
{
	if( root == NULL )
		return 0;
	double dist_sq, dx;
	int ret, added_res = 0;
	dist_sq = Distance_exm( root->getFeature(), target ,type);

	if( dist_sq <= range )
	{
		pair<vector<double>, double> temp( root->getFeature(), dist_sq );
		res_nearest.push_back( temp );
		added_res = 1;
	}
	// 目标节点维和根节点维的差
	dx = target[ root->getSplitDim() ] - root->getFeature()[ root->getSplitDim() ];
	// 左右节点选择
	ret = findNearest_range( dx<=0.0 ? root->_left_child : root->_right_child , target, range, res_nearest, type );
	//
	if( ret >=0 && fabs(dx) < range ){
		added_res += ret;
		ret = findNearest_range( dx<=0.0 ? root->_right_child : root->_left_child , target, range, res_nearest, type);
	}
	added_res += ret;
	return added_res;
}

/// 计算度量距离
double KDtree::Distance_exm( vector<double> x1, vector<double> x2 , int type )
{
	// type = 1 欧式距离
	if( x1.size() != x2.size() )
		return 0;
	int size = x1.size();
	
	double d = 0;

	if( type == KDDIST_EUCLIDEAN )
	{
		for( int i=0; i<size; i++)
			d += ( x1.at(i) - x2.at(i) )*( x1.at(i) - x2.at(i) );
		d = sqrt(d);
	}
	// type = 2 曼哈顿距离
	else if( type == KDDIST_CITYBLOCK )
	{
		for( int i=0; i<size; i++)
			d += abs( x1.at(i) - x2.at(i) );
	}
	// 切比雪夫距离
	else if( type == KDDIST_CHEBYSHEV )
	{
		d = abs( x1.at(0) - x2.at(0));
		for( int i=1; i<size; i++)
			if( d < abs( x1.at(i) - x2.at(i) ))
				d = abs( x1.at(i) - x2.at(i) );
	}

	return d;
}

