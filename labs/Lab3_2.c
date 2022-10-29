#include <stdio.h>

char upperToLower(char c);
int getPosition(char c);

int main() {
  printf("Input a character in Uppercase: ");
  char c;
  scanf("%c", &c);
  char lowered = upperToLower(c);
  printf("Lowercase of Entered character is %c\n", lowered);
  printf("Poistion of '%c' in English Alphabets is %d\n", lowered,
         getPosition(lowered));
  return 0;
}

char upperToLower(char c) { return c - 'A' + 'a'; }
int getPosition(char c) { return c - 'a' + 1; }
