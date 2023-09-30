#include <limits.h>
#include "decision_stump.h"
typedef struct DecisionTree {
  Stump *stump;
  DecisionTree *greaterChild;
  DecisionTree *lesserChild;
  int remaining_depth;
} Tree;

Tree *trainDecisionTree(float **X, int *y, int n, int d, int max_depth = INT_MAX, float min_infogain = 0.0f);
int *predictDecisionTree(Tree *tree, float **X, int n, int d);
// The tree passed into this function will free all created stumps, and greater/lesser children if they exist, as well as itself.
void *freeDecisionTree(Tree *tree);
