#include <stdio.h>
#include <stdlib.h>
#include "decision_stump.h"

#define DATA_PATH "./data/sleep_data.csv"
#define N_COLS 5
#define N_ROWS 11

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

  /*
  printf("X:\n");
  for(int n = 0; n < N_ROWS; n++) {
    for (int d = 0; d < N_COLS - 1; d++)
      printf("%f,", X[n][d]);
    printf("\n");
  }
  printf("y:\n");
  for(int n = 0; n < N_ROWS; n++) {
    printf("%d\n", y[n]);
  }
  */

  // DO SOMETHING WITH DATA
  Stump *stump = trainDecisionStump(X, y, N_ROWS, N_COLS);
  printf("---STUMP INFO---\nyes_mode: %d\nno_mode: %d\nbest_feature: %d\nbest_threshold: %f\nbest_info_gain: %f\n----------------\n", stump->yes_mode, stump->no_mode, stump->best_feature, stump->best_threshold, stump->best_info_gain);

  float **X_test = malloc(sizeof(float *));
  X_test[0] = malloc(sizeof(float) * N_COLS);
  X_test[0][0] = 9.0f;
  X_test[0][1] = 10.0f;
  X_test[0][2] = 10.0f;
  X_test[0][3] = 2.0f;
  X_test[0][4] = 12.0f;

  int *y_pred = predictDecisionStump(stump, X_test, 1, N_COLS);

  printf("prediction: %d\n", y_pred[0]);

  for (int i = 0; i < N_ROWS; i++)
    free(X[i]);
  free(X);
  free(y);

  free(stump);

  free(y_pred);
  free(X_test[0]);
  free(X_test);

  return 0;
}
