/*
 * CSED101 Assignment $2
 * Submission by Ryang Sohn (ryangsohn@postech.ac.kr)
 * Compiled & Tested on macOS 13.0 @ AArch64 using clang v15.0.3
 */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// Maximum length of image path.
#define MAX_PATH_LEN 74

// Maximum size of the image.
#define SIZE 75

// Accepted absolute error of floating point numbers.
#define ERROR 0.00001

// Supported operations.
#define OP_HIST 1
#define OP_CHANGE_COLOR 2
#define OP_PRINT 3
#define OP_SAVE 4
#define OP_EXIT 5

// Consts for results.
#define RES_SUCCESS 0
#define RES_ERROR 1

// Return maximum value.
float max_f(float a, float b);

// Return minimum value.
float min_f(float a, float b);

// Check if `a` and `b` are absolutely equal.
bool abs_equal(float a, float b);

// Compare floats `a` and `b`. Returns 0 if equal, 1 if `a` > `b`, -1 if
// otherwise.
int compare_f(float a, float b);

// Check if num is in a half-open interval [a, b).
bool in_range(int num, int a, int b);

// Load ppm image file from `filename` to `image_*` arrays and set width and
// height accordingly. `image_*` arrays have dimensions of (C, H, W).
int load_image(const char *filename, int image_rgb[][SIZE][SIZE],
               float image_hsv[][SIZE][SIZE], int *width, int *height);

// Save image stored in `image_rgb` to "output.ppm".
void save_image(int image_rgb[][SIZE][SIZE], int width, int height);

// Print image to the terminal using true-color escape codes.
void print_image(int image_rgb[][SIZE][SIZE], int width, int height);

// Print color histogram to the terminal.
void print_histogram(float image_hsv[][SIZE][SIZE], int width, int height);

// Convert RGB colors to HSV colors.
void rgb_to_hsv(int r, int g, int b, float *h, float *s, float *v);

// Convert HSV colors to RGB colors.
void hsv_to_rgb(float h, float s, float v, int *r, int *g, int *b);

// Convert HSV image array to RGB image array.
void hsv_to_rgb_image(float image_hsv[][SIZE][SIZE],
                      int image_rgb[][SIZE][SIZE], int width, int height);

// Quqntize hue to a integer in half-open interval [0, 12).
int quantize_hue(float h);

// Change overall hue of the image.
void change_color(float image_hsv[][SIZE][SIZE], int width, int height,
                  int source, int target);

// Set terminal output color.
void set_output_color_rgb(int r, int g, int b);

// Reset terminal output color.
void reset_output_color();

// Print menus and get user input.
int choose_menu(const char *filename);

// Get user inputs for source and target colors.
void choose_colors(int *source, int *target);

int main() {
  char filename[MAX_PATH_LEN + 1];
  printf("Enter input filename: ");
  scanf("%s", filename);
  int image_rgb[3][SIZE][SIZE], image_width, image_height;
  float image_hsv[3][SIZE][SIZE];
  int load_result =
      load_image(filename, image_rgb, image_hsv, &image_width, &image_height);
  if (load_result == RES_ERROR) {
    printf("File not found: %s\n", filename);
    return 0;
  }

  bool finish = false;
  int source, target;
  while (!finish) {
    int input = choose_menu(filename);
    switch (input) {
    case OP_HIST:
      print_histogram(image_hsv, image_width, image_height);
      break;
    case OP_CHANGE_COLOR:
      print_histogram(image_hsv, image_width, image_height);
      choose_colors(&source, &target);
      change_color(image_hsv, image_width, image_height, source, target);
      hsv_to_rgb_image(image_hsv, image_rgb, image_width, image_height);
      break;
    case OP_PRINT:
      print_image(image_rgb, image_width, image_height);
      break;
    case OP_SAVE:
      save_image(image_rgb, image_width, image_height);
      break;
    case OP_EXIT:
      finish = true;
      break;
    default:
      return 1; // This must not happen
    }
    printf("\n");
  }
  return 0;
}

bool in_range(int num, int a, int b) { return a <= num && num < b; }

float max_f(float a, float b) { return a > b ? a : b; }

float min_f(float a, float b) { return a < b ? a : b; }

bool abs_equal(float a, float b) { return fabsf(a - b) < ERROR; }

int compare_f(float a, float b) {
  if (abs_equal(a, b))
    return 0;
  else if (a > b)
    return 1;
  return -1;
}

int load_image(const char *filename, int image_rgb[][SIZE][SIZE],
               float image_hsv[][SIZE][SIZE], int *width, int *height) {
  FILE *image_fp = fopen(filename, "r");
  if (image_fp == NULL)
    return RES_ERROR;
  char signature[3]; // Must be "P3"
  fscanf(image_fp, "%s", signature);
  fscanf(image_fp, "%d %d", width, height);
  int channel_len;
  fscanf(image_fp, "%d", &channel_len);
  for (int y = 0; y < *height; y++)
    for (int x = 0; x < *width; x++) {
      for (int chan = 0; chan < 3; chan++)
        fscanf(image_fp, "%d", &image_rgb[chan][y][x]);
      rgb_to_hsv(image_rgb[0][y][x], image_rgb[1][y][x], image_rgb[2][y][x],
                 &image_hsv[0][y][x], &image_hsv[1][y][x], &image_hsv[2][y][x]);
    }
  fclose(image_fp);
  return RES_SUCCESS;
}

void save_image(int image_rgb[][SIZE][SIZE], int width, int height) {
  FILE *image_fp = fopen("output.ppm", "w");
  fprintf(image_fp, "P3 %d %d 255\n", width, height);
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      for (int chan = 0; chan < 3; chan++)
        fprintf(image_fp, "%d ", image_rgb[chan][y][x]);
  fclose(image_fp);
}

void print_image(int image_rgb[][SIZE][SIZE], int width, int height) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      set_output_color_rgb(image_rgb[0][y][x], image_rgb[1][y][x],
                           image_rgb[2][y][x]);
      printf("■");
    }
    printf("\n");
  }
  reset_output_color();
}

void print_histogram(float image_hsv[][SIZE][SIZE], int width, int height) {
  int pixel_count[12] = {0};
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      pixel_count[quantize_hue(image_hsv[0][y][x])] +=
          !abs_equal(image_hsv[1][y][x], 0);
  for (int i = 0; i < 12; i++) {
    printf("[%2d]", i);
    int r, g, b;
    hsv_to_rgb(30 * i, 1, 1, &r, &g, &b);
    set_output_color_rgb(r, g, b);
    for (int j = 0; j < pixel_count[i] / 10; j++)
      printf("■");
    reset_output_color();
    printf("%d\n", pixel_count[i]);
  }
}

void rgb_to_hsv(int r, int g, int b, float *h, float *s, float *v) {
  float normalized_r = (float)r / 255;
  float normalized_g = (float)g / 255;
  float normalized_b = (float)b / 255;
  float c_max = max_f(max_f(normalized_r, normalized_g), normalized_b);
  float c_min = min_f(min_f(normalized_r, normalized_g), normalized_b);
  float delta = c_max - c_min;
  float h_val, s_val, v_val;
  if (abs_equal(delta, 0))
    h_val = 0;
  else if (abs_equal(c_max, normalized_r))
    h_val = 60 * (normalized_g - normalized_b) / delta;
  else if (abs_equal(c_max, normalized_g))
    h_val = 60 * ((normalized_b - normalized_r) / delta + 2);
  else
    h_val = 60 * ((normalized_r - normalized_g) / delta + 4);
  if (abs_equal(c_max, 0))
    s_val = 0;
  else
    s_val = delta / c_max;
  v_val = c_max;
  if (compare_f(h_val, 0) < 0)
    h_val += 360;
  if (compare_f(h_val, 360) >= 0)
    h_val -= 360;
  if (h != NULL)
    *h = h_val;
  if (s != NULL)
    *s = s_val;
  if (v != NULL)
    *v = v_val;
}

int quantize_hue(float h) {
  float q = h / 30;
  for (int i = 0; i < 11; i++)
    if (compare_f(q, i) >= 0 && compare_f(q, i + 1) < 0)
      return i;
  return 11;
}

void change_color(float image_hsv[][SIZE][SIZE], int width, int height,
                  int source, int target) {
  int delta = target - source;
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++) {
      float *h = &image_hsv[0][y][x];
      *h += delta * 30;
      if (compare_f(*h, 0) < 0)
        *h += 360;
      else if (compare_f(*h, 360) >= 0)
        *h -= 360;
    }
}

void hsv_to_rgb(float h, float s, float v, int *r, int *g, int *b) {
  float c = v * s;
  float x = c * (1 - fabs(fmod(h / 60, 2) - 1));
  float m = v - c;
  float normalized[3] = {0};
  int quantized_h = quantize_hue(h);
  normalized[(quantized_h / 2 + 1) % 6 / 2] = c;
  normalized[2 - (quantized_h / 2 + 1) % 3] = x;
  if (r != NULL)
    *r = (normalized[0] + m) * 255;
  if (g != NULL)
    *g = (normalized[1] + m) * 255;
  if (b != NULL)
    *b = (normalized[2] + m) * 255;
}

void hsv_to_rgb_image(float image_hsv[][SIZE][SIZE],
                      int image_rgb[][SIZE][SIZE], int width, int height) {
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      hsv_to_rgb(image_hsv[0][y][x], image_hsv[1][y][x], image_hsv[2][y][x],
                 &image_rgb[0][y][x], &image_rgb[1][y][x], &image_rgb[2][y][x]);
}

void set_output_color_rgb(int r, int g, int b) {
  printf("\033[38;2;%d;%d;%dm", r, g, b);
}

void reset_output_color() { printf("\033[0m"); }

int choose_menu(const char *filename) {
  printf("=======================\n");
  printf("  IMAGE COLOR CHANGER\n");
  printf("=======================\n");
  printf("  1. Image Histogram\n");
  printf("  2. Change Color\n");
  printf("  3. Print Image\n");
  printf("  4. Save Image\n");
  printf("  5. Exit\n");
  printf("Loaded file: %s\n\n", filename);
  int input;
  while (true) {
    printf("Choose menu number >> ");
    scanf("%d", &input);
    if (in_range(input, OP_HIST, OP_EXIT + 1))
      break;
    printf("Wrong input!\n");
  }
  return input;
}

void choose_colors(int *source, int *target) {
  while (true) {
    printf("Choose source color >> ");
    scanf("%d", source);
    if (in_range(*source, 0, 12))
      break;
    printf("Wrong input!\n");
  }
  while (true) {
    printf("Choose target color >> ");
    scanf("%d", target);
    if (in_range(*target, 0, 12))
      break;
    printf("Wrong input!\n");
  }
}
