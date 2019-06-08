#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "header/nnet.h"
#include "header/util.h"
#include "header/mnist.h"

#define LOSS_CALC_RATE	5000

void learn(void);
void test(void);

int main(int argc, char *argv[]){
	if(argc > 1){
		if(!strcmp(argv[1],"-l")){
			learn();
		}else if(!strcmp(argv[1],"-t")){
			test();
		}
	}
	return 0;
}

/**
 * ニューラルネットワークの学習を行う．
 * 最終的な重みとバイアスは外部ファイルとして保存される．
 */
void learn(void){
	int i, j, k;
	
	//訓練データの読み込み
	TRAINDATA td;
	td.fval = read_mnistimg("./mnist/train-images.idx3-ubyte");
	puts("Image data has loaded.");
	td.label = read_mnistlbl_ohv("./mnist/train-labels.idx1-ubyte");
	puts("Label data has loaded.");
	
	//学習率(大きくし過ぎるとdoubleの上限を超えます)
	double alpha = 0.0000001;
	
	//重みとバイアスの宣言(callocで確保します)
	double **weight = calloc_d2(td.label.size, td.fval.size);
	double *bias = calloc_d1(td.label.size);
	
	//学習前のクロスエントロピーの総和
	printf("Data: %6d, Loss: %.8f\n", 0, xentrloss(td, weight, bias));
	
	//カテゴリ出力とカテゴリ事後確率の宣言
	double *catout = malloc_d1(td.label.size);
	double *catpp = malloc_d1(td.label.size);
	
	//学習
	for(i = 0; i < td.fval.num; ++i){
		//ニューラルネットワークの出力を計算
		nncout_train(td, weight, bias, i, catout);
		//カテゴリ事後確率の導出
		nncpp(catout, td.label.size, catpp);
		//更新
		for(j = 0; j < td.label.size; ++j){
			//重みの更新
			for(k = 0; k < td.fval.size; ++k){
				weight[j][k] += alpha*((td.label.data[i][j]-catpp[j])*td.fval.data[i][k]);
			}
			//バイアスの更新
			bias[j] += alpha*(td.label.data[i][j]-catpp[j]);
		}
		//クロスエントロピー誤差(計算負荷の高い処理なので，実行回数は少なめ)
		if((i+1) % LOSS_CALC_RATE == 0) printf("Data: %6d, Loss: %.8f\n", i+1, xentrloss(td, weight, bias));
	}
	free_d1(catout);
	free_d1(catpp);
	
	//重みの書き出し
	writefile_d2("./result/weight-value", weight, td.label.size, td.fval.size);	
	//バイアスの書き出し
	writefile_d1("./result/bias-value", bias, td.label.size);
	
	free_d2(weight, td.label.size);
	free_d1(bias);
}

/**
 * 学習したニューラルネットワークを用いてテストデータの分類を行う．
 * 結果は混同行列として出力される．
 */
void test(void){
	int i, j, k;
	
	//学習データの読みこみ
	TESTDATA td;
	td.fval = read_mnistimg("./mnist/t10k-images.idx3-ubyte");
	puts("Image data has loaded.");
	td.label = read_mnistlbl("./mnist/t10k-labels.idx1-ubyte");
	puts("Label data has loaded.");

	//重みとバイアスの宣言(callocで確保します)
	double **weight = calloc_d2(td.label.size, td.fval.size);
	double *bias = calloc_d1(td.label.size);
	
	//重みの読み込み
	readfile_d2("./result/weight-value", weight, td.label.size, td.fval.size);
	//バイアスの読み込み
	readfile_d1("./result/bias-value", bias, td.label.size);
	
	//混同行列
	double **confmat = calloc_d2(td.label.size, td.fval.size);
	
	//カテゴリ出力とカテゴリ事後確率の宣言
	double *catout = malloc_d1(td.label.size);
	double *catpp = malloc_d1(td.label.size);
	
	//カテゴリ分類
	int m_category;
	for(i = 0; i < td.fval.num; ++i){
		//ニューラルネットワークの出力を計算
		nncout_test(td, weight, bias, i, catout);
		//カテゴリ事後確率の導出
		nncpp(catout, td.label.size, catpp);
		//事後確率が最大であるカテゴリを導出
		m_category = getmax(catpp, td.label.size);
		//分類結果を格納(混同行列の計算に利用)
		confmat[td.label.data[i]][m_category] += 1;
		//分類結果を出力
		printf("[Data:%d]truth:%d predict:%d(pp:%.1f)\n", i+1, td.label.data[i], m_category, catpp[m_category]*100);
	}
	free_d1(catout);
	free_d1(catpp);
	
	//混同行列の計算
	int cat_num;
	for(i = 0; i < td.label.size; ++i){
		cat_num = 0;
		for(j = 0; j < td.label.size; ++j) cat_num += confmat[i][j];
		for(j = 0; j < td.label.size; ++j) confmat[i][j] /= cat_num;
	}
	//混同行列の出力
	puts("-----Confusion Matrix-----");
	printf("   ");
	for(i = 0; i < td.label.size; ++i) printf("%4d  ", i);
	putchar('\n');
	for(i = 0; i < td.label.size; ++i){
		printf("%d: ", i);
		for(j = 0; j < td.label.size; ++j){
			printf("%3.1f   ", confmat[i][j]*100);
		}
		putchar('\n');
	}
	
	//正解率の出力
	puts("-----Accuracy Rate-----");
	double arate = 0;
	for(i = 0; i < td.label.size; ++i){
		arate += confmat[i][i];
	}
	arate /= td.label.size;
	printf("%3.1f\n", arate*100);
	
	free_d2(weight, td.label.size);
	free_d1(bias);
	free_d2(confmat, td.label.size);
}
