typedef struct DecisionStump {
  int yes_mode; // The result of an example reaching the yes leaf
  int no_mode; // The result of an example reaching the no leaf
  int best_feature; // Index of the feature on which it creates a split
  float best_threshold; // The threshold where to split (on the feature)
  float best_info_gain; // The infogain of splitting on the feature at the threshold
} Stump;
Stump *trainDecisionStump(float **X, int *y, int n, int d);
int *predictDecisionStump(Stump *stump, float **X, int n, int d);
void printDecisionStump(Stump *stump);
