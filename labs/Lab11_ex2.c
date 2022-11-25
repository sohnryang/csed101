#include <stdio.h>

int main() {
  printf("문자열 입력: ");
  char str[100];
  scanf("%s", str);
  int res = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    char ch = str[i];
    if ('0' <= ch && ch <= '9')
      res += ch - '0';
  }
  printf("숫자의 합: %d\n", res);
  return 0;
}
