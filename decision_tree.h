#include "decision_stump.h"
typedef struct DecisionTree {
  Stump *stump;
  struct DecisionTree *greaterChild;
  struct DecisionTree *lesserChild;
  int remaining_depth;
} Tree;

typedef struct RandomTree {
  Tree *tree;
  float **X_sample;
  int *y_sample;
} RTree;

Tree *trainDecisionTree(float **X, int *y, int n, int d, int max_depth, float min_infogain, Stump *(*trainDecisionStump)(float **X, int *y, int n, int d), int *(*predictDecisionStump)(Stump *stump, float **X, int n, int d));

Tree *trainDefaultTree(float **X, int *y, int n, int d, int max_depth, float min_infogain);
// By default, we want the random tree to overfit, so max_depth should be very large and min_infogain = max. These parameters are just here if someone wants to modify it.
RTree *trainRandomTree(float **X, int *y, int n, int d, int max_depth, float min_infogain);
int *predictDecisionTree(Tree *tree, float **X, int n);
int findPrediction(Tree *tree, float *X);
// The tree passed into this function will free all created stumps, and greater/lesser children if they exist, as well as itself.
void freeDecisionTree(Tree *tree);
void freeRandomTree(RTree *tree);
void printDecisionTree(Tree *tree);
