#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "funcs.h"

float *f_merge(float *arr_1, int n1, float *arr_2, int n2) {
  // we assume arr_1 and arr_2 are already sorted
  int t1 = 0;
  int t2 = 0;
  float *out = malloc(sizeof(float) * (n1 + n2));
  int i = 0;
  while (t1 != n1 && t2 != n2) {
    out[i++] = (arr_1[t1] < arr_2[t2]) ? arr_1[t1++] : arr_2[t2++];
  }

  while (t1 != n1)
    out[i++] = arr_1[t1++];
  while (t2 != n2)
    out[i++] = arr_2[t2++];

  return out;
}
float *f_mergesort(float *arr, int n) {
  // store left side and right side
  float *ls, *rs;
  // count how big lc and rc are
  int lc = 0;
  int rc = 0;
  for (int i = 0; i < n; i++) {
    if (arr[i] > arr[n - 1])
      lc++;
    else
      rc++;
  }
  ls = malloc(sizeof(float) * lc);
  rs = malloc(sizeof(float) * rc);
  lc = 0;
  rc = 0;
  for (int i = 0; i < n; i++) {
    if (arr[i] > arr[n - 1])
      ls[lc++] = arr[i];
    else
      rs[rc++] = arr[i];
  }

  // There's probably a more efficient way to structure this if statement, but I think this one is a bit clearer
  if (lc == 0) {
    float *out = f_merge(NULL, 0, rs, rc);
    free(ls);
    free(rs);
    return out;
  } else if (rc == 0) {
    float *out = f_merge(ls, lc, NULL, 0);
    free(ls);
    free(rs);
    return out;
  }

  float *sorted_left = f_mergesort(ls, lc);
  float *sorted_right = f_mergesort(rs, rc);

  free(ls);
  free(rs);

  float *arr_out = f_merge(sorted_left, lc, sorted_right, rc);

  free(sorted_left);
  free(sorted_right);
  return arr_out;
}

int bin_mode(int *y, int n) {
  int num_0s = 0;
  int num_1s = 0;

  for (int i = 0; i < n; i++) {
    switch(y[i]){
      case 0: num_0s++; break;
      default: num_1s++; break; // We can say 1 == default since in a binary sequence we only have 1s and 0s
    }
  }
  // NOTE: if number of zeroes equals number of ones, then we return 0
  return num_1s > num_0s;
}
int *i_unique(int *vec, int n, int *len) {
  if (n == 0)
    return malloc(sizeof(int));

  // make a sorted copy of the vector, which will take O(nlogn)
  int *sorted = malloc(sizeof(int) * n);
  memcpy(sorted, vec, sizeof(int) * n);
  // TODO: implement mergesort or qsort
  // mergesort(sorted);

  // First, we can count the number of unique elements in O(n) (assuming list is sorted)
  *len = 1;
  for (int i = 1; i < n; i++) {
    if(sorted[i-1] != sorted[i])
      (*len)++;
  }

  // Then, we can isolate the number of unique elements in O(n)
  int *out = malloc(sizeof(int) * (*len));
  int clen = 1;
  out[0] = sorted[0];
  for (int i = 1; i < n; i++) {
    if(sorted[i-1] != sorted[i]) {
      out[clen] = sorted[i];
      clen++;
    }
  }

  return out;
}
float *f_unique(float *vec, int n, int *len) {
  if (n == 0)
    return malloc(sizeof(float));

  // make a sorted copy of the vector, which will take O(nlogn)
  float *sorted = f_mergesort(vec, n);

  // First, we can count the number of unique elements in O(n) (assuming list is sorted)
  *len = 1;
  for (int i = 1; i < n; i++) {
    if (sorted[i - 1] != sorted[i])
      (*len)++;
  }

  // Then, we can isolate the number of unique elements in O(n)
  float *out = malloc(sizeof(float) * (*len));
  int clen = 1;
  out[0] = sorted[0];
  for (int i = 1; i < n; i++) {
    if (sorted[i - 1] != sorted[i]) {
      out[clen] = sorted[i];
      clen++;
    }
  }

  free(sorted);

  return out;
}

float *f_getColumn(float **X, int idx, int n) {
  float *column = malloc(sizeof(float) * n);
  for (int i = 0; i < n; i++)
    column[i] = X[i][idx];

  return column;
}

float entropy(float p) {
  return -p * log2f(p);
}

float l_entropy(float *p, int n) {
  float out = 0;
  for (int i = 0; i < n; i++)
    out += entropy(p[i]);
  return out;
}

int i_count(int *vec, int target, int n) {
  int c = 0;
  for (int i = 0; i < n; i++)
    if(vec[i] == target)
      c++;
  return c;
}

void split(int **yes_pred, int **no_pred, int *size_yes, int *size_no, float *vec, int *y, int n, int threshold) {
  int y_counter = 0;
  int n_counter = 0;
  *size_yes = 0;
  *size_no = 0;
  for (int i = 0; i < n; i++) {
    if (vec[i] > threshold)
      (*size_yes)++;
    else
      (*size_no)++;
  }
  *yes_pred = malloc(sizeof(int) * (*size_yes));
  *no_pred = malloc(sizeof(int) * (*size_no));
  for (int i = 0; i < n; i++) {
    if (vec[i] > threshold){
      (*yes_pred)[y_counter] = y[i];
      y_counter++;
    } else {
      (*no_pred)[n_counter] = y[i];
      n_counter++;
    }
  }
}

// Samples "n" items from a list "in" of length "len" and returns that array (of length "n")
int *i_sample(int *in, int len, int n) {
  int *out = malloc(sizeof(int) * len);
  memcpy(out, in, sizeof(int) * len);
  while (len > n) {
    int to_del = ((double)rand() / RAND_MAX) * len;
    int *temp = malloc(sizeof(int) * (len - 1));
    memcpy(temp, out, sizeof(int) * to_del);
    memcpy(temp + to_del, out + to_del + 1, sizeof(int) * (len - to_del - 1));
    memcpy(out, temp, sizeof(int) * (len - 1));
    out = realloc(out, sizeof(int) * (len - 1));
    free(temp);
    len--;
    }
  return out;
}
