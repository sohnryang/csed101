#include <math.h>
#include <stdio.h>

void triangle(int a, int b, float *perimeter, float *area);
void print(float perimeter, float area);

int main() {
  int a, b;
  printf("Enter two integer sides: ");
  scanf("%d %d", &a, &b);
  float perimeter, area;
  triangle(a, b, &perimeter, &area);
  print(perimeter, area);
  return 0;
}

void triangle(int a, int b, float *perimeter, float *area) {
  double c = sqrt(a * a + b * b);
  *perimeter = a + b + c;
  *area = (double)a * b / 2;
}

void print(float perimeter, float area) {
  printf("Perimeter: %.6lf\n", perimeter);
  printf("Area     : %.6lf\n", area);
}
