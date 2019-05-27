#ifndef __SIMPLE_NNET_H__
#define __SIMPLE_NNET_H__

#include "snnet-types.h"

extern FVALUE read_mnistimg(char *filename);
extern LABEL read_mnistlbl(char *filename);
extern LABELOHV read_mnistlbl_ohv(char *filename);
extern double softmax(const double *output, const int id, const int size);
extern double cross_entropy(const double *rpp, const double *wpp, const int size);
extern void nncout_train(const TRAINDATA td, double **weight, const double *bias, const int id, double *catout);
extern void nncout_test(const TESTDATA td, double **weight, const double *bias, const int id, double *catout);
extern void nncpp(const double *catout, const int size, double *catpp);
extern double xentrloss(const TRAINDATA td, double **weight, const double *bias);
extern int getmax(const double *p, int size);

#endif