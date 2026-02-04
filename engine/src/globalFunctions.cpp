#include "globalFunctions.h"

float HeavyComputations(int numIterations) {
	 float acc = 0.0f;
	 for (int i = 0; i < numIterations; i++) {
		 acc += sin(i) * cos(i);
	 }
	 return acc;
}