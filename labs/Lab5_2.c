#include <stdio.h>

int main() {
  for (int i = 1; i <= 9; i++) {
    for (int j = 2; j <= 9; j++)
      printf("%d * %d = %2d  ", j, i, j * i);
    printf("\n");
  }
  return 0;
}
