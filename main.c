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
  RTree *tree = trainRandomTree(X, y, N_ROWS, N_COLS, -1, -1);
  printf("finished training...\n");

  // DO SOMETHING WITH TREE
  printDecisionTree(tree->tree);

  int *y_pred = predictDecisionTree(tree->tree, X, N_ROWS);

  int n_correct = 0;
  for (int i = 0; i < N_ROWS; i++)
    if (y_pred[i] == y[i])
      n_correct++;
  float accuracy = ((float)n_correct) / ((float)N_ROWS);

  printf("TRAINING ACCURACY: %f\n", accuracy);

  // Testing on Blinding Lights. Result should be false (danceability = 50 < 75) (has NA value for feature 10, so I set NA=0 for this specifically)
  // In the original dataset (X) all the rows with NA values present were cut, and only the columns with numeric values were kept (no artist's name, song name, etc.)
  float **X_test = malloc(sizeof(float *) * 1);
  X_test[0] = malloc(sizeof(float) * N_COLS);
  X_test[0][0] = 1.0f;
  X_test[0][1] = 2019.0f;
  X_test[0][2] = 11.0f;
  X_test[0][3] = 29.0f;
  X_test[0][4] = 43899.0f;
  X_test[0][5] = 69.0f;
  X_test[0][6] = 3703895074.0f;
  X_test[0][7] = 672.0f;
  X_test[0][8] = 199.0f;
  X_test[0][9] = 3421.0f;
  X_test[0][10] = 20.0f;
  X_test[0][11] = 0.0f;
  X_test[0][12] = 171.0f;
  X_test[0][13] = 38.0f;
  X_test[0][14] = 80.0f;
  X_test[0][15] = 0.0f;
  X_test[0][16] = 0.0f;
  X_test[0][17] = 9.0f;
  X_test[0][18] = 7.0f;

  int *y_test_pred = predictDecisionTree(tree->tree, X_test, 1);
  printf("prediction of Blinding Lights: %d\nCorrect answer is: 0\n", y_pred[0]);
  free(X_test[0]);
  free(X_test);
  free(y_test_pred);
  free(y_pred);

  freeRandomTree(tree);

  for (int i = 0; i < N_ROWS; i++)
    free(X[i]);
  free(X);
  free(y);

  return 0;
}
