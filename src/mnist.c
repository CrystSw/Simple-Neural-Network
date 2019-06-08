#include <stdio.h>
#include <stdlib.h>

#include "header/nnet_types.h"

/**
 * mnistの画像データをロードする．
 *
 * @param filename - ファイル名
 * @return - 特徴量データ
 */
FVALUE read_mnistimg(char *filename) {
	int i, j;
	
	FVALUE fval;
	FILE *mnistimg;
	//画像データのロード
	if((mnistimg = fopen(filename, "rb")) == NULL){
		fprintf(stderr, "Error: Cannot open file.\n");
		exit(1);
	}
	//ヘッダ部の読み込み
	u_char header[16];
	if(fread(header, sizeof(u_char), 16, mnistimg) != 16){
		fprintf(stderr, "Error: Invalid file format.\n");
		exit(1);
	}
	fval.num = (header[4] << 24) + (header[5] << 16) + (header[6] << 8) + header[7];
	fval.r_size = ((header[8] << 24) + (header[9] << 16) + (header[10] << 8) + header[11]);
	fval.c_size = ((header[12] << 24) + (header[13] << 16) + (header[14] << 8) + header[15]);
	fval.size = fval.r_size*fval.c_size;
	//画像データの読み込み
	fval.data = (u_char**)malloc(sizeof(u_char*)*fval.num);
	for(i = 0; i < fval.num; ++i){
		fval.data[i] = (u_char*)malloc(sizeof(u_char)*fval.size);
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
 * @param filename - ファイル名
 * @return - ラベルデータ(one-hot-vector形式)
 */
LABELOHV read_mnistlbl_ohv(char *filename) {
	int i, j;
	
	LABELOHV label;
	FILE *mnistlbl;
	//ラベルデータのロード
	if((mnistlbl = fopen(filename, "rb")) == NULL){
		fprintf(stderr, "Error: Cannot open file.\n");
		exit(1);
	}
	//ヘッダ部の読み込み
	u_char header[8];
	if(fread(header, sizeof(u_char), 8, mnistlbl) != 8){
		fprintf(stderr, "Error: Invalid file format.\n");
		exit(1);
	}
	label.num = (header[4] << 24) + (header[5] << 16) + (header[6] << 8) + header[7];
	label.size = 10;	//カテゴリ数
	//ラベルデータの読み込み
	label.data = (double**)malloc(sizeof(double*)*label.num);
	//one-hot-vector形式へ変換
	u_char category;
	for(i = 0; i < label.num; ++i){
		label.data[i] = (double*)malloc(sizeof(double)*label.size);
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
 * @param filename - ファイル名
 * @return - ラベルデータ
 */
LABEL read_mnistlbl(char *filename) {
	int i;
	
	LABEL label;
	FILE *mnistlbl;
	//ラベルデータのロード
	if((mnistlbl = fopen(filename, "rb")) == NULL){
		fprintf(stderr, "Error: Cannot open file.\n");
		exit(1);
	}
	//ヘッダ部の読み込み
	u_char header[8];
	if(fread(header, sizeof(u_char), 8, mnistlbl) != 8){
		fprintf(stderr, "Error: Invalid file format.\n");
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