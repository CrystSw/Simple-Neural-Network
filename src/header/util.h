#ifndef __SNN_UTIL_H_INCLUDED__
#define __SNN_UTIL_H_INCLUDED__

extern double *malloc_d1(const int size1);
extern double *calloc_d1(const int size1);
extern double **malloc_d2(const int size1, const int size2);
extern double **calloc_d2(const int size1, const int size2);
extern void free_d1(double *array);
extern void free_d2(double **array, const int size1);
extern int getmax(const double *array, int size1);
extern void writefile_d1(const char *filename, double *array_d1, const int size1);
extern void readfile_d1(const char *filename, double *array_d1, const int size1);
extern void writefile_d2(const char *filename, double **array_d2, const int size1, const int size2);
extern void readfile_d2(const char *filename, double **array_d2, const int size1, const int size2);

#endif