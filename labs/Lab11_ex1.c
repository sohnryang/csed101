#include <stdio.h>
#include <string.h>

int main() {
  printf("문자열 입력: ");
  char str[100];
  scanf("%s", str);
  printf("문자열(%s)의 길이: %ld\n", str, strlen(str));
  return 0;
}
