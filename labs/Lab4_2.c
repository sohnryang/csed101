#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void randData(int *, int *, int *);
float arithmeticMean(int, int);
float geometricMean(int, int);
float harmonicMean(int, int);

int main() {
  srand(time(NULL));
  int a1, a2, a3;
  randData(&a1, &a2, &a3);
  int b1, b2, b3;
  randData(&b1, &b2, &b3);

  printf("arithmeticMean(%2d, %2d) => %.2f\n", a1, b1, arithmeticMean(a1, b1));
  printf(" geometricMean(%2d, %2d) => %.2f\n", a2, b2, geometricMean(a2, b2));
  printf("  harmonicMean(%2d, %2d) => %.2f\n", a3, b3, geometricMean(a3, b3));
  return 0;
}

void randData(int *x, int *y, int *z) {
  *x = rand() % 66 + 5;
  *y = rand() % 66 + 5;
  *z = rand() % 66 + 5;
}

float arithmeticMean(int a, int b) { return (float)(a + b) / 2; }

float geometricMean(int a, int b) { return (float)sqrt(a * b); }

float harmonicMean(int a, int b) { return (float)(2 * a * b) / (a + b); }
