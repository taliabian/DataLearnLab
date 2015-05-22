#ifndef KDTREE_H
#define KDTREE_H
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define KDDIST_EUCLIDEAN 1
#define KDDIST_CITYBLOCK 2
#define KDDIST_CHEBYSHEV 3

namespace KDTree
{
	class KDTreeNode
	{
	private:
		int _split_dim;				/// 分割超平面的方向轴序号
		vector<double> _feature;	/// 数据集中某个数据点向量
		double	_split_mid;		/// 数据点分割超平面所在维的数据向量的中数
		int _num_feature;		/// 该节点的特征数
	public:
		KDTreeNode *_left_child, *_right_child, *_parent;/// 左右子节点, 父节点

	public:

		KDTreeNode():_left_child(0), _right_child(0), _parent(0), _split_mid(0), _num_feature(0){}
		KDTreeNode( KDTreeNode *left_child, KDTreeNode *right_child, 
					KDTreeNode *parent, int split_dim, vector<double> feat, double split_mid, int num_feature):
					_left_child( left_child ), _right_child( right_child ), _parent(parent),
					_split_dim( split_dim ), _feature( feat ), _split_mid( split_mid ), _num_feature(num_feature){ }

		KDTreeNode( const KDTreeNode &rhs );
		KDTreeNode& operator = ( const KDTreeNode &rhs );

		vector<double>& getFeature( ){ return _feature; }
		double getSplitMid( ){ return _split_mid; }
		int& getSplitDim( ){ return _split_dim; }
		int getNodeNumFeat( ){ return _num_feature; }

		void create( KDTreeNode *left_child, KDTreeNode *right_child,
			KDTreeNode *parent, int split_dim, vector<double> feat, double split_mid, int num_feature );
	};

	class KDtree	/// kd tree 类定义
	{
	public:
		KDTreeNode *_root;		/// kd树根节点
	public:
		KDtree():_root(NULL){};
		
		void Create( const vector<vector<double> > &feat );	/// 创建kd树
		void destroy( );										/// 销毁kd树
		~KDtree(){ destroyKDtree(_root); }

		pair<vector<double>, double> findNearest( vector<double> target, int type );	/// 查找最近邻点
		int findNearest( vector<double> target, double range, vector<pair<vector<double>, double>> &res_nearest, int type);
		double KDtree::Distance_exm( vector<double> x1, vector<double> x2 , int type);
	
	private:
		KDTreeNode* CreateKDtree( const vector<vector<double> > &feat );	/// 创建kd树
		void destroyKDtree( KDTreeNode *root);
		pair<vector<double>, double> findNearest_i( KDTreeNode *root, vector<double> target, int type=KDDIST_EUCLIDEAN );
		int findNearest_range( KDTreeNode *root, vector<double> target, double range, 
								vector<pair<vector<double>, double>> &res_nearest, int type );
	};
}

#endif