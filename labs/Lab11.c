#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char name[26];
  int exam[2];
  float avg;
  char grade;
} STUDENT;

void add_student(STUDENT *student);
void print_header();
void print_one_student(STUDENT student, int n);
void print_students(STUDENT slist[], int n);

int main() {
  STUDENT slist[10];
  int slist_len = 0;
  while (true) {
    printf(">> ");
    char cmd[10];
    scanf("%s", cmd);

    if (!strcmp(cmd, "add")) {
      add_student(slist + slist_len++);
    } else if (!strcmp(cmd, "print")) {
      print_students(slist, slist_len);
    } else if (!strcmp(cmd, "find")) {
      printf("이름: ");
      char name[26];
      scanf("%s", name);
      int idx = -1;
      for (int i = 0; i < slist_len; i++)
        if (!strcmp(name, slist[i].name)) {
          idx = i;
          break;
        }
      if (idx == -1) {
        printf("Not found\n\n");
        continue;
      }
      print_header();
      print_one_student(slist[idx], idx + 1);
    } else if (!strcmp(cmd, "quit"))
      break;
    else
      printf("Unknown command\n");
    printf("\n");
  }
  return 0;
}

void add_student(STUDENT *student) {
  printf("이름: ");
  scanf("%s", student->name);
  printf("중간고사: ");
  scanf("%d", &student->exam[0]);
  printf("기말고사: ");
  scanf("%d", &student->exam[1]);
  student->avg = 0.5 * (student->exam[0] + student->exam[1]);
  if (student->avg >= 90)
    student->grade = 'A';
  else if (student->avg >= 80)
    student->grade = 'B';
  else
    student->grade = 'C';
}

void print_header() {
  printf("번호  이름                 중간  기말  평균 (학점)\n");
}

void print_one_student(STUDENT student, int n) {
  printf("%2d    %-20s %3d   %3d    %.1f (%c)\n", n, student.name,
         student.exam[0], student.exam[1], student.avg, student.grade);
}

void print_students(STUDENT slist[], int n) {
  print_header();
  for (int i = 0; i < n; i++)
    print_one_student(slist[i], i + 1);
}
