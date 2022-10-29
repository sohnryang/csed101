#include <stdio.h>

int main() {
  printf("Enter a number: ");
  int num;
  scanf("%d", &num);
  while (num) {
    printf("%d", num % 10);
    num /= 10;
  }
  printf("\n");
  return 0;
}
