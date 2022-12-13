#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

List *new_list() {
  List *created = (List *)malloc(sizeof(List)); // allocate list
  created->head = NULL;   // head points to nothing as there are no nodes
  created->file_size = 0; // initially, the total size must be zero
  created->size = 0;      // likewise, the number of nodes is zero
  return created;
}

Node *new_node(Song song) {
  Node *created = (Node *)malloc(sizeof(Node)); // allocate node
  created->next = NULL; // initially, the next node does not exist
  created->data = song; // copy song data to created node's data.
  return created;
}

int append_list(List *list, Song song) {
  // Check if space is sufficient.
  if (list->file_size + song.file_size > MAX_FILE_SIZE)
    return RES_ERR_SPACE;
  // `current` points to the current node, and `leader` points to the node
  // before `current`.
  Node *inserted = new_node(song), *leader = NULL, *current = list->head;
  while (current != NULL) {
    // Calculate difference of song.title and current->data.title using
    // `strcmp`.
    int diff = strcmp(song.title, current->data.title);
    if (diff < 0)
      break;
    // Check if a node with same name exists.
    else if (diff == 0)
      return RES_ERR_DUPE;

    // Move on to the next node.
    leader = current;
    current = current->next;
  }
  list->size++;                      // increment the number of nodes by one.
  list->file_size += song.file_size; // add to total file size.

  // If inserting to the first node
  if (leader == NULL) {
    inserted->next = current; // current node comes after inserted node
    list->head = inserted;    // inserted node is the head
    return RES_OK;
  }
  leader->next = inserted;  // inserted node comes after the leader node
  inserted->next = current; // current node comes after the inserted node
  return RES_OK;
}

Node *remove_list(List *list, char *title, int *index) {
  Node *leader = NULL, *current = list->head;
  *index = -1;
  int i = 0;
  while (current != NULL) {
    // Check if title matches.
    if (!strcmp(current->data.title, title))
      break;
    leader = current;
    current = current->next;
    i++;
  }
  // Check if the title matched nothing.
  if (current == NULL)
    return NULL;
  *index = i;
  list->size--;                               // decrement list size by one
  list->file_size -= current->data.file_size; // substract file size from total
  if (leader == NULL) {
    list->head = current->next; // the second node is the first node
    return current;
  }
  leader->next = current->next; // the node after current comes after leader
  return current;
}

List *sort_list(List *list) {
  List *sorted = new_list(); // create a new list
  Node *node = list->head;
  // Loop over original list.
  while (node != NULL) {
    Node *inserted = new_node(node->data); // create a new node for sorted list.

    // Make pointers for leader and current, just like in append_list.
    Node *leader = NULL, *current = sorted->head;
    while (current != NULL) {
      // Check if inserting here is correct.
      if (inserted->data.preference > current->data.preference)
        break;
      // Check if inserting here is correct, when preferences are the same.
      if (inserted->data.preference == current->data.preference &&
          strcmp(inserted->data.title, current->data.title) < 0)
        break;
      // Move on to the next node.
      leader = current;
      current = current->next;
    }
    // Check if inserting to the first.
    if (leader == NULL) {
      inserted->next = current; // current node comes after inserted node
      sorted->head = inserted;  // inserted node is the head
    } else {
      leader->next = inserted;  // inserted node comes after the leader node
      inserted->next = current; // current node comes after the inserted node
    }
    sorted->size++;                                // increment list size by one
    sorted->file_size += inserted->data.file_size; // add file size to total
    node = node->next;                             // move on to the next node
  }
  return sorted;
}

void free_list(List *list) {
  Node *current = list->head;
  // Loop over the list
  while (current != NULL) {
    // To avoid use-after-free bugs, copy the pointer to the next node before
    // deallocating current node.
    Node *next = current->next;
    free(current);  // free the current node
    current = next; // move on to the next node
  }
}

List *load_list(char *filename) {
  FILE *infile = fopen(filename, "r"); // open file in read mode

  // Check if file open was successful.
  if (infile == NULL)
    return NULL;
  // Create a new list.
  List *list = new_list();
  Song song;
  // Repeat until the end of file is reached.
  while (fscanf(infile, "%s %s %f %f", song.title, song.artist, &song.file_size,
                &song.preference) != EOF) {
    int not_ok = append_list(list, song);
    // Check if inserting to list was successful.
    if (not_ok)
      printf("용량 초과! 음악(%s)은 추가되지 않았습니다.\n", song.title);
  }
  fclose(infile); // close the file
  return list;
}

void print_table_header() {
  // Print the top line.
  for (int i = 0; i < 80; i++)
    printf("=");
  printf("\n");
  printf(" No.");
  // Print the spacing.
  for (int i = 0; i < 10; i++)
    printf(" ");
  printf("Title");
  // Print the spacing.
  for (int i = 0; i < 14; i++)
    printf(" ");
  printf("Artist");
  // Print the spacing.
  for (int i = 0; i < 13; i++)
    printf(" ");
  printf("Volume");
  // Print the spacing.
  for (int i = 0; i < 10; i++)
    printf(" ");
  printf("Preference\n");
  // Print the bottom line.
  for (int i = 0; i < 80; i++)
    printf("-");
  printf("\n");
}

void print_table_footer(int len, float total_size) {
  // Print a line.
  for (int i = 0; i < 80; i++)
    printf("-");
  printf("\n");
  // Print storage and song count.
  printf("Total number of songs: %d\n", len);
  printf("Storage: %.2f MB\n", total_size);
  // Print a line.
  for (int i = 0; i < 80; i++)
    printf("=");
  printf("\n");
}

void print_song(int num, Song data) {
  // Print formatted song info.
  printf("#%-3d|%14s     |%14s     |%11.2f MB    |%8.2f\n", num, data.title,
         data.artist, data.file_size, data.preference);
}

void show_fn(List *list, int len, bool favorite_only) {
  // Check if only favorite songs will be printed.
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
      total_size += current->data.file_size; // accumulate total size
      current = current->next;               // move on to the next node
    }
  }
  print_table_footer(len, total_size);
}

void show_favorites_fn(List *list) {
  printf("상위 몇 개의 음악을 추출할까요? >> ");
  int len;
  scanf("%d", &len);
  // Check if too many songs were requested.
  if (len > list->size) {
    printf("추출하고자 하는 음악의 수가 전체 음악의 수보다 많습니다.\n");
    return;
  }
  // Check if too few songs were requested.
  if (len < 1) {
    printf("추출하고자 하는 음악의 수는 1 이상이어야 합니다.\n");
    return;
  }
  List *sorted = sort_list(list); // sort the list
  show_fn(sorted, len, true);     // call `show_fn` with appropriate args

  // Deallocate sorted list since it won't be used later
  free_list(sorted);
  free(sorted);
}

void delete_fn(List *list) {
  printf("삭제할 음악의 타이틀을 입력해주세요. >> ");
  char title[MAX_TITLE_LEN + 1];
  scanf("%s", title); // get title of the song to remove
  int index;
  // Delete a node by its title.
  Node *deleted = remove_list(list, title, &index);
  if (deleted == NULL) {
    printf("플레이리스트에 해당 음악(%s)이 없습니다.\n", title);
    return;
  }
  print_table_header(); // print table header
  print_song(index + 1, deleted->data);
  free(deleted); // deallocate the deleted node
  // Print a line.
  for (int i = 0; i < 80; i++)
    printf("-");
  printf("\n");
  printf("위 음악이 삭제되었습니다.\n");
}

void add_fn(List *list) {
  Song song;
  // Get input for song.
  printf("추가할 음악의 타이틀을 입력해주세요. >> ");
  scanf("%s", song.title); // get title from stdin
  printf("추가할 음악의 아티스트를 입력해주세요. >> ");
  scanf("%s", song.artist); // get artist from stdin
  printf("추가할 음악의 용량을 입력해주세요. >> ");
  scanf("%f", &song.file_size); // get file size from stdin
  printf("추가할 음악의 선호도를 입력해주세요. >> ");
  scanf("%f", &song.preference); // get preference from stdin
  int not_ok = append_list(list, song);
  // Check if the song was not added due to storage limit.
  if (not_ok == RES_ERR_SPACE)
    printf("용량 초과! 음악(%s)은 추가되지 않았습니다.\n", song.title);
  // Check if the song was not added because it had duplicate.
  if (not_ok == RES_ERR_DUPE)
    printf("해당 음악이 이미 플레이리스트 내에 존재합니다.\n");
}

void exit_fn(List *list) {
  printf("저장할 파일명을 입력해주세요. >> ");
  char filename[MAX_FILENAME_LEN + 1];
  scanf("%s", filename);
  // Open file to save.
  FILE *outfile = fopen(filename, "w");
  List *sorted = sort_list(list); // sort list by preference
  Node *current = sorted->head;
  // Loop over the sorted list.
  while (current != NULL) {
    fprintf(outfile, "%s\t%s\t%f\t%f\n", current->data.title,
            current->data.artist, current->data.file_size,
            current->data.preference);
    current = current->next; // move on to the next node
  }
  // Deallocate the sorted list.
  free_list(sorted);
  free(sorted);
  printf("프로그램을 종료합니다.\n");
  fclose(outfile); // close the file
}
