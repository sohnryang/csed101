#pragma once

#define MAX_TITLE_LEN 14
#define MAX_ARTIST_NAME_LEN 14
#define MAX_FILE_SIZE 50.0
#define MAX_FILENAME_LEN 30

#define OP_OK 1
#define OP_ERR 0

typedef struct Song {
  char title[MAX_TITLE_LEN + 1], artist[MAX_ARTIST_NAME_LEN + 1];
  float file_size, preference;
} Song;

typedef struct Node {
  Song data;
  struct Node *next;
} Node;

typedef struct List {
  int size;
  float file_size;
  Node *head;
} List;

List *new_list();

Node *new_node(Song song);

int append_list(List *list, Song song);

void remove_list(List *list, char *title);

List *sort_list(List *list);

List *load_list(char *filename);

void show_fn(List *list);

void exit_fn(List *list);
