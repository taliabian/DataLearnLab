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
/// ����kd��
void KDtree::Create( const vector<vector<double> > &feat )
{
	_root = CreateKDtree( feat );
}
KDTreeNode* KDtree::CreateKDtree( const vector<vector<double> > &feat )
{
	if( feat.empty() )
		return NULL ;
	vector<vector<double> > featcopy = feat;
	int dims = featcopy.at(0).size(); // ��������ά��
	int size = featcopy.size();				  // ���ݵ���
	
	// ����ÿ��ά���ķ���ͷ�������ά
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
			 dim_max_var = i;  //�������ά
		}
	}
	
	for( int i=0; i<size; i++)
		tmp.at(i) = featcopy[i][dim_max_var]; // �������ά�������㼯
	// ����λ��
	sort( tmp.begin(), tmp.end() );
	int idx;
	double split_mid = tmp.at(size/2);
	for( int i=0; i<tmp.size(); i++)
		if( featcopy[i][dim_max_var] == split_mid ) //��λ����Ӧ������
		{
			idx = i;
			break;
		}
	vector<double> split = featcopy.at( idx );

	if( idx >=0 && idx < size)
		featcopy.erase( featcopy.begin() + idx);//ȥ�����ڵ�
	else
		return NULL;
	// ȷ�����ҽڵ�
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
	// ��ڵ�kd��
	pNewNode->_left_child = CreateKDtree( feat_left );
	if(pNewNode->_left_child != NULL)
		pNewNode->_left_child->_parent = pNewNode;
	// �ҽڵ�kd��
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
/// ��������ڵ�
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
	vector<KDTreeNode*> search_path;	// ��ջ,��������·��
	vector<double> nearest;
	double max_dist;
	
	while(pSearch != NULL)
	{
		// ���ڵ���ջ
		search_path.push_back( pSearch );
		int s = pSearch->getSplitDim();
		if( target[s] <= pSearch->getFeature()[s])// Ŀ�������ֵС�ڽڵ�����ֵ
			pSearch = pSearch->_left_child; // ��ڵ���ջ
		else 
			pSearch = pSearch->_right_child;// �ҽڵ���ջ
	}
	nearest = search_path.back()->getFeature();// �����
	max_dist = Distance_exm( nearest, target, type );// ����

	search_path.pop_back();// ȥ����ǰ�����
	double d1, d2;
	// ��������·��
	while( !search_path.empty() )
	{
		KDTreeNode *pBack = search_path.back();//ջ����
		search_path.pop_back();// ��ջ

		if( pBack->_left_child == NULL && pBack->_right_child == NULL )
		{
			nearest = pBack->getFeature();
			max_dist = Distance_exm( nearest, target, type );
		}
		else
		{
			int s = pBack->getSplitDim(); 
			if( abs( pBack->getFeature()[s] - target[s]) < max_dist ) // �õ��Ŀ��������ά����С��Ŀǰ�����������
			{
				d1 = Distance_exm( nearest, target, type );// ������Ŀ���ľ���
				d2 = Distance_exm( pBack->getFeature(), target, type );// ��ǰ���Ŀ���ľ���
				if( d1 > d2 )
					max_dist = d2;// ��ǰ�����
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
	// Ŀ��ڵ�ά�͸��ڵ�ά�Ĳ�
	dx = target[ root->getSplitDim() ] - root->getFeature()[ root->getSplitDim() ];
	// ���ҽڵ�ѡ��
	ret = findNearest_range( dx<=0.0 ? root->_left_child : root->_right_child , target, range, res_nearest, type );
	//
	if( ret >=0 && fabs(dx) < range ){
		added_res += ret;
		ret = findNearest_range( dx<=0.0 ? root->_right_child : root->_left_child , target, range, res_nearest, type);
	}
	added_res += ret;
	return added_res;
}

/// �����������
double KDtree::Distance_exm( vector<double> x1, vector<double> x2 , int type )
{
	// type = 1 ŷʽ����
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
	// type = 2 �����پ���
	else if( type == KDDIST_CITYBLOCK )
	{
		for( int i=0; i<size; i++)
			d += abs( x1.at(i) - x2.at(i) );
	}
	// �б�ѩ�����
	else if( type == KDDIST_CHEBYSHEV )
	{
		d = abs( x1.at(0) - x2.at(0));
		for( int i=1; i<size; i++)
			if( d < abs( x1.at(i) - x2.at(i) ))
				d = abs( x1.at(i) - x2.at(i) );
	}

	return d;
}

