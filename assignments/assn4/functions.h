#pragma once

typedef struct Song {
  char title[15], artist[15];
  float file_size, preference;
} Song;

typedef struct Node {
  Song data;
  struct Node *next;
} Node;

typedef struct List {
  int size;
  Node *head, *tail;
} List;
