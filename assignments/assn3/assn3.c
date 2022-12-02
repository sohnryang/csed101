/*
 * CSED101: Assignment #3
 * Submission by Ryang Sohn (ryangsohn@postech.ac.kr)
 * Compiled & Tested on macOS 13.0.0 @ AArch64 using clang v15.0.5
 */

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
void free_ladder(int **arr, int height);

// Load ladder from `filename`. Save relevant infos to variables referenced by
// `parites`, `height`, `num_line`. Return `NULL` if `fopen` failed.
int **load_ladder(char filename[], int *parties, int *height, int *num_line);

// Print ladder saved in board. Colors are set according to `visited_map`.
void print_ladder(int **board, int **visited_map, int parties, int height);

// Traverse one step when current position is referenced by `x` and `y`.
// `visited_map` holds number of source for every visited cell in
// `ladder_board`.
// There are two types of cells: active and stale. Active cells
// are cells that are being visited in current pass. Stale cells are cells that
// are visited in previous passes. Stale cells are marked by negative sign.
void traverse_step(int **board, int **visited_map, int parties, int height,
                   int *y, int *x, int source);

int main() {
  srand(time(NULL));         // set random seed to current time
  int **ladder_board = NULL; // 2d array to hold ladder data.
  while (true) {
    int cmd = menu_input(); // command input
    if (cmd == OP_GEN) {
      int parties;       // number of participants
      int height;        // height of ladder board
      int line_count;    // number of horizontal lines
      char filename[21]; // path to file saved
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
      char filename[21]; // name of file to read
      scanf("%s", filename);
      printf("\n");
      int parties;  // number of participants
      int height;   // height of ladder board
      int num_line; // number of horizontal lines
      // Load ladder from `filename`.
      ladder_board = load_ladder(filename, &parties, &height, &num_line);
      if (ladder_board == NULL) {
        printf("File not found: %s\n\n", filename);
        continue;
      }
      int **visited_map = alloc_2d(height, parties * 2 - 1);
      int y = -1, x = -1;      // current y and x coordinates
      int current_source = 0;  // source of current pass
      bool print_full = false; // whether last pass was "paint full" status
      while (true) {
        clear_screen(); // clear screen before printing current turn
        // If `y` is not -1, assume that `y` and `x` hold meaningful position.
        if (y != -1)
          printf("%d %d\n", y, x);
        print_ladder(ladder_board, visited_map, parties, height);
        printf("\n");
        // If `y` is not set or zero, start from some source.
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
              free(dests);
            } else
              printf("result: %d -> %c\n\n", current_source, 'A' + x / 2);
          }
          // Each line contains `parties` slots for vertical lines, and `parties
          // - 1` slots for horizontal lines.
          int width = 2 * parties - 1;
          // Mark already visited cells as stale by flipping signs of positive
          // numbers.
          for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
              if (visited_map[y][x] > 0)
                visited_map[y][x] = -visited_map[y][x];
          printf(">> ");
          scanf("%d", &current_source);
          // If `current_source` is zero even after input, terminate.
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
          char ch; // dummy variable to hold newline character
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
  // Allocate `height` slots for each rows.
  int **board = (int **)calloc(height, sizeof(int *));
  for (int i = 0; i < height; i++)
    board[i] = (int *)calloc(width, sizeof(int)); // allocate each row
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
  // Vertical lines should be placed on points with even `x` coordinate.
  for (int y = 0; y < height; y++)
    for (int x = 0; x < 2 * parties - 1; x += 2)
      board[y][x] = 1;
}

void generate_ladder(int **board, int num_line, int parties, int height) {
  draw_vertical(board, parties, height);

  int count = 0; // number of horizontal lines created
  while (count < num_line) {
    // Horizontal liens can be created everywhere except top and bottom lines,
    // and next to other lines.
    // y coordinate to place horizontal line.
    int y = 1 + rand() % (height - 2);
    // x coordinate to place horizontal line.
    int x = rand() % (parties - 1) * 2 + 1;
    if (board[y][x]) // check if already placed
      continue;
    if (x > 2 && board[y][x - 2]) // check if horizontal line exists on the left
      continue;
    if (x < 2 * parties - 3 && board[y][x + 2]) // check if exists on the right
      continue;
    board[y][x] = 1;
    count++;
  }
}

void save_ladder(char filename[], int **board, int parties, int height,
                 int num_line) {
  FILE *outfile = fopen(filename, "w");
  fprintf(outfile, "%d %d %d\n", parties, height, num_line);
  int width = 2 * parties - 1; // width of board
  for (int y = 1; y < height - 1; y++)
    for (int x = 1; x < width; x += 2)
      if (board[y][x]) // save coordinate of horizontal line.
        fprintf(outfile, "%d %d\n", y, (x + 1) / 2);
  fclose(outfile);
}

void free_ladder(int **board, int height) {
  // Deallocate every row.
  for (int i = 0; i < height; i++)
    free(board[i]);
  // Deallocate array of row pointers.
  free(board);
}

int **load_ladder(char filename[], int *parties, int *height, int *num_line) {
  FILE *infile = fopen(filename, "r");
  if (infile == NULL) // check if file is opened properly
    return NULL;
  fscanf(infile, "%d %d %d", parties, height, num_line);
  // Allocate a new board.
  int **board = alloc_2d(*height, *parties * 2 - 1);
  draw_vertical(board, *parties, *height);
  for (int i = 0; i < *num_line; i++) {
    int y;           // y coordinate of horizontal line
    int participant; // participant the line belongs to
    fscanf(infile, "%d %d", &y, &participant);
    board[y][2 * participant - 1] = 1; // mark horizontal line.
  }
  return board;
}

void print_ladder(int **board, int **visited_map, int parties, int height) {
  // Print header of ladders.
  for (int i = 0; i < parties; i++)
    printf("  %c ", 'A' + i);
  printf("\n");
  int width = parties * 2 - 1;
  // Mapping from `source % 5` to color code.
  const int color_table[5] = {COLOR_PURPLE, COLOR_RED, COLOR_GREEN,
                              COLOR_YELLOW, COLOR_BLUE};
  for (int y = 0; y < height; y++) {
    printf("  ");
    for (int x = 0; x < width; x++) {
      // If the cell was previously visited, set color accordingly.
      if (visited_map[y][x]) {
        // Calculate absolute value because active and stale cells print the
        // same way.
        int source =
            (visited_map[y][x] < 0 ? -visited_map[y][x] : visited_map[y][x]);
        set_output_color(color_table[source % 5]);
      }
      if (x % 2 == 0) // if vertical line is being printed
        printf("+");
      else if (board[y][x]) // if horizontal lien exists
        printf("---");
      else // if horizontal line does not exist
        printf("   ");
      if (visited_map[y][x])
        reset_output_color();
    }
    printf("\n");
  }
  // Print footer.
  for (int i = 0; i < parties; i++)
    printf(" %2d ", i + 1);
  printf("\n");
}

void traverse_step(int **board, int **visited_map, int parties, int height,
                   int *y, int *x, int source) {
  int width = 2 * parties - 1;
  // If this is first pass, decrement variable referenced by `y`.
  if (*y == height)
    *y -= 1;
  // Check if moving to the left is possible and the cell has not been visited
  // yet.
  else if (*x > 0 && board[*y][*x - 1] && visited_map[*y][*x - 1] != source)
    *x -= 1;
  // Check if moving to the right is possible and the cell has not been visited
  // yet.
  else if (*x < width - 1 && board[*y][*x + 1] &&
           visited_map[*y][*x + 1] != source)
    *x += 1;
  // Moving sideways is not possible, so move upward.
  else
    *y -= 1;
  visited_map[*y][*x] = source;
}
