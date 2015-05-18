
template <typename Type>
kdTree<Type>::kdTree()
{
	avr = vector<Type>(0); 
	stddev = vector<Type>(0);
	x = Matrix<Type>(0,0);
	y = vector<Type>(0); 
}

template <typename Type>
kdTree<Type>::kdTree(  Matrix<Type> _x, vector<Type> _y )
{
	int rows = _x.rows();
	int cols = _x.cols();
	avr = vector<Type>( cols ); 
	stddev = vector<Type>( cols );
	x = _x;
	y = _y; 
}

template <typename Type>
kdTree<Type>::~kdTree()
{

}

/// 计算度量距离
template <typename Type>
bool kdTree<Type>::computeDistance( vector<Type> x1, vector<Type> x2, double &d, int type=KDDIST_EUCLIDEAN )
{
	// type = 1 欧式距离
	if( x1.size() != x2.size() )
		return 0;
	int size = x1.size();
	d = 0;

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
	// 标准欧式距离
	else if( type == KDDIST_STANDEUCLIDEAN)
	{
		// 样本特征的标准差
		computeStdDec();
		d = 0;
		for( int i=0; i<size; i++ )
			d += (( x1.at(i) - x2.at(i) ) / stddev.at(i))*(( x1.at(i) - x2.at(i) ) / stddev.at(i));
		d = sqrt(d);
	}
}

/// 创建平衡kd树
template <typename Type>
kd_node<Type>* kdTree<Type>::CreateKDTree(  )//vector<Type>* features
{
	int n = x.rows(); //特征点个数
	struct kd_node<Type>* kd_root;
	if( !x || n<=0)
	{
		cout << "warning: CreateKDtree(): no features \n";
		return NULL;
	}

	kd_root = kd_node_init( x, n ); /// 初始化根节点
	
	expand_kd_node_subtree( kd_root );
	
	return kd_root;
}
/// 初始化kd树节点
template <typename Type>
kd_node<Type>* kdTree<Type>::kd_node_init( Matrix<Type> features, int n)
{
	kd_node<Type> *kdnode = new kd_node<Type>;
  //  kdnode = malloc( sizeof( kd_node<Type> ) );
	memset( kdnode, 0, sizeof( kd_node<Type> ) );
	kdnode->ki = -1;
	kdnode->features = features;
	kdnode->n = n;

	return kdnode;
}
/// 子节点扩展
template <typename Type>
void kdTree<Type>::expand_kd_node_subtree( kd_node<Type>* kd_node )
{
	if( kd_node->n == 1 || kd_node->n == 0)   /// 叶节点
	{
		kd_node->leaf = 1;
		return;
	}
	assign_part_key( kd_node );
	partition_features( kd_node );

	if( kd_node->kd_left )	/// 左节点
		expand_kd_node_subtree( kd_node->kd_left );
	if( kd_node->kd_right )	/// 右节点
		expand_kd_node_subtree( kd_node->kd_right );
}
///
template <typename Type>
void kdTree<Type>::assign_part_key( kd_node<Type>* kd_node )
{
	Matrix<Type> features;
	double kv, x, mean, var, var_max = 0;

	int d, n, i, j, ki = 0;

	features = kd_node->features;
	n = kd_node->n;
	d = features.cols(); //每个特征点的特征个数
	
	vector<double> tmp( n );

	for( j=0; j<d; j++)
	{
		mean = var = 0;
		for( i=0; i<n; i++) 
			mean += features[i][j];
		mean /= n;	// 每个特征的平均值
		for( i=0; i<n; i++ )
		{
			x = features[i][j] - mean;
			var += x*x;
		}
		var /= n;	// 每个特征的方差
		if( var > var_max )
		{
			ki = j;	//选择的特征区分项
			var_max = var;	
		}
	}
	for( i=0; i<n; i++)
		tmp.at(i) = features[i][ki];
	kv = median_select( tmp ); // 找中位数位置
	
	kd_node->ki = ki;
	kd_node->kv = kv;
}

/// 特征中位数
template <typename Type>
double kdTree<Type>::median_select( vector<Type> feats )
{
	vector<Type> tmp = feats;
	double t;
	sort( tmp.begin(), tmp.end() );
	int index = int(feats.size()/2);
	return tmp.at(index);
}

/// 特征分类
template <typename Type>
void kdTree<Type>::partition_features( kd_node<Type>* kd_node )
{
	Matrix<Type> features;
	vector<Type> tmp;
	double kv;
	int n, ki, p, i, j = -1;

	features = kd_node->features;
	n = kd_node->n;
	ki = kd_node->ki;
	kv = kd_node->kv;

	for( i=0; i<n; i++)
		if( features[i][ki] <= Type(kv) )
		{
			features = ExchangeRows( features, ++j, i );
			if( features[j][ki] == kv)
				p = j;
		}
	features = ExchangeRows( features, j, p );
	if( j == n-1 )
	{
		kd_node->leaf = 1;
		return;
	}

	kd_node->kd_left = kd_node_init( getMatrixFromRows( features, 0, j), j+1); //左节点
	kd_node->kd_right = kd_node_init( getMatrixFromRows( features, j+1, n-1 ), n-j-1 ); // 右节点
}

/// 用kd数实现搜索
template <typename Type>
bool kdTree<Type>::searchKDTree( )
{

}

/// 获得样本平均值
template <typename Type>
vector<Type> kdTree<Type>::getAvr()
{
	return avr;
}
/// 计算样本特征平均值
template <typename Type>
void kdTree<Type>::computeAvr()
{
	int rows = x.rows();
	int cols = x.cols();
	for( int i=0; i<cols; i++)
	{
		avr[i] = 0;
		for( int j=0; j<rows; j++)
			avr[i] += x[j][i];
		avr[i] = avr[i]/rows;
	}
	cout << "avr" << avr;
}

/// 获得样本特征标准差
template <typename Type>
vector<Type> kdTree<Type>::getStdDev()
{
	return stddev;
}

/// 计算样本特征标准差
template <typename Type>
void kdTree<Type>::computeStdDec()
{
	int rows = x.rows();
	int cols = x.cols();
	computeAvr();
	for( int i=0; i<cols; i++)
	{
		stddev[i] = 0;
		for( int j=0; j<rows; j++)
			stddev[i] += (x[j][i] - avr[i])*(x[j][i] - avr[i]);
		stddev[i] /= rows;
		stddev[i] = sqrt(stddev[i]);
	}
	cout << "stddev" << stddev << endl;		 
}

/// 计算样本特征协方差矩阵
template <typename Type>
Matrix<Type> kdTree<Type>::computeCovariance()
{
	int rows = x.cols();
	Matrix<Type> con( rows, rows );
	con = trMult( x, x) / ( x.rows() - 1 );
	cout << "convariance" << con;
	return con;
}