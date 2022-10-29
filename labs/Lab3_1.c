#include <stdio.h>

int add(int x, int y);
float average(int x, int y);
void printResults(int x, int y, int sum, float avg);

int main() {
  printf("Enter two integers: ");
  int x, y;
  scanf("%d %d", &x, &y);
  int sum = add(x, y);
  float avg = average(x, y);
  printResults(x, y, sum, avg);
  return 0;
}

int add(int x, int y) { return x + y; }

float average(int x, int y) {
  float sum = add(x, y);
  return sum / 2;
}

void printResults(int x, int y, int sum, float avg) {
  printf("The sum of %d and %d is %d\n", x, y, sum);
  printf("The average of numbers is %.1f\n", avg);
}
