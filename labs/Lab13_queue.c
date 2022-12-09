/*
 * 작성일: 2022-12-09
 * 학  번: 20220323
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING 20

typedef struct node {
  char data;
  struct node *next;
} NODE;

typedef struct {
  int count;
  NODE *front;
  NODE *rear;
} QUEUE;

void printQueue(QUEUE *q);
int enqueue(QUEUE *q, char data);
int dequeue(QUEUE *q, char *data);
int isEmpty(QUEUE *q);

int main() {
  int len, i, ioRes;
  char str[MAX_STRING];
  QUEUE queue;

  queue.count = 0;
  queue.front = queue.rear = NULL;

  while (1) {
    printf(">> ");
    ioRes = scanf("%s", str);

    if (ioRes == EOF)
      break;

    len = strlen(str);
    for (i = 0; i < len; i++) {
      enqueue(&queue, str[i]);
    }

    printQueue(&queue);
    char dummy;
    for (i = 0; i < len; i++)
      dequeue(&queue, &dummy);
  }

  printf("bye!\n");

  return 0;
} // main

void printQueue(QUEUE *q) {
  char data;

  printf("   ");
  NODE *current = q->front;
  while (current != NULL) {
    data = current->data;
    printf("%c", data);
    current = current->next;
  }
  printf("\n\n");
}

int enqueue(QUEUE *q, char data) {
  NODE *temp;

  temp = (NODE *)malloc(sizeof(NODE));
  if (!temp)
    return 0;

  temp->data = data;
  temp->next = NULL;

  if (isEmpty(q)) {
    q->count = 1;
    q->front = temp;
    q->rear = temp;
  } else {
    q->count++;
    q->rear->next = temp;
    q->rear = temp;
  }

  return 1;
}

int dequeue(QUEUE *q, char *data) {
  NODE *temp;

  if (isEmpty(q))
    return 0;

  temp = q->front;
  *data = temp->data;
  q->front = temp->next;
  q->count--;
  free(temp);

  return 1;
}

int isEmpty(QUEUE *q) { return q->count == 0; }
