#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char name[26];
  int exam[2];
  float avg;
  char grade;
} STUDENT;

typedef struct Node {
  STUDENT data;
  struct Node *next;
} Node;

typedef struct {
  int count;
  Node *head;
} List;

void add_student(List *list);
void print_header();
void print_one_student(STUDENT student, int n);
void print_students(List list);

int main() {
  List list = {0, NULL};
  while (true) {
    printf(">> ");
    char cmd[10];
    scanf("%s", cmd);

    if (!strcmp(cmd, "add")) {
      add_student(&list);
    } else if (!strcmp(cmd, "print")) {
      print_students(list);
    } else if (!strcmp(cmd, "find")) {
      printf("이름: ");
      char name[26];
      scanf("%s", name);
      STUDENT *student = NULL;
      Node *current = list.head;
      int i;
      for (i = 0; i < list.count; i++) {
        if (!strcmp(name, current->data.name)) {
          student = &current->data;
          break;
        }
        current = current->next;
      }
      if (student == NULL) {
        printf("Not found\n\n");
        continue;
      }
      print_header();
      print_one_student(*student, i + 1);
    } else if (!strcmp(cmd, "quit"))
      break;
    else
      printf("Unknown command\n");
    printf("\n");
  }
  Node *current = list.head;
  for (int i = 0; i < list.count; i++) {
    Node *next = current->next;
    free(current);
    current = next;
  }
  return 0;
}

void add_student(List *list) {
  Node *new_node = malloc(sizeof(Node));
  printf("이름: ");
  scanf("%s", new_node->data.name);
  printf("중간고사: ");
  scanf("%d", &new_node->data.exam[0]);
  printf("기말고사: ");
  scanf("%d", &new_node->data.exam[1]);
  new_node->data.avg = 0.5 * (new_node->data.exam[0] + new_node->data.exam[1]);
  if (new_node->data.avg >= 90)
    new_node->data.grade = 'A';
  else if (new_node->data.avg >= 80)
    new_node->data.grade = 'B';
  else
    new_node->data.grade = 'C';
  if (list->count == 0) {
    list->head = new_node;
    list->count++;
    return;
  }
  Node *last_node = list->head;
  for (int i = 1; i < list->count; i++)
    last_node = last_node->next;
  last_node->next = new_node;
  list->count++;
}

void print_header() {
  printf("번호  이름                 중간  기말  평균 (학점)\n");
}

void print_one_student(STUDENT student, int n) {
  printf("%2d    %-20s %3d   %3d    %.1f (%c)\n", n, student.name,
         student.exam[0], student.exam[1], student.avg, student.grade);
}

void print_students(List list) {
  print_header();
  Node *current = list.head;
  int i = 1;
  for (int i = 0; i < list.count; i++) {
    print_one_student(current->data, i + 1);
    current = current->next;
  }
}
