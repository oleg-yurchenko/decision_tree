#include "random_forest.h"
#include "funcs.h"
#include <stdlib.h>
#include <stdio.h>

Forest *trainRandomForest(float **X, int *y, int n, int d, int count) {
  Forest *out = malloc(sizeof(Forest));
  out->count = count;
  out->trees = malloc(sizeof(RTree) * count);

  for (int i = 0; i < count; i++)
    out->trees[i] = trainRandomTree(X, y, n, d, -1, -1);

  return out;
}

int *predictRandomForest(Forest *forest, float **X, int n) {
  int **y = malloc(sizeof(int *) * forest->count);
  for (int i = 0; i < forest->count; i++)
    y[i] = predictDecisionTree(forest->trees[i]->tree, X, n);
  int **y_t = i_transpose(y, forest->count, n);
  int *y_pred = malloc(sizeof(int) * n);
  for (int i = 0; i < n; i++) {
    y_pred[i] = bin_mode(y_t[i], forest->count);
  }

  for (int i = 0; i < forest->count; i++)
      free(y[i]);
  free(y);

  for (int i = 0; i < n; i++)
    free(y_t[i]);
  free(y_t);

  return y_pred;
}

void freeRandomForest(Forest *forest) {
  for (int i = 0; i < forest->count; i++)
    freeRandomTree(forest->trees[i]);
  free(forest->trees);
  free(forest);
}

void printRandomForest(Forest *forest) {
  printf("&&&& PRINTING RANDOM FOREST &&&&\n");
  printf("tree count: %d\n", forest->count);
  for (int i = 0; i < forest->count; i++) {
    printf("Printing Tree %d...\n", i);
    printDecisionTree(forest->trees[i]->tree);
  }
  printf("&&&& DONE PRINTING RANDOM FOREST &&&&\n");
}
