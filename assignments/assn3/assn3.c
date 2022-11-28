/*
 * CSED101: Assignment #3
 * Submission by Ryang Sohn (ryangsohn@postech.ac.kr)
 * Compiled & Tested on macOS 13.0.0 @ AArch64 using clang v15.0.5
 */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Supported operations.
#define OP_GEN 1  // create ladder board
#define OP_RUN 2  // start the game
#define OP_EXIT 3 // exit the program

// Color codes.
#define COLOR_RED 91
#define COLOR_GREEN 92
#define COLOR_YELLOW 93
#define COLOR_BLUE 94
#define COLOR_PURPLE 95

// Set terminal output color according to color code.
void set_output_color(int code);

// Reset terminal output color.
void reset_output_color();

// Clear the terminal.
void clear_screen();

// Get input from menu.
int menu_input();

// Allocate 2d array using `calloc`.
int **alloc_2d(int height, int width);

// Get ladder info from user input.
void ladder_input(int *parties, int *height, int *line_count, char *filename);

// Draw vertical line to `board` for `party` number of people and height
// `height`.
void draw_vertical(int **board, int parties, int height);

// Randomly generate ladder for `party` number of people with height `height`.
void generate_ladder(int **board, int num_line, int parties, int height);

// Save ladder to `filename`.
void save_ladder(char filename[], int **board, int parties, int height,
                 int num_line);

// Deallocate 2d array with height `height`.
void free_ladder(int **board, int height);

// Load ladder from `filename`. Save relevant infos to variables referenced by
// `parites`, `height`, `num_line`. Return `NULL` if `fopen` failed.
int **load_ladder(char *filename, int *parties, int *height, int *num_line);

// Print ladder saved in board. Colors are set according to `visited_map`.
void print_ladder(int **board, int **visited_map, int parties, int height);

// Traverse one step when current position is
void traverse_step(int **board, int **visited_map, int parties, int height,
                   int *y, int *x, int source);

int main() {
  srand(time(NULL)); // set random seed to current time
  int **ladder_board;
  while (true) {
    int cmd = menu_input();
    if (cmd == OP_GEN) {
      int parties, height, line_count;
      char filename[21];
      printf("\n");
      ladder_input(&parties, &height, &line_count, filename);
      printf("\n");
      // Each line contains `parties` slots for vertical lines, and `parties -
      // 1` slots for horizontal lines.
      ladder_board = alloc_2d(height, parties * 2 - 1);
      generate_ladder(ladder_board, line_count, parties, height);
      save_ladder(filename, ladder_board, parties, height, line_count);
      // `ladder_board` will be unused, so deallocate it.
      free_ladder(ladder_board, height);
    } else if (cmd == OP_RUN) {
      printf("\n");
      printf("파일 이름: ");
      char filename[21];
      scanf("%s", filename);
      printf("\n");
      int parties, height, num_line;
      // Load ladder from `filename`.
      ladder_board = load_ladder(filename, &parties, &height, &num_line);
      if (ladder_board == NULL) {
        printf("File not found: %s\n\n", filename);
        continue;
      }
      int **visited_map = alloc_2d(height, parties * 2 - 1), y = -1, x = -1,
          current_source = 0;
      bool print_full = false;
      while (true) {
        clear_screen(); // clear screen before printing current turn
        if (y != -1) {
          printf("%d %d\n", y, x);
        }
        print_ladder(ladder_board, visited_map, parties, height);
        printf("\n");
        if (y <= 0) {
          if (current_source) {
            // If the ladder was fully painted in the last pass, results should
            // be printed differently.
            if (print_full) {
              print_full = false;
              char *dests = calloc(parties + 1, sizeof(char));
              // The top level does not contain horizontal lines, so
              // destinations can be determined from the top level of
              // `visited_map`.
              for (int i = 0; i < parties; i++)
                dests[visited_map[0][2 * i]] = 'A' + i;
              for (int i = 1; i <= parties; i++)
                printf("%d -> %c\n", i, dests[i]);
              printf("\n");
            } else
              printf("result: %d -> %c\n\n", current_source, 'A' + x / 2);
          }
          // Each line contains `parties` slots for vertical lines, and `parties
          // - 1` slots for horizontal lines.
          int width = 2 * parties - 1;
          // Mark already visited cells as stale.
          for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
              if (visited_map[y][x] > 0)
                visited_map[y][x] = -visited_map[y][x];
          printf(">> ");
          scanf("%d", &current_source);
          // If `current_source` is zero even after input, it is terminal sign.
          if (current_source == 0) {
            printf("\n");
            break;
          }
          // If `current_source` is -1, fully paint the ladder.
          if (current_source == -1) {
            // Deallocate current `visited_map` and create a new one.
            free_ladder(visited_map, height);
            // Each line contains `parties` slots for vertical lines, and
            // `parties - 1` slots for horizontal lines.
            visited_map = alloc_2d(height, parties * 2 - 1);
            // Traverse every party.
            for (int i = 0; i < parties; i++) {
              y = height;
              x = i * 2;
              // Loop until current position is top.
              while (y > 0)
                traverse_step(ladder_board, visited_map, parties, height, &y,
                              &x, i + 1);
            }
            // Set current position to the origin.
            y = 0;
            x = 0;
            // Print all source-destination pair.
            print_full = true;
            continue;
          }
          // Start from the selected position.
          y = height;
          x = (current_source - 1) * 2;
        } else {
          char ch;
          // Workaround: if we are starting from the bottom, there is
          // "unconsumed" newline character left in the buffer.
          if (y == height - 1)
            scanf("%c", &ch);
          scanf("%c", &ch);
        }
        traverse_step(ladder_board, visited_map, parties, height, &y, &x,
                      current_source);
      }
      // `ladder_board` and `visited_map` are unused from now, so deallocate
      // them.
      free_ladder(ladder_board, height);
      free_ladder(visited_map, height);
    } else if (cmd == OP_EXIT) {
      break;
    } else
      return 1; // this must not happen
  }
  return 0;
}

void set_output_color(int code) { printf("\033[%dm", code); }

void reset_output_color() { printf("\033[0m"); }

void clear_screen() {
  system("clear"); // for UNIX OSes
}

int **alloc_2d(int height, int width) {
  int **board = (int **)calloc(height, sizeof(int *));
  for (int i = 0; i < height; i++)
    board[i] = (int *)calloc(width, sizeof(int));
  return board;
}

int menu_input() {
  printf("[사다리 게임]\n");
  printf("=======================================\n");
  printf("1. 사다리 보드 생성\n");
  printf("2. 사다리 타기 시작\n");
  printf("3. 종료\n");
  printf("=======================================\n");
  printf("선택: ");
  int input;
  scanf("%d", &input);
  return input;
}

void ladder_input(int *parties, int *height, int *line_count, char *filename) {
  printf("참여 인원수: ");
  scanf("%d", parties);
  printf("사다리 높이: ");
  scanf("%d", height);
  printf("가로줄 개수: ");
  scanf("%d", line_count);
  printf("파일이름: ");
  scanf("%s", filename);
}

void draw_vertical(int **board, int parties, int height) {
  for (int y = 0; y < height; y++)
    for (int x = 0; x < 2 * parties - 1; x += 2)
      board[y][x] = 1;
}

void generate_ladder(int **board, int num_line, int parties, int height) {
  draw_vertical(board, parties, height);

  int count = 0;
  while (count < num_line) {
    int y = 1 + rand() % (height - 2);
    int x = rand() % (parties - 1) * 2 + 1;
    if (board[y][x])
      continue;
    if (x > 2 && board[y][x - 2])
      continue;
    if (x < 2 * parties - 3 && board[y][x + 2])
      continue;
    board[y][x] = 1;
    count++;
  }
}

void save_ladder(char filename[], int **board, int parties, int height,
                 int num_line) {
  FILE *outfile = fopen(filename, "w");
  fprintf(outfile, "%d %d %d\n", parties, height, num_line);
  int width = 2 * parties - 1;
  for (int y = 1; y < height - 1; y++)
    for (int x = 1; x < width; x += 2)
      if (board[y][x])
        fprintf(outfile, "%d %d\n", y, (x + 1) / 2);
  fclose(outfile);
}

void free_ladder(int **board, int height) {
  for (int i = 0; i < height; i++)
    free(board[i]);
  free(board);
}

int **load_ladder(char *filename, int *parties, int *height, int *num_line) {
  FILE *infile = fopen(filename, "r");
  if (infile == NULL)
    return NULL;
  int **board;
  fscanf(infile, "%d %d %d", parties, height, num_line);
  board = alloc_2d(*height, *parties * 2 - 1);
  draw_vertical(board, *parties, *height);
  for (int i = 0; i < *num_line; i++) {
    int y, party;
    fscanf(infile, "%d %d", &y, &party);
    board[y][2 * party - 1] = 1;
  }
  return board;
}

void print_ladder(int **board, int **visited_map, int parties, int height) {
  for (int i = 0; i < parties; i++)
    printf("  %c ", 'A' + i);
  printf("\n");
  int width = parties * 2 - 1;
  const int color_table[5] = {COLOR_PURPLE, COLOR_RED, COLOR_GREEN,
                              COLOR_YELLOW, COLOR_BLUE};
  for (int y = 0; y < height; y++) {
    printf("  ");
    for (int x = 0; x < width; x++) {
      if (visited_map[y][x])
        set_output_color(color_table[abs(visited_map[y][x]) % 5]);
      if (x % 2 == 0)
        printf("+");
      else if (board[y][x])
        printf("---");
      else
        printf("   ");
      if (visited_map[y][x])
        reset_output_color();
    }
    printf("\n");
  }
  for (int i = 0; i < parties; i++)
    printf(" %2d ", i + 1);
  printf("\n");
}

void traverse_step(int **board, int **visited_map, int parties, int height,
                   int *y, int *x, int source) {
  int width = 2 * parties - 1;
  if (*y == height)
    *y -= 1;
  else if (*x > 0 && board[*y][*x - 1] && visited_map[*y][*x - 1] != source)
    *x -= 1;
  else if (*x < width - 1 && board[*y][*x + 1] &&
           visited_map[*y][*x + 1] != source)
    *x += 1;
  else
    *y -= 1;
  visited_map[*y][*x] = source;
}
