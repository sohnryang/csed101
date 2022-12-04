#pragma once

#include <stdbool.h>

#define MAX_TITLE_LEN 14
#define MAX_ARTIST_NAME_LEN 14
#define MAX_FILE_SIZE 50.0
#define MAX_FILENAME_LEN 30

#define RES_OK 0
#define RES_ERR_SPACE 1
#define RES_ERR_DUPE 2
#define RES_NOT_FOUND 3

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

Node *remove_list(List *list, char *title, int *index);

List *sort_list(List *list);

void free_list(List *list);

List *load_list(char *filename);

void print_table_header();

void print_table_footer(int len, float total_size);

void print_song(int num, Song song);

void show_fn(List *list, int len, bool favorite_only);

void show_favorites_fn(List *list);

void delete_fn(List *list);

void add_fn(List *list);

void exit_fn(List *list);
