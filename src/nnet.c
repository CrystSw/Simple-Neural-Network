#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "header/util.h"
#include "header/nnet_types.h"

/**
 * ソフトマックス値を計算する．
 *
 * @param catout - ソフトマックス値の計算に用いる数値が格納された配列
 * @param id - ソフトマックス値を計算したい数値のインデックス
 * @param size - 要素数
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
 * @param rpp - 真の確率分布
 * @param wpp - 誤った確率分布
 * @param size - 要素数
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
 * ある特徴量を入力した際のニューラルネットワークの出力を計算する．(教師データ用)
 *
 * @param td - 教師データ
 * @param weight - 重みデータ
 * @param bias - バイアスデータ
 * @param id - 計算に用いる教師データのインデックス
 * @param catout - 各ニューロンの出力が格納された配列(出力)
 */
void nncout_train(const TRAINDATA td, double **weight, const double *bias, const int id, double *catout){
	int i, j;
	
	for(i = 0; i < td.label.size; ++i){
		catout[i] = 0;
		for(j = 0; j < td.fval.size; ++j){
			catout[i] += weight[i][j]*td.fval.data[id][j];
		}
		catout[i] += bias[i];
	}
}

/**
 * ある特徴量を入力した際のニューラルネットワークの出力を計算する．(テストデータ用)
 *
 * @param td - テストデータ
 * @param weight - 重みデータ
 * @param bias - バイアスデータ
 * @param id - 計算に用いる教師データのインデックス
 * @param catout - 各ニューロンの出力が格納された配列(出力)
 */
void nncout_test(const TESTDATA td, double **weight, const double *bias, const int id, double *catout){
	int i, j;
	
	for(i = 0; i < td.label.size; ++i){
		catout[i] = 0;
		for(j = 0; j < td.fval.size; ++j){
			catout[i] += weight[i][j]*td.fval.data[id][j];
		}
		catout[i] += bias[i];
	}
}

/**
 * ニューラルネットワークの出力を基にカテゴリ事後確率の計算を行う．
 *
 * @param catout - 各ニューロンの出力が格納された配列
 * @param size - カテゴリ数
 * @param catcpp - カテゴリ事後確率各が格納された配列(出力)
 */
void nncpp(const double *catout, const int size, double *catpp){
	int i;
	
	for(i = 0; i < size; ++i){
		catpp[i] = softmax(catout, i, size);
	}
}

/**
 * クロスエントロピーの総和を計算する．
 *
 * @param td - 教師データ
 * @param weight - 重みデータ
 * @param bias - バイアスデータ
 * @return - クロスエントロピーの総和
 */
double xentrloss(const TRAINDATA td, double **weight, const double *bias){
	int i, j, k;
	
	double loss = 0;
	double *catout, *catpp;
	catout = malloc_d1(td.label.size);
	catpp = malloc_d1(td.label.size);
	
	for(i = 0; i < td.fval.num; ++i){
		//ニューラルネットワークの出力を計算
		nncout_train(td, weight, bias, i, catout);
		//カテゴリ事後確率の導出
		nncpp(catout, td.label.size, catpp);
		//クロスエントロピーの計算
		loss += cross_entropy(td.label.data[i], catpp, td.label.size);
	}
	free_d1(catout);
	free_d1(catpp);
	return loss;
}
