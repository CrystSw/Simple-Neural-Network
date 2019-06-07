#ifndef __SNN_MNIST_H_INCLUDED__
#define __SNN_MNIST_H_INCLUDED__

#include "nnet_types.h"

extern FVALUE read_mnistimg(char *filename);
extern LABELOHV read_mnistlbl_ohv(char *filename);
extern LABEL read_mnistlbl(char *filename);

#endif