#include "decision_stump.h"
typedef struct DecisionTree {
  Stump *stump;
  struct DecisionTree *greaterChild;
  struct DecisionTree *lesserChild;
  int remaining_depth;
} Tree;

Tree *trainDecisionTree(float **X, int *y, int n, int d, int max_depth, float min_infogain);
int *predictDecisionTree(Tree *tree, float **X, int n);
int findPrediction(Tree *tree, float *X);
// The tree passed into this function will free all created stumps, and greater/lesser children if they exist, as well as itself.
void freeDecisionTree(Tree *tree);
void printDecisionTree(Tree *tree);
