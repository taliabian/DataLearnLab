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
	int ki;						/**< �����������������λ�� */
	double kv;					/**< �������������� */
	int leaf;					/**< Ҷ�ڵ�Ϊ1,����Ϊ0 */
	Matrix<Type> features;		/**< �õ����� */
	int n;						/**< ��������� */
	struct kd_node* kd_left;	/**< ���� */
	struct kd_node* kd_right;	/**< �Һ��� */
};

template <typename Type>
class kdTree
{
public:
	kdTree();
	kdTree( Matrix<Type> _x, vector<Type> _y );
	~kdTree();
	/// �����������
	bool computeDistance( vector<Type> x1, vector<Type> x2, double &d, int type = KDDIST_EUCLIDEAN );
	/// ����ƽ��kd��
	kd_node<Type>* CreateKDTree(  );//vector<Type>* features
	/// ��kd��ʵ������
	bool searchKDTree( );
	
	/// �����������ƽ��ֵ
	vector<Type> getAvr();
	/// ������������ƽ��ֵ
	void computeAvr();
	/// �������������׼��
	vector<Type> getStdDev();
	/// ��������������׼��
	void computeStdDec();
	/// ������������Э�������
	Matrix<Type> computeCovariance();
protected:
	/// ��ʼ��kd���ڵ�
	kd_node<Type>* kd_node_init( Matrix<Type> features, int n);
	/// �ӽڵ���չ
	void expand_kd_node_subtree( kd_node<Type>* kd_node );
	///
	void assign_part_key( kd_node<Type>* kd_node );
	/// ��������
	void partition_features( kd_node<Type>* kd_node );
	/// ������λ��
	double median_select( vector<Type> feats );
private:
	vector<Type> avr; // ��������ƽ��ֵ
	vector<Type> stddev; // ����������׼��
	Matrix<Type> x; // ����������
	vector<Type> y; // �������༯
};

#include "kdTree_impl.h"

#endif