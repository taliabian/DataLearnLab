#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include <cmath>
#include "matrix.h"

using namespace std;
using namespace matrixlab;

#define KDDIST_EUCLIDEAN 1
#define KDDIST_CITYBLOCK 2
#define KDDIST_CHEBYSHEV 3
#define KDDIST_STANDEUCLIDEAN 4


template <typename Type>
struct kd_node
{
	int ki;						/**< 特征点特征方差最大位置 */
	double kv;					/**< 特征点特征中数 */
	int leaf;					/**< 叶节点为1,否则为0 */
	Matrix<Type> features;		/**< 该点特征 */
	int n;						/**< 特征点个数 */
	struct kd_node* kd_left;	/**< 左孩子 */
	struct kd_node* kd_right;	/**< 右孩子 */
};

template <typename Type>
class kdTree
{
public:
	kdTree();
	kdTree( Matrix<Type> _x, vector<Type> _y );
	~kdTree();
	/// 计算度量距离
	bool computeDistance( vector<Type> x1, vector<Type> x2, double &d, int type = KDDIST_EUCLIDEAN );
	/// 创建平衡kd树
	kd_node<Type>* CreateKDTree(  );//vector<Type>* features
	/// 用kd数实现搜索
	bool searchKDTree( );
	
	/// 获得样本特征平均值
	vector<Type> getAvr();
	/// 计算样本特征平均值
	void computeAvr();
	/// 获得样本特征标准差
	vector<Type> getStdDev();
	/// 计算样本特征标准差
	void computeStdDec();
	/// 计算样本特征协方差矩阵
	Matrix<Type> computeCovariance();
protected:
	/// 初始化kd树节点
	kd_node<Type>* kd_node_init( Matrix<Type> features, int n);
	/// 子节点扩展
	void expand_kd_node_subtree( kd_node<Type>* kd_node );
	///
	void assign_part_key( kd_node<Type>* kd_node );
	/// 特征分类
	void partition_features( kd_node<Type>* kd_node );
	/// 特征中位数
	double median_select( vector<Type> feats );
private:
	vector<Type> avr; // 样本特征平均值
	vector<Type> stddev; // 样本特征标准差
	Matrix<Type> x; // 样本特征集
	vector<Type> y; // 样本归类集
};

#include "kdTree_impl.h"

#endif