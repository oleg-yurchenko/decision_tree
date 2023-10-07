#include <assert.h>
#include <float.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "decision_tree.h"

/*
typedef struct DecisionTree {
  Stump *stump;
  struct DecisionTree *greaterChild;
  struct DecisionTree *lesserChild;
  int remaining_depth;
} Tree;
*/

// This is the standard call, so it uses the parameter defined implementations for "trainDecisionStump" and "predictDecisionStump" functions.
Tree *trainDecisionTree(float **X, int *y, int n, int d, int max_depth, float min_infogain, Stump *(*trainDecisionStump)(float **X, int *y, int n, int d), int *(*predictDecisionStump)(Stump *stump, float **X, int n, int d)) {
  if (max_depth == 0)
    return NULL;
  if (max_depth < 0)
    max_depth = INT_MAX;
  if (min_infogain < 0)
    min_infogain = FLT_MAX;

  // Create our Tree, and initialize the values
  Tree *out = malloc(sizeof(Tree));
  out->greaterChild = NULL;
  out->lesserChild = NULL;
  out->remaining_depth = max_depth - 1;

  // Train the stump on the given data
  out->stump = trainDecisionStump(X, y, n, d);
  // If we can't find where to split at, or the info gain is too high, this tree is a dud, so return NULL
  if (out->stump->best_feature == -1 || out->stump->best_info_gain > min_infogain) {
    freeDecisionTree(out);
    return NULL;
  }
  // Everything is ok, we can split into children
  // We need to split the training set and the outputs so that we can pass them based on the predictions of the stump
  // Since the stump is trained as above, we can just pass every row of X into the predict function, and pass that row into greater or lesser child array
  int *y_pred = predictDecisionStump(out->stump, X, n, d);
  // find the size of 0s and 1s in y_pred for allocation
  int n_greater = 0, n_lesser = 0;
  for (int i = 0; i < n; i++)
    y_pred[i] == 0 ? n_lesser++ : n_greater++;

  float **X_greater = malloc(sizeof(float *) * n_greater);
  int *y_greater = malloc(sizeof(int) * n_greater);
  float **X_lesser = malloc(sizeof(float *) * n_lesser);
  int *y_lesser = malloc(sizeof(int) * n_lesser);
  int c_greater = 0, c_lesser = 0;
  for (int i = 0; i < n; i++) {
    if (y_pred[i] == 0) {
      X_lesser[c_lesser] = X[i];
      y_lesser[c_lesser] = y[i];
      c_lesser++;
    } else {
      X_greater[c_greater] = X[i];
      y_greater[c_greater] = y[i];
      c_greater++;
    }
  }
  free(y_pred);

  out->greaterChild = trainDecisionTree(X_greater, y_greater, n_greater, d, out->remaining_depth, min_infogain, trainDecisionStump, predictDecisionStump);
  out->lesserChild = trainDecisionTree(X_lesser, y_lesser, n_lesser, d, out->remaining_depth, min_infogain, trainDecisionStump, predictDecisionStump);

  free(X_greater);
  free(y_greater);
  free(X_lesser);
  free(y_lesser);

  return out;
}

Tree *trainDefaultTree(float **X, int *y, int n, int d, int max_depth, float min_infogain) {
  return trainDecisionTree(X, y, n, d, max_depth, min_infogain, trainDefaultStump, predictDecisionStump);
}

RTree *trainRandomTree(float **X, int *y, int n, int d, int max_depth, float min_infogain) {
  RTree *r_tree = malloc(sizeof(RTree));
  r_tree->X_sample = malloc(sizeof(float *) * n);
  //for (int i = 0; i < n; i++)
  //  r_tree->X_sample[i] = malloc(sizeof(float) * d);
  r_tree->y_sample = malloc(sizeof(int) * n);
  // Sample random indices
  //for (int i = 0; i < n; i++)
  for (int i = 0; i < n; i++) {
    int sample_idx = (n - 1) * ((double)rand() / RAND_MAX);
    r_tree->X_sample[i] = X[sample_idx];
    r_tree->y_sample[i] = y[sample_idx];
  }

  r_tree->tree = trainDecisionTree(r_tree->X_sample, r_tree->y_sample, n, d, max_depth, min_infogain, trainRandomStump, predictDecisionStump);
  return r_tree;
}

int *predictDecisionTree(Tree *tree, float **X, int n) {
  int *out = malloc(sizeof(int) * n);
  for (int i = 0; i < n; i++) {
    out[i] = findPrediction(tree, X[i]);
    assert(out[i] != -1);
  }
  return out;
}

int findPrediction(Tree *tree, float *X) {
  if (tree == NULL)
    return -1;
  if (X[tree->stump->best_feature] > tree->stump->best_threshold) {
    if(tree->greaterChild == NULL)
      return tree->stump->yes_mode;
    else
      return findPrediction(tree->greaterChild, X);
  } else {
    if(tree->lesserChild == NULL)
      return tree->stump->no_mode;
    else
      return findPrediction(tree->lesserChild, X);
  }
}

void freeDecisionTree(Tree *tree) {
  if (tree == NULL)
    return;
  free(tree->stump);
  freeDecisionTree(tree->greaterChild);
  freeDecisionTree(tree->lesserChild);
  free(tree);
}

void freeRandomTree(RTree *tree) {
  if (tree == NULL)
    return;
  freeDecisionTree(tree->tree);
  // the reason we don't free each pointer in X_sample is because they are references to existing data, that should be freed by whoever allocated it initially
  free(tree->X_sample);
  free(tree->y_sample);
  free(tree);
}

void printDecisionTree(Tree *tree) {
  if (tree == NULL) {
    printf("END TREE\n");
    return;
  }
  printf("---Tree begin---\n");
  printf("Tree level: %d\nStump info:\n", INT_MAX - tree->remaining_depth);
  printDecisionStump(tree->stump);
  printf("CHILDREN:\n");
  printDecisionTree(tree->greaterChild);
  printDecisionTree(tree->lesserChild);
  printf("---Tree over----\n");
}
