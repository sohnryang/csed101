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
    return RES_ERR_SPACE;
  Node *inserted = new_node(song), *leader = NULL, *current = list->head;
  while (current != NULL) {
    int diff = strcmp(song.title, current->data.title);
    if (diff < 0)
      break;
    else if (diff == 0)
      return RES_ERR_DUPE;
    leader = current;
    current = current->next;
  }
  list->size++;
  list->file_size += song.file_size;
  if (leader == NULL) {
    inserted->next = current;
    list->head = inserted;
    return RES_OK;
  }
  leader->next = inserted;
  inserted->next = current;
  return RES_OK;
}

Node *remove_list(List *list, char *title, int *index) {
  Node *leader = NULL, *current = list->head;
  *index = -1;
  int i = 0;
  while (current != NULL) {
    if (!strcmp(current->data.title, title))
      break;
    leader = current;
    current = current->next;
    i++;
  }
  if (current == NULL)
    return NULL;
  *index = i;
  list->size--;
  list->file_size -= current->data.file_size;
  if (leader == NULL) {
    list->head = current->next;
    return current;
  }
  leader->next = current->next;
  return current;
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

void free_list(List *list) {
  Node *current = list->head;
  while (current != NULL) {
    Node *next = current->next;
    free(current);
    current = next;
  }
}

List *load_list(char *filename) {
  FILE *infile = fopen(filename, "r");
  if (infile == NULL)
    return NULL;
  List *list = new_list();
  Song song;
  while (fscanf(infile, "%s %s %f %f", song.title, song.artist, &song.file_size,
                &song.preference) != EOF) {
    int not_ok = append_list(list, song);
    if (not_ok)
      printf("용량 초과! 음악(%s)은 추가되지 않았습니다.\n", song.title);
  }
  fclose(infile);
  return list;
}

void print_table_header() {
  for (int i = 0; i < 80; i++)
    printf("=");
  printf("\n");
  printf(" No.");
  for (int i = 0; i < 10; i++)
    printf(" ");
  printf("Title");
  for (int i = 0; i < 14; i++)
    printf(" ");
  printf("Artist");
  for (int i = 0; i < 13; i++)
    printf(" ");
  printf("Volume");
  for (int i = 0; i < 10; i++)
    printf(" ");
  printf("Preference\n");
  for (int i = 0; i < 80; i++)
    printf("-");
  printf("\n");
}

void print_table_footer(int len, float total_size) {
  for (int i = 0; i < 80; i++)
    printf("-");
  printf("\n");
  printf("Total number of songs: %d\n", len);
  printf("Storage: %.2f MB\n", total_size);
  for (int i = 0; i < 80; i++)
    printf("=");
  printf("\n");
}

void print_song(int num, Song data) {
  printf("#%-3d|%14s     |%14s     |%11.2f MB    |%8.2f\n", num, data.title,
         data.artist, data.file_size, data.preference);
}

void show_fn(List *list, int len, bool favorite_only) {
  if (favorite_only) {
    for (int i = 0; i < 34; i++)
      printf(" ");
    printf("FAVORITES\n");
  } else {
    for (int i = 0; i < 33; i++)
      printf(" ");
    printf("PLAYLIST\n");
  }
  print_table_header();
  float total_size = 0;
  if (len == 0)
    printf("Empty Playlist!\n");
  else {
    Node *current = list->head;
    for (int i = 0; i < len; i++) {
      print_song(i + 1, current->data);
      total_size += current->data.file_size;
      current = current->next;
    }
  }
  print_table_footer(len, total_size);
}

void show_favorites_fn(List *list) {
  printf("상위 몇 개의 음악을 추출할까요? >> ");
  int len;
  scanf("%d", &len);
  if (len > list->size) {
    printf("추출하고자 하는 음악의 수가 전체 음악의 수보다 많습니다.\n");
    return;
  }
  if (len < 1) {
    printf("추출하고자 하는 음악의 수는 1 이상이어야 합니다.\n");
    return;
  }
  List *sorted = sort_list(list);
  show_fn(sorted, len, true);
  free_list(sorted);
  free(sorted);
}

void delete_fn(List *list) {
  printf("삭제할 음악의 타이틀을 입력해주세요. >> ");
  char title[MAX_TITLE_LEN + 1];
  scanf("%s", title);
  int index;
  Node *deleted = remove_list(list, title, &index);
  if (deleted == NULL) {
    printf("플레이리스트에 해당 음악(%s)이 없습니다.\n", title);
    return;
  }
  print_table_header();
  print_song(index + 1, deleted->data);
  free(deleted);
  for (int i = 0; i < 80; i++)
    printf("-");
  printf("\n");
  printf("위 음악이 삭제되었습니다.\n");
}

void add_fn(List *list) {
  Song song;
  printf("추가할 음악의 타이틀을 입력해주세요. >> ");
  scanf("%s", song.title);
  printf("추가할 음악의 아티스트를 입력해주세요. >> ");
  scanf("%s", song.artist);
  printf("추가할 음악의 용량을 입력해주세요. >> ");
  scanf("%f", &song.file_size);
  printf("추가할 음악의 선호도를 입력해주세요. >> ");
  scanf("%f", &song.preference);
  int not_ok = append_list(list, song);
  if (not_ok == RES_ERR_SPACE)
    printf("용량 초과! 음악(%s)은 추가되지 않았습니다.\n", song.title);
  if (not_ok == RES_ERR_DUPE)
    printf("해당 음악이 이미 플레이리스트 내에 존재합니다.\n");
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
