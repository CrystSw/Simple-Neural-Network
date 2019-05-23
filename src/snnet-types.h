typedef unsigned char u_char;

typedef struct {
	int num;				//データ数
	int r_size;				//行ベクトルの次元(要素数)
	int c_size;				//列ベクトルの次元(要素数)
	int size;				//全次元数(要素数)
	unsigned char **data;	//特徴データ
} FVALUE;

typedef struct {
	int num;		//データ数
	int size;		//カテゴリ数
	double **data;	//ラベルデータ(理想事後確率)
} LABELOHV;

typedef struct {
	int num;		//データ数
	int size;		//カテゴリ数
	unsigned char *data;	//ラベルデータ
} LABEL;

typedef struct{
	FVALUE fval;
	LABELOHV label;
} TRAINDATA;

typedef struct{
	FVALUE fval;
	LABEL label;
} TESTDATA;
