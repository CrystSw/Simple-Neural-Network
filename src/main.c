#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "snnet.h"

#define LOSS_CALC_RATE	5000
#define OUTPUT_IMAGE	1

void learn(void);
void test(void);

int main(int argc, char *argv[]){
	if(argc > 1){
		if(!strcmp(argv[1],"learn")){
			learn();
		}else if(!strcmp(argv[1],"test")){
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
	int i,j,k;
	
	//訓練データの読み込み
	TRAINDATA td;
	td.fval = read_mnistimg("./mnist/train-images.idx3-ubyte");
	puts("Image data has loaded.");
	td.label = read_mnistlbl_ohv("./mnist/train-labels.idx1-ubyte");
	puts("Label data has loaded.");
	
	//学習率(大きくし過ぎるとdoubleの上限を超えます)
	double alpha = 0.0000001;
	
	//重みとバイアスの宣言(callocで確保します)
	double **weight, *bias;
	weight = (double**)calloc(td.label.size, sizeof(double*));
	for(j = 0; j < td.label.size; ++j){
		weight[j] = (double*)calloc(td.fval.size, sizeof(double));
	}
	bias = (double*)calloc(td.label.size, sizeof(double));
	
	//学習前のクロスエントロピーの総和
	printf("Data: %6d, Loss: %.8f\n", 0, xentrloss(td, weight, bias));
	
	//カテゴリ出力とカテゴリ事後確率の宣言
	double *catout, *catpp;
	catout = (double*)malloc(sizeof(double)*td.label.size);
	catpp = (double*)malloc(sizeof(double)*td.label.size);
	
	//学習
	for(i = 0; i < td.fval.num; ++i){
		//ニューラルネットワークの出力を計算
		catout = nncout(td, weight, bias, i);
		//カテゴリ事後確率の導出
		catpp = nncpp(catout, td.label.size);
		//重みの更新
		for(j = 0; j < td.label.size; ++j){
			for(k = 0; k < td.fval.size; ++k){
				weight[j][k] += alpha*((td.label.data[i][j]-catpp[j])*td.fval.data[i][k]);
			}
		}
		//バイアスの更新
		for(j = 0; j < td.label.size; ++j){
			bias[j] += alpha*(td.label.data[i][j]-catpp[j]);
		}
		//クロスエントロピーの総和
		if((i+1) % LOSS_CALC_RATE == 0) printf("Data: %6d, Loss: %.8f\n", i+1, xentrloss(td, weight, bias));
		
		free(catout);
		free(catpp);
	}
	
	//重みの書き出し
	FILE *wwp;
	if((wwp = fopen("weight-value","w")) != NULL){
		for(i = 0; i < td.label.size; ++i){
			for(j = 0; j < td.fval.size; ++j){
				fprintf(wwp, "%.20f\n", weight[i][j]);
			}
		}
	}else{
		fprintf(stderr, "Error: Cannot open file.");
		exit(1);
	}
	fclose(wwp);
	
	//バイアスの書き出し
	FILE *wbp;
	if((wbp = fopen("bias-value","w")) != NULL){
		for(i = 0; i < td.label.size; ++i){
			fprintf(wbp, "%.20f\n", bias[i]);
		}
	}else{
		fprintf(stderr, "Error: Cannot open file.");
		exit(1);
	}
	fclose(wbp);
	
	for(i = 0; i < td.fval.size; ++i) free(weight[i]);
	for(i = 0; i < td.label.size; ++i) free(weight);
	free(bias);
}

void test(void){
	int i,j,k;
	
	//学習データの読みこみ
	TESTDATA td;
	td.fval = read_mnistimg("./mnist/t10k-images.idx3-ubyte");
	puts("Image data has loaded.");
	td.label = read_mnistlbl("./mnist/t10k-labels.idx1-ubyte");
	puts("Label data has loaded.");

	//重みとバイアスの宣言(callocで確保します)
	double **weight, *bias;
	weight = (double**)calloc(td.label.size, sizeof(double*));
	for(j = 0; j < td.label.size; ++j){
		weight[j] = (double*)calloc(td.fval.size, sizeof(double));
	}
	bias = (double*)calloc(td.label.size, sizeof(double));
	
	//重みの読み込み
	FILE *rwp;
	if((rwp = fopen("weight-value","r")) != NULL){
		for(i = 0; i < td.label.size; ++i){
			for(j = 0; j < td.fval.size; ++j){
				fscanf(rwp, "%lf", &weight[i][j]);
			}
		}
	}else{
		fprintf(stderr, "Error: Cannot open file.");
		exit(1);
	}
	fclose(rwp);
	
	//バイアスの読み込み
	FILE *rbp;
	if((rbp = fopen("bias-value","r")) != NULL){
		for(i = 0; i < td.label.size; ++i){
			fscanf(rbp, "%lf", &bias[i]);
		}
	}else{
		fprintf(stderr, "Error: Cannot open file.");
		exit(1);
	}
	fclose(rbp);
	
	//混同行列
	double **confmat;
	confmat = (double**)calloc(td.label.size, sizeof(double*));
	for(j = 0; j < td.label.size; ++j){
		confmat[j] = (double*)calloc(td.fval.size, sizeof(double));
	}
	
	//カテゴリ出力とカテゴリ事後確率の宣言
	double *catout, *catpp;
	catout = (double*)malloc(sizeof(double)*td.label.size);
	catpp = (double*)malloc(sizeof(double)*td.label.size);
	
	//カテゴリ分類
	int m_category;
	for(i = 0; i < td.fval.num; i++){
		//ニューラルネットワークの出力を計算
		catout = nncout_test(td, weight, bias, i);
		//カテゴリ事後確率の導出
		catpp = nncpp(catout, td.label.size);
		//事後確率が最大であるカテゴリを導出
		m_category = getmax(catpp, td.label.size);
		confmat[td.label.data[i]][m_category] += 1;
		printf("[Data:%d]right:%d predict:%d(pp:%.1f)\n", i+1, td.label.data[i], m_category, catpp[m_category]*100);
		//誤った画像の表示
#ifdef OUTPUT_IMAGE
		if(td.label.data[i] != m_category){
			puts("-----Image-----");
			for(j = 0; j < td.fval.size; ++j){
				putchar(td.fval.data[i][j] < 128 ? '-' : '#');
				if((j+1) % td.fval.c_size == 0) putchar('\n');
			}
		}
#endif
		free(catout);
		free(catpp);
	}
	//混同行列の計算
	int cat_num;
	for(i = 0; i < td.label.size; ++i){
		cat_num = 0;
		for(j = 0; j < td.label.size; ++j) cat_num += confmat[i][j];
		for(j = 0; j < td.label.size; ++j) confmat[i][j] /= cat_num;
	}
	
	//混同行列の出力
	puts("-----Confusion Matrix-----");
	for(i = 0; i < td.label.size; i++){
		for(j = 0; j < td.label.size; j++){
			printf("%3.1f   ", confmat[i][j]*100);
		}
		putchar('\n');
	}
	
	for(i = 0; i < td.fval.size; ++i) free(weight[i]);
	for(i = 0; i < td.label.size; ++i) free(weight);
	free(bias);
	for(i = 0; i < td.fval.size; ++i) free(confmat[i]);
	for(i = 0; i < td.label.size; ++i) free(confmat);
}