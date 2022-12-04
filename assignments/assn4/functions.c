#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

List *new_list() {
  List *created = (List *)malloc(sizeof(List));
  created->head = NULL;
  created->file_size = 0;
  created->size = 0;
  return created;
}

Node *new_node(Song song) {
  Node *created = (Node *)malloc(sizeof(Node));
  created->next = NULL;
  strcpy(created->data.title, song.title);
  strcpy(created->data.artist, song.artist);
  created->data.file_size = song.file_size;
  created->data.preference = song.preference;
  return created;
}

int append_list(List *list, Song song) {
  if (list->file_size + song.file_size > MAX_FILE_SIZE)
    return OP_ERR;
  list->file_size += song.file_size;
  Node *inserted = new_node(song);
  if (list->size++ == 0) {
    list->head = inserted;
    return OP_OK;
  }
  Node *leader = NULL, *current = list->head;
  while (current != NULL) {
    if (strcmp(song.title, current->data.title) < 0)
      break;
    leader = current;
    current = current->next;
  }
  if (leader == NULL) {
    inserted->next = current;
    list->head = inserted;
    return OP_OK;
  }
  leader->next = inserted;
  inserted->next = current;
  return OP_OK;
}

void remove_list(List *list, char *title) {
  Node *leader = NULL, *current = list->head;
  while (current != NULL) {
    if (!strcmp(current->data.title, title))
      break;
    leader = current;
    current = current->next;
  }
  list->size--;
  list->file_size -= current->data.file_size;
  if (leader == NULL) {
    list->head = current->next;
    free(current);
    return;
  }
  leader->next = current->next;
  free(current);
}

List *sort_list(List *list) {
  List *sorted = new_list();
  Node *node = list->head;
  while (node != NULL) {
    Node *inserted = new_node(node->data);
    if (sorted->size++ == 0)
      sorted->head = inserted;
    else {
      Node *leader = NULL, *current = sorted->head;
      while (current != NULL) {
        if (inserted->data.preference > current->data.preference)
          break;
        leader = current;
        current = current->next;
      }
      if (leader == NULL) {
        inserted->next = current;
        sorted->head = inserted;
      } else {
        leader->next = inserted;
        inserted->next = current;
      }
    }
    node = node->next;
  }
  return sorted;
}

List *load_list(char *filename) {
  FILE *infile = fopen(filename, "r");
  if (infile == NULL)
    return NULL;
  List *list = new_list();
  Song song;
  while (fscanf(infile, "%s %s %f %f", song.title, song.artist, &song.file_size,
                &song.preference) != EOF) {
    int ok = append_list(list, song);
    if (!ok)
      printf("용량 초과! 음악(%s)은 추가되지 않았습니다.\n", song.title);
  }
  fclose(infile);
  return list;
}

void show_fn(List *list) {
  if (list->size == 0) {
    printf("Empty Playlist!\n");
    return;
  }
  for (int i = 0; i < 22; i++)
    printf(" ");
  printf("PLAYLIST\n");
  for (int i = 0; i < 84; i++)
    printf("=");
  printf("\n");
  printf(" No.");
  for (int i = 0; i < 10; i++)
    printf(" ");
  printf("Title");
  for (int i = 0; i < 18; i++)
    printf(" ");
  printf("Artist");
  for (int i = 0; i < 13; i++)
    printf(" ");
  printf("Volume");
  for (int i = 0; i < 10; i++)
    printf(" ");
  printf("Preference\n");
  for (int i = 0; i < 84; i++)
    printf("-");
  printf("\n");
  int number = 1;
  Node *current = list->head;
  while (current != NULL) {
    printf("#%-3d|%15s     |%15s     |%11.2f MB    |%8.2f\n", number++,
           current->data.title, current->data.artist, current->data.file_size,
           current->data.preference);
    current = current->next;
  }
  for (int i = 0; i < 84; i++)
    printf("-");
  printf("\n");
  printf("Total number of songs: %d\n", list->size);
  printf("Storage: %5.2f MB\n", list->file_size);
  for (int i = 0; i < 84; i++)
    printf("=");
  printf("\n");
}

void exit_fn(List *list) {
  printf("저장할 파일명을 입력해주세요. >> ");
  char filename[MAX_FILENAME_LEN + 1];
  scanf("%s", filename);
  FILE *outfile = fopen(filename, "w");
  Node *current = list->head;
  while (current != NULL) {
    fprintf(outfile, "%s\t%s\t%f\t%f\n", current->data.title,
            current->data.artist, current->data.file_size,
            current->data.preference);
    current = current->next;
  }
  printf("프로그램을 종료합니다.\n");
  fclose(outfile);
}
