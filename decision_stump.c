#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/_types/_null.h>
#include "funcs.h"
#include "decision_stump.h"

/*
typedef struct DecisionStump {
  int yes_mode; // The result of an example reaching the yes leaf
  int no_mode; // The result of an example reaching the no leaf
  int best_feature; // Index of the feature on which it creates a split
  float best_threshold; // The threshold where to split (on the feature)
  float best_info_gain; // The infogain of splitting on the feature at the threshold
} Stump;
*/

/* X: a 2-D array with n rows and d columns, such that the columns are features and the rows are examples (floating point numbers)
 * y: a 1-D array that is the transpose of a n row, 1 column vector, the training output (these are either 0 or 1)
 * n: the number of examples (rows in X and y^T)
 * d: the number of features (columns in X)
 * 
 * Returns a pointer to a decision stump with the parameters defined in the struct above (yes_mode and no_mode are the output for when the predictors are 1 and 0, respectively)
 * This function allocates the space for the stump, and expects the receiving process to free it
 */
Stump *trainDecisionStump(float **X, int *y, int n, int d, int *feat_indices) {
  Stump *stump = malloc(sizeof(Stump));

  // initialize the values of stump
  stump->yes_mode = bin_mode(y, n);
  stump->no_mode = bin_mode(y, n);
  stump->best_feature = -1;
  stump->best_threshold = -1;
  stump->best_info_gain = 0;

  // Go over every single feature
  for (int i = 0; i < d; i++) {
    // Go over every threshold in the feature
    float *attribute_vec = f_getColumn(X, feat_indices[i], n); 
    int num_uniques;
    float *unique_ts = f_unique(attribute_vec, n, &num_uniques);
    for (int j = 0; j < num_uniques; j++) {
      float t = unique_ts[j];
      int *yes_arr, *no_arr;
      int yes_size, no_size;
      split(&yes_arr, &no_arr, &yes_size, &no_size, attribute_vec, y, n, t);
      int curr_yes_mode = bin_mode(yes_arr, yes_size);
      int curr_no_mode = bin_mode(no_arr, no_size);

      // The following are probabilities of encountering a yes or no in the sets that were given
      float tot_p_arr[2], yes_p_arr[2], no_p_arr[2];
      tot_p_arr[0] = ((float) yes_size) / n;
      tot_p_arr[1] = ((float) no_size) / n;
      yes_p_arr[0] = ((float) i_count(yes_arr, 0, yes_size)) / yes_size;
      yes_p_arr[1] = ((float) i_count(yes_arr, 1, yes_size)) / yes_size;
      no_p_arr[0] = ((float) i_count(no_arr, 0, no_size)) / no_size;
      no_p_arr[1] = ((float) i_count(no_arr, 1, no_size)) / no_size;
      float curr_info_gain = l_entropy(tot_p_arr, 2) - (l_entropy(yes_p_arr, 2) * yes_size / n) - (l_entropy(no_p_arr, 2) * no_size / n);

      if (curr_info_gain > stump->best_info_gain) {
        stump->yes_mode = curr_yes_mode;
        stump->no_mode = curr_no_mode;
        stump->best_feature = feat_indices[i];
        stump->best_threshold = t;
        stump->best_info_gain = curr_info_gain;
      }
      free(yes_arr); free(no_arr);
    }
    free(attribute_vec);
    free(unique_ts);
  }

  return stump;
}

Stump *trainDefaultStump(float **X, int *y, int n, int d) {
  int *feat_indices = malloc(sizeof(int) * d);
  for (int i = 0; i < d; i++)
    feat_indices[i] = i;
  Stump *stump = trainDecisionStump(X, y, n, d, feat_indices);
  free(feat_indices);
  return stump;
}

Stump *trainRandomStump(float **X, int *y, int n, int d) {
  int avail[d];
  for (int i = 0; i < d; i++)
    avail[i] = i;
  int sqrt_d = sqrt(d);
  int *feat_indices = i_sample((int *)avail, d, sqrt_d);
  Stump *stump = trainDecisionStump(X, y, n, sqrt_d, feat_indices);
  free(feat_indices);
  return stump;
}

// NOTE: the out array does NOT predict the mode anymore, it only predicts 1 if it should be greater, and 0 if lesser
// The DECISION TREE is responsible for using the stump's yes_mode and no_mode parameters to classify it once it reaches its end
int *predictDecisionStump(Stump *stump, float **X, int n, int d) {
  int *out = malloc(sizeof(int) * n);
  float *X_t = f_getColumn(X, stump->best_feature, n);
  for (int i = 0; i < n; i++)
    out[i] = (X_t[i] > stump->best_threshold) ? 1 : 0;
  free(X_t);
  return out;
}

void printDecisionStump(Stump *stump) {
  printf("---STUMP INFO---\nyes_mode: %d\nno_mode: %d\nbest_feature: %d\nbest_threshold: %f\nbest_info_gain: %f\n----------------\n", stump->yes_mode    , stump->no_mode, stump->best_feature, stump->best_threshold, stump->best_info_gain);
}

