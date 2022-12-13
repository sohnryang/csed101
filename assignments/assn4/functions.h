#pragma once

#include <stdbool.h>

// Maximum length of song title.
#define MAX_TITLE_LEN 14

// Maximum length of artist name.
#define MAX_ARTIST_NAME_LEN 14

// Maximum total file size of list.
#define MAX_FILE_SIZE 50.0

// Maximum filename length.
#define MAX_FILENAME_LEN 30

//
#define RES_OK 0
#define RES_ERR_SPACE 1
#define RES_ERR_DUPE 2
#define RES_NOT_FOUND 3

// Song info.
typedef struct Song {
  // Title of the song.
  char title[MAX_TITLE_LEN + 1];

  // Artist of the song.
  char artist[MAX_ARTIST_NAME_LEN + 1];

  // File size of the song.
  float file_size;

  // Preference of the song.
  float preference;
} Song;

// Node info.
typedef struct Node {
  // Song data held by node.
  Song data;

  // Pointer to the next node.
  struct Node *next;
} Node;

// List info.
typedef struct List {
  // Number of nodes in the list.
  int size;

  // Total file size of songs.
  float file_size;

  // Pointer to the first node.
  Node *head;
} List;

// Create a new list and return pointer to the created list.
List *new_list();

// Create a new node from song data.
Node *new_node(Song song);

// Append a node which holds data from `song` to `list`.
int append_list(List *list, Song song);

// Remove a node with song title `title` from `list`, set deleted index and
// return removed node.
Node *remove_list(List *list, char *title, int *index);

// Create a list sorted by preference and return it.
List *sort_list(List *list);

// Deallocate list.
void free_list(List *list);

// Load the list from file saved in `filename`.
List *load_list(char *filename);

// Print the table header.
void print_table_header();

// Print the table footer.
void print_table_footer(int len, float total_size);

// Print individual song.
void print_song(int num, Song song);

// Handle "show" command.
void show_fn(List *list, int len, bool favorite_only);

// Handle "show_favorites" command.
void show_favorites_fn(List *list);

// Handle "delete" command.
void delete_fn(List *list);

// Handle "add" command.
void add_fn(List *list);

// Handle "exit" command.
void exit_fn(List *list);
