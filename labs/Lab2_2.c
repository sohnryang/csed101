#include <stdio.h>

int main() {
  double r;
  const double Pi = 3.141592;
  printf("반지름을 입력하시오: ");
  scanf("%lf", &r);
  printf("원 둘레: %.6lf\n", 2 * Pi * r);
  printf("원 넓이: %.6lf\n", Pi * r * r);
  return 0;
}
