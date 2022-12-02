#include <stdio.h>

#include "calc.h"

int main() {
  printf("Enter two integers: ");
  int a, b;
  scanf("%d %d", &a, &b);
  printf("Results:\n");
  printf("%2d + %2d = %3d\n", a, b, add(a, b));
  printf("%2d - %2d = %3d\n", a, b, sub(a, b));
  printf("%2d * %2d = %3d\n", a, b, mul(a, b));
  printf("%2d / %2d = %6.2f\n", a, b, div(a, b));
  return 0;
}
