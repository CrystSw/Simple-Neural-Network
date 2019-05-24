#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "snnet-types.h"

/**
 * mnistの画像データをロードする．
 *
 * @arg filename - ファイル名
 * @return - 特徴量データ
 */
FVALUE read_mnistimg(char *filename) {
	int i, j;
	
	FVALUE fval;
	FILE *mnistimg;
	//画像データのロード
	if((mnistimg = fopen(filename, "rb")) == NULL){
		fprintf(stderr, "Error: Cannot open file.");
		exit(1);
	}
	//ヘッダ部の読み込み
	u_char header[16];
	if(fread(header, sizeof(u_char), 16, mnistimg) != 16){
		fprintf(stderr, "Error: Invalid file format.");
		exit(1);
	}
	fval.num = (header[4] << 24) + (header[5] << 16) + (header[6] << 8) + header[7];
	fval.r_size = ((header[8] << 24) + (header[9] << 16) + (header[10] << 8) + header[11]);
	fval.c_size = ((header[12] << 24) + (header[13] << 16) + (header[14] << 8) + header[15]);
	fval.size = fval.r_size*fval.c_size;
	//画像データの読み込み
	fval.data = (u_char**)malloc(sizeof(u_char*)*fval.num);
	for(i = 0; i < fval.num; ++i) fval.data[i] = (u_char*)malloc(sizeof(u_char)*fval.size);
	for(i = 0; i < fval.num; ++i){
		for(j = 0; j < fval.size; ++j){
			if(fread(&fval.data[i][j], sizeof(u_char), 1, mnistimg) != 1){
				fprintf(stderr, "Error: Invalid file format.");
				exit(1);
			}
		}
	}
	fclose(mnistimg);
	
	return fval;
}

/**
 * mnistのラベルデータをロードする．
 * この関数はラベルデータをone-hot-vector形式で読み込む．
 *
 * @arg filename - ファイル名
 * @return - ラベルデータ(one-hot-vector形式)
 */
LABELOHV read_mnistlbl_ohv(char *filename) {
	int i, j;
	
	LABELOHV label;
	FILE *mnistlbl;
	//ラベルデータのロード
	if((mnistlbl = fopen(filename, "rb")) == NULL){
		fprintf(stderr, "Error: Cannot open file.");
		exit(1);
	}
	//ヘッダ部の読み込み
	u_char header[8];
	if(fread(header, sizeof(u_char), 8, mnistlbl) != 8){
		fprintf(stderr, "Error: Invalid file format.");
		exit(1);
	}
	label.num = (header[4] << 24) + (header[5] << 16) + (header[6] << 8) + header[7];
	label.size = 10;	//カテゴリ数
	//ラベルデータの読み込み
	label.data = (double**)malloc(sizeof(double*)*label.num);
	for(i = 0; i < label.num; ++i) label.data[i] = (double*)malloc(sizeof(double)*label.size);
	//one-hot-vector形式へ変換
	u_char category;
	for(i = 0; i < label.num; ++i){
		if(fread(&category, sizeof(u_char), 1, mnistlbl) != 1){
			fprintf(stderr, "Error: Invalid file format.");
			exit(1);
		}
		for(j = 0; j < label.size; ++j){
			label.data[i][j] = (double)(j == category ? 1 : 0);
		}
	}
	fclose(mnistlbl);
	
	return label;
}

/**
 * mnistのラベルデータをロードする．
 *
 * @arg filename - ファイル名
 * @return - ラベルデータ
 */
LABEL read_mnistlbl(char *filename) {
	int i;
	
	LABEL label;
	FILE *mnistlbl;
	//ラベルデータのロード
	if((mnistlbl = fopen(filename, "rb")) == NULL){
		fprintf(stderr, "Error: Cannot open file.");
		exit(1);
	}
	//ヘッダ部の読み込み
	u_char header[8];
	if(fread(header, sizeof(u_char), 8, mnistlbl) != 8){
		fprintf(stderr, "Error: Invalid file format.");
		exit(1);
	}
	label.num = (header[4] << 24) + (header[5] << 16) + (header[6] << 8) + header[7];
	label.size = 10;
	//ラベルデータの読み込み
	label.data = (u_char*)malloc(sizeof(u_char)*label.num);
	for(i = 0; i < label.num; ++i){
		if(fread(&label.data[i], sizeof(u_char), 1, mnistlbl) != 1){
			fprintf(stderr, "Error: Invalid file format.");
			exit(1);
		}
	}
	fclose(mnistlbl);
	
	return label;
}

/**
 * ソフトマックス値を計算する．
 *
 * @arg catout - ソフトマックス値の計算に用いる数値が格納された配列
 * @arg id - ソフトマックス値を計算したい数値のインデックス
 * @arg size - 要素数
 * @return - ソフトマックス値
 */
double softmax(const double *output, const int id, const int size){
	int i;
	
	double denom = 0.0;
	for(i = 0; i < size; ++i){
		denom += exp(output[i]);
	}
	return exp(output[id]) / denom;
}

/**
 * クロスエントロピー値を計算する．
 *
 * @arg rpp - 真の確率分布
 * @arg wpp - 誤った確率分布
 * @arg size - 要素数
 * @return - クロスエントロピー値
 */
double cross_entropy(const double *rpp, const double *wpp, const int size){
	int i;
	
	double cross_emt = 0.0;
	for(i = 0; i < size; ++i){
		//カテゴリ事後確率が0の場合，log0が計算できないので，double型の最小値dbl_min(>0)で計算
		cross_emt += -rpp[i]*log((wpp[i] != 0 ? wpp[i] : DBL_MIN));
	}
	return cross_emt;
}

/**
 * 配列内で最も値の大きい要素のインデックスを取得する．
 *
 * @arg p - 配列
 * @arg size - 要素数
 * @return - インデックス
 */
int getmax(const double *p, int size){
	int i;
	
	int max = 0;
	for(i = 1; i < size; ++i){
		if(p[i] > p[max]) max = i;
	}
	return max;
}

/**
 * ある特徴量を入力した際のニューラルネットワークの出力を計算する．
 * この関数の処理結果として得られたポインタは，必ずfree関数を用いて解放してください．
 *
 * @arg td - 教師データ
 * @arg weight - 重みデータ
 * @arg bias - バイアスデータ
 * @arg id - 計算に用いる教師データのインデックス
 * @return - 各ニューロンの出力が格納された配列(double配列へのポインタ)
 */
double *nncout(const TRAINDATA td, double **weight, const double *bias, const int id){
	int i, j;
	
	double *catout;
	catout = (double*)malloc(sizeof(double)*td.label.size);
	
	for(i = 0; i < td.label.size; ++i){
		catout[i] = 0;
		for(j = 0; j < td.fval.size; ++j){
			catout[i] += weight[i][j]*td.fval.data[id][j];
		}
		catout[i] += bias[i];
	}
	return catout;
}

/**
 * ある特徴量を入力した際のニューラルネットワークの出力を計算する．
 * この関数の処理結果として得られたポインタは，必ずfree関数を用いて解放してください．
 *
 * @arg td - テストデータ
 * @arg weight - 重みデータ
 * @arg bias - バイアスデータ
 * @arg id - 計算に用いる教師データのインデックス
 * @return - 各ニューロンの出力が格納された配列(double配列へのポインタ)
 */
double *nncout_test(const TESTDATA td, double **weight, const double *bias, const int id){
	int i, j;
	
	double *catout;
	catout = (double*)malloc(sizeof(double)*td.label.size);
	
	for(i = 0; i < td.label.size; ++i){
		catout[i] = 0;
		for(j = 0; j < td.fval.size; ++j){
			catout[i] += weight[i][j]*td.fval.data[id][j];
		}
		catout[i] += bias[i];
	}
	return catout;
}

/**
 * ニューラルネットワークの出力を基にカテゴリ事後確率の計算を行う．
 * この関数の処理結果として得られたポインタは，必ずfree関数を用いて解放してください．
 *
 * @arg catout - 各ニューロンの出力が格納された配列
 * @arg size - カテゴリ数
 * @return - カテゴリ事後確率各が格納された配列(double配列へのポインタ)
 */
double *nncpp(const double *catout, const int size){
	int i;
	
	double *catpp;
	catpp = (double*)malloc(sizeof(double)*size);
	
	for(i = 0; i < size; ++i){
		catpp[i] = softmax(catout, i, size);
	}
	return catpp;
}

/**
 * クロスエントロピーの総和を計算する．
 *
 * @arg td - 教師データ
 * @arg weight - 重みデータ
 * @arg bias - バイアスデータ
 * @return - クロスエントロピーの総和
 */
double xentrloss(const TRAINDATA td, double **weight, const double *bias){
	int i, j, k;
	
	double loss = 0;
	double *catout, *catpp;
	
	for(i = 0; i < td.fval.num; ++i){
		//ニューラルネットワークの出力を計算
		catout = nncout(td, weight, bias, i);
		//カテゴリ事後確率の導出
		catpp = nncpp(catout, td.label.size);
		//クロスエントロピーの計算
		loss += cross_entropy(td.label.data[i], catpp, td.label.size);
		
		free(catout);
		free(catpp);
	}
	return loss;
}