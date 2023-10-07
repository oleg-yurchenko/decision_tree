#include <math.h>

int bin_mode(int *y, int n);
float *f_unique(float *vec, int n, int *len);
float *f_getColumn(float **X, int idx, int n);
float entropy(float p);
float l_entropy(float *p, int n);
int i_count(int *vec, int target, int n);
void split(int **yes_pred, int **no_pred, int *size_yes, int *size_no, float *vec, int *y, int n, int threshold);
int *i_sample(int *in, int len, int n);
// TODO: All below here
int *i_unique(int *vec, int n, int *len);
int *i_getColumn(int **X, int idx, int n);
int f_count(float *vec, float target, int n);
