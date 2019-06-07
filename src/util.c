#include <stdio.h>
#include <stdlib.h>

/**
 * 不定長1次元配列の確保(double)
 * malloc版
 *
 * @param size1 - 1次元目の要素数
 * @return - 確保した領域へのポインタ
 */
double *malloc_d1(const int size1){
	int i;

	double *pointer;
	pointer = (double*)malloc(size1*sizeof(double));
	return pointer;
}


/**
 * 不定長1次元配列の確保(double)
 * calloc版
 *
 * @param size1 - 1次元目の要素数
 * @return - 確保した領域へのポインタ
 */
double *calloc_d1(const int size1){
	int i;

	double *pointer;
	pointer = (double*)calloc(size1, sizeof(double));
	return pointer;
}


/**
 * 不定長2次元配列の確保(double)
 * malloc版
 *
 * @param size1 - 1次元目の要素数
 * @param size2 - 2次元目の要素数
 * @return - 確保した領域へのポインタ
 */
double **malloc_d2(const int size1, const int size2){
	int i;

	double **pointer;
	pointer = (double**)malloc(size1*sizeof(double*));
	for(i = 0; i < size1; ++i){
		pointer[i] = (double*)malloc(size2*sizeof(double));
	}
	return pointer;
}


/**
 * 不定長2次元配列の確保(double)
 * calloc版
 *
 * @param size1 - 1次元目の要素数
 * @param size2 - 2次元目の要素数
 * @return - 確保した領域へのポインタ
 */
double **calloc_d2(const int size1, const int size2){
	int i;

	double **pointer;
	pointer = (double**)calloc(size1, sizeof(double*));
	for(i = 0; i < size1; ++i){
		pointer[i] = (double*)calloc(size2, sizeof(double));
	}
	return pointer;
}


/**
 * 不定長1次元配列の解放(double)
 * プログラム中の関数名の統一のために用意しています．
 * 内容はfree()を利用した場合と同じです．
 *
 * @param array - 	1次元配列のポインタ
 */
void free_d1(double *array){
	free(array);
}


/**
 * 不定長2次元配列の解放(double)
 *
 * @param array - 2次元配列のポインタ
 * @param size1 - 1次元目の要素数
 */
void free_d2(double **array, const int size1){
	int i;

	for(i = 0; i < size1; ++i){
		free(array[i]);
	}
	free(array);
}


/**
 * 配列内で最も値の大きい要素のインデックスを取得する(double)
 *
 * @param array - 1次元配列のポインタ
 * @param size1 - 1次元目の要素数
 * @return - インデックス
 */
int getmax(const double *array, int size1){
	int i;
	
	int max = 0;
	for(i = 1; i < size1; ++i){
		if(array[i] > array[max]) max = i;
	}
	return max;
}


/**
 * 1次元配列をファイルへ出力する(double)
 *
 * @param filename - ファイル名
 * @param array_d1 - 1次元配列のポインタ
 * @param size1 - 1次元目の要素数
 */
void writefile_d1(const char *filename, const double *array_d1, const int size1){
	int i;
	
	FILE *wfp;
	if((wfp = fopen(filename,"w")) != NULL){
		for(i = 1; i < size1; ++i){
			fprintf(wfp, "%.15f\n", array_d1[i]);
		}
	}else{
		fprintf(stderr, "Error: Cannot open file.\n");
		exit(1);
	}
	fclose(wfp);
}


/**
 * ファイルから1次元配列をロードする(double)
 *
 * @param filename - ファイル名
 * @param array_d1 - 1次元配列のポインタ
 * @param size1 - 1次元目の要素数
 */
void readfile_d1(const char *filename, double *array_d1, const int size1){
	int i;
	
	FILE *rfp;
	if((rfp = fopen(filename,"r")) != NULL){
		for(i = 0; i < size1; ++i){
			fscanf(rfp, "%lf", &array_d1[i]);
		}
	}else{
		fprintf(stderr, "Error: Cannot open file.\n");
		exit(1);
	}
	fclose(rfp);
}


/**
 * 2次元配列をファイルへ出力する(double)
 *
 * @param filename - ファイル名
 * @param array_d2 - 2次元配列のポインタ
 * @param size1 - 1次元目の要素数
　* @param size2 - 2次元目の要素数
 */
void writefile_d2(const char *filename, const double **array_d2, const int size1, const int size2){
	int i, j;
	
	FILE *wfp;
	if((wfp = fopen(filename,"w")) != NULL){
		for(i = 0; i < size1; ++i){
			for(j = 0; j < size2; ++j){
				fprintf(wfp, "%.15f\n", array_d2[i][j]);
			}
		}
	}else{
		fprintf(stderr, "Error: Cannot open file.\n");
		exit(1);
	}
	fclose(wfp);
}


/**
 * ファイルから2次元配列をロードする(double)
 *
 * @param filename - ファイル名
 * @param array_d2 - 2次元配列のポインタ
 * @param size1 - 1次元目の要素数
 */
void readfile_d2(const char *filename, double **array_d2, const int size1, const int size2){
	int i, j;
	
	FILE *rfp;
	if((rfp = fopen(filename,"r")) != NULL){
		for(i = 0; i < size1; ++i){
			for(j = 0; j < size2; ++j){
				fscanf(rfp, "%lf", &array_d2[i][j]);
			}
		}
	}else{
		fprintf(stderr, "Error: Cannot open file.\n");
		exit(1);
	}
	fclose(rfp);
}