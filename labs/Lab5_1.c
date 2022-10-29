#include <stdio.h>

float calc_average(int a, int b);
char calc_grade(float score);

int main() {
  printf("Enter your midterm score: ");
  int midterm;
  scanf("%d", &midterm);
  printf("Enter your final score: ");
  int final;
  scanf("%d", &final);
  float average = calc_average(midterm, final);
  printf("Average: %.1f\n", average);
  char grade = calc_grade(average);
  printf("Grade: %c\n", grade);
  return 0;
}

float calc_average(int a, int b) { return 0.5 * (a + b); }
char calc_grade(float score) {
  if (score >= 90)
    return 'A';
  else if (score >= 80)
    return 'B';
  else if (score >= 70)
    return 'C';
  else if (score >= 60)
    return 'D';
  return 'F';
}
