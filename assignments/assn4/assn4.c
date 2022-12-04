/*
 * CSED101 Assignment #4
 * Submission by Ryang Sohn (ryangsohn@postech.ac.kr)
 * Compiled & Tested on macOS 13.0 @ AArch64 using clang v15.0.6
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "functions.h"

#define MAX_FILENAME_LEN 30
#define MAX_CMD_LEN 20

int main() {
  List *playlist;
  while (true) {
    printf("음악 리스트 파일 이름을 입력해주세요. >> ");
    char filename[MAX_FILENAME_LEN + 1];
    scanf("%s", filename);
    playlist = load_list(filename);
    if (playlist != NULL)
      break;
    printf("유효하지 않은 파일입니다. ");
  }
  printf("\n");

  while (true) {
    printf("명령어를 입력해주세요. >> ");
    char cmd[MAX_CMD_LEN + 1];
    scanf("%s", cmd);
    if (!strcmp(cmd, "show"))
      show_fn(playlist);
    else if (!strcmp(cmd, "show_favorites")) {
    } else if (!strcmp(cmd, "add")) {
    } else if (!strcmp(cmd, "delete")) {
    } else if (!strcmp(cmd, "exit")) {
    } else
      printf("유효하지 않은 명령어입니다.\n");
    printf("\n");
  }
  return 0;
}
