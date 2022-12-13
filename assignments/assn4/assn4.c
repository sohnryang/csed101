/*
 * CSED101 Assignment #4
 * Submission by Ryang Sohn (ryangsohn@postech.ac.kr)
 * Compiled & Tested on macOS 13.0 @ AArch64 using clang v15.0.6
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

// Maximum command length.
#define MAX_CMD_LEN 20

int main() {
  List *playlist;
  char filename[MAX_FILENAME_LEN + 1];
  printf("음악 리스트 파일 이름을 입력해주세요. >> ");
  // Loop until successful read.
  while (true) {
    scanf("%s", filename);
    // Load list from file.
    playlist = load_list(filename);
    // Check if load was successful.
    if (playlist != NULL)
      break; // on successful load, break the input loop
    printf("유효하지 않은 파일입니다. ");
    printf("음악 리스트 파일을 다시 입력해주세요. >> ");
  }
  printf("\n");

  while (true) {
    printf("명령어를 입력해주세요. >> ");
    char cmd[MAX_CMD_LEN + 1]; // create command buffer
    scanf("%s", cmd);
    if (!strcmp(cmd, "show")) // show
      show_fn(playlist, playlist->size, false);
    else if (!strcmp(cmd, "show_favorites")) // show_favorites
      show_favorites_fn(playlist);
    else if (!strcmp(cmd, "add")) // add
      add_fn(playlist);
    else if (!strcmp(cmd, "delete")) // delete
      delete_fn(playlist);
    else if (!strcmp(cmd, "exit")) { // exit
      exit_fn(playlist);
      break;
    } else
      printf("유효하지 않은 명령어입니다.\n");
    printf("\n");
  }

  // Deallocate the list.
  free_list(playlist);
  free(playlist);
  return 0;
}
