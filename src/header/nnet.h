#ifndef __SNN_NNET_H_INCLUDED__
#define __SNN_NNET_H_INCLUDED__

#include "nnet_types.h"

extern double softmax(const double *output, const int id, const int size);
extern double cross_entropy(const double *rpp, const double *wpp, const int size);
extern void nncout_train(const TRAINDATA td, double **weight, const double *bias, const int id, double *catout);
extern void nncout_test(const TESTDATA td, double **weight, const double *bias, const int id, double *catout);
extern void nncpp(const double *catout, const int size, double *catpp);
extern double xentrloss(const TRAINDATA td, double **weight, const double *bias);

#endif