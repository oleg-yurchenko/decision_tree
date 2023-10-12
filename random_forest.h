#include "decision_tree.h"
typedef struct RandomForest {
  RTree **trees;
  int count;
} Forest;

Forest *trainRandomForest(float **X, int *y, int n, int d, int count);
int *predictRandomForest(Forest *forest, float **X, int n);
void freeRandomForest(Forest *forest);
void printRandomForest(Forest *forest);

