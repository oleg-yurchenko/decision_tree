#include <stdio.h>
#include <stdlib.h>
#include "decision_tree.h"

#define DATA_PATH "./data/danceability_data.csv"
#define N_COLS 19
#define N_ROWS 829

int main() {
  FILE *data_file = fopen(DATA_PATH, "r");
  if (!data_file)
    return -1;

  float **X = (float **) malloc(sizeof(float *) * N_ROWS);
  for (int i = 0; i < N_ROWS; i++)
    X[i] = (float *) malloc(sizeof(float) * N_COLS);

  int *y = (int *) malloc(sizeof(int) * N_ROWS);

  for (int n = 0; n < N_ROWS; n++) {
    for (int d = 0; d < N_COLS; d++)
      fscanf(data_file, "%f,", &(X[n][d]));
    fscanf(data_file, "%d\n", &(y[n]));
  }
  fclose(data_file);

  printf("starting training...\n");
  // This tree will severely overfit the data, especially since we don't have an actual validation set, and it has infinite depth
  Tree *tree = trainDecisionTree(X, y, N_ROWS, N_COLS, -1, -1);
  printf("finished training...\n");

  // DO SOMETHING WITH TREE
  printDecisionTree(tree);

  int *y_pred = predictDecisionTree(tree, X, N_ROWS);

  int n_correct = 0;
  for (int i = 0; i < N_ROWS; i++)
    if (y_pred[i] == y[i])
      n_correct++;
  float accuracy = ((float)n_correct) / ((float)N_ROWS);

  printf("ACCURACY: %f\n", accuracy);

  freeDecisionTree(tree);

  for (int i = 0; i < N_ROWS; i++)
    free(X[i]);
  free(X);
  free(y);

  return 0;
}
