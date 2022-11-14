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
#define OP_HIST 1         // print histogram
#define OP_CHANGE_COLOR 2 // change image color
#define OP_PRINT 3        // print the image
#define OP_SAVE 4         // save current image in memory
#define OP_EXIT 5         // exit the program

// Consts for results.
#define RES_SUCCESS 0 // the function was successful
#define RES_ERROR 1   // the function errored out

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

// Set terminal output color according to RGB values.
void set_output_color_rgb(int r, int g, int b);

// Reset terminal output color.
void reset_output_color();

// Print menus and get user input.
int choose_menu(const char *filename);

// Get user inputs for source and target colors.
void choose_colors(int *source, int *target);

int main() {
  char filename[MAX_PATH_LEN + 1]; // max length + null terminal
  printf("Enter input filename: ");
  scanf("%s", filename);

  // Using dynamic memory allocation might be a better idea.
  // Array dimensions are (C, H, W).
  int image_rgb[3][SIZE][SIZE], image_width, image_height;
  float image_hsv[3][SIZE][SIZE];

  int load_result =
      load_image(filename, image_rgb, image_hsv, &image_width, &image_height);
  if (load_result == RES_ERROR) {
    printf("File not found: %s\n", filename);
    return 0; // this prematuarely quits the program.
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
      // Print current histogram before prompting about colors.
      print_histogram(image_hsv, image_width, image_height);
      choose_colors(&source, &target);
      change_color(image_hsv, image_width, image_height, source, target);
      // The `image_hsv` array is changed, so propagate to `image_rgb`.
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
      return 1; // this must not happen
    }
    printf("\n"); // print newline for better aesthetic.
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
  return -1; // if a < b
}

int load_image(const char *filename, int image_rgb[][SIZE][SIZE],
               float image_hsv[][SIZE][SIZE], int *width, int *height) {
  FILE *image_fp = fopen(filename, "r");
  if (image_fp == NULL)
    return RES_ERROR; // report failure if file could not be opened
  char signature[3];  // must be "P3"
  fscanf(image_fp, "%s", signature); // input "P3" signature from file
  fscanf(image_fp, "%d %d", width,
         height);  // input width and height of the image
  int channel_len; // must be 255
  fscanf(image_fp, "%d", &channel_len);
  for (int y = 0; y < *height; y++)
    for (int x = 0; x < *width; x++) {
      for (int chan = 0; chan < 3; chan++)
        fscanf(image_fp, "%d", &image_rgb[chan][y][x]);
      // The whole pixel is loaded after the above loop. Now the pixel can be
      // converted to HSV color space.
      rgb_to_hsv(image_rgb[0][y][x], image_rgb[1][y][x], image_rgb[2][y][x],
                 &image_hsv[0][y][x], &image_hsv[1][y][x], &image_hsv[2][y][x]);
    }
  fclose(image_fp); // clean up file pointer
  return RES_SUCCESS;
}

void save_image(int image_rgb[][SIZE][SIZE], int width, int height) {
  FILE *image_fp = fopen("output.ppm", "w");
  fprintf(image_fp, "P3 %d %d 255\n", width, height);
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      for (int chan = 0; chan < 3; chan++)
        fprintf(image_fp, "%d ", image_rgb[chan][y][x]);
  fclose(image_fp); // clean up file pointer
}

void print_image(int image_rgb[][SIZE][SIZE], int width, int height) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      set_output_color_rgb(image_rgb[0][y][x], image_rgb[1][y][x],
                           image_rgb[2][y][x]);
      printf("■"); // each pixel is printed as a colored square
    }
    printf("\n"); // put a newline after a single line of the image is printed
  }
  reset_output_color(); // reset foreground color after dumping image to the
                        // terminal
}

void print_histogram(float image_hsv[][SIZE][SIZE], int width, int height) {
  // An array to hold number of pixels in specific h value interval.
  int pixel_count[12] = {0};
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      // Colors with zero saturation are ignored.
      pixel_count[quantize_hue(image_hsv[0][y][x])] +=
          !abs_equal(image_hsv[1][y][x], 0);
  for (int i = 0; i < 12; i++) {
    printf("[%2d]", i);
    int r, g, b;
    // Pre-calculating lookup tables might be a better idea.
    hsv_to_rgb(30 * i, 1, 1, &r, &g, &b);
    set_output_color_rgb(r, g, b); // set bar color

    // The length of histogram bars are one-tenth of the actual count.
    for (int j = 0; j < pixel_count[i] / 10; j++)
      printf("■");
    reset_output_color(); // reset color for prints afterwards
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
  if (abs_equal(delta, 0))
    *h = 0;
  // Error-tolerant comparison is not used because `c_max` is exactly same as
  // one of the `normalized_*` variable.
  else if (c_max == normalized_r)
    *h = 60 * (normalized_g - normalized_b) / delta;
  else if (c_max == normalized_g)
    *h = 60 * ((normalized_b - normalized_r) / delta + 2);
  else
    *h = 60 * ((normalized_r - normalized_g) / delta + 4);

  if (abs_equal(c_max, 0))
    *s = 0;
  else
    *s = delta / c_max;
  *v = c_max;

  // Normalize hue value. Using `fmod` might be a better idea.
  if (compare_f(*h, 0) < 0)
    *h += 360;
  if (compare_f(*h, 360) >= 0)
    *h -= 360;
}

int quantize_hue(float h) {
  float q = h / 30;
  for (int i = 0; i < 11; i++)
    if (compare_f(q, i) >= 0 &&
        compare_f(q, i + 1) < 0) // use error-tolerant comparison
      return i;
  return 11; // if nothing matched, the hue must be quantized to 11
}

void change_color(float image_hsv[][SIZE][SIZE], int width, int height,
                  int source, int target) {
  int delta = target - source;
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++) {
      float *h = &image_hsv[0][y][x]; // only hue value is necessary
      *h += delta * 30;               // change color
      // Normalize hue value to fit in [0, 360) interval.
      if (compare_f(*h, 0) < 0)
        *h += 360; // if hue is negative, add 360
      else if (compare_f(*h, 360) >= 0)
        *h -= 360; // if hue is greater or equal to 360, substract it
    }
}

void hsv_to_rgb(float h, float s, float v, int *r, int *g, int *b) {
  float c = v * s;
  float x = c * (1 - fabs(fmod(h / 60, 2) - 1));
  float m = v - c;
  float normalized[3] = {0};
  int quantized_h = quantize_hue(h);
  // Exploit symmetry to eliminate braches, which are detrimental to
  // performance.
  // The indices of c and x are as follows:
  //
  // h |11 | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |10
  // ==================================================
  // c | 0 | 0 | 0 | 0 | 1 | 1 | 1 | 1 | 2 | 2 | 2 | 2
  // x | 2 | 2 | 1 | 1 | 0 | 0 | 2 | 2 | 1 | 1 | 0 | 0
  //
  // As one can see, there is a clear pattern. The following code follows the
  // pattern using modulo operations.
  normalized[(quantized_h / 2 + 1) % 6 / 2] = c;
  normalized[2 - (quantized_h / 2 + 1) % 3] = x;
  *r = (normalized[0] + m) * 255;
  *g = (normalized[1] + m) * 255;
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
  while (true) { // loop until valid input
    printf("Choose menu number >> ");
    scanf("%d", &input);
    // Check if command is in the range.
    if (in_range(input, OP_HIST, OP_EXIT + 1))
      break;
    printf("Wrong input!\n");
  }
  return input;
}

void choose_colors(int *source, int *target) {
  while (true) { // loop until valid first color input
    printf("Choose source color >> ");
    scanf("%d", source);
    if (in_range(*source, 0, 12))
      break;
    printf("Wrong input!\n");
  }
  while (true) { // loop until valid second color input
    printf("Choose target color >> ");
    scanf("%d", target);
    if (in_range(*target, 0, 12))
      break;
    printf("Wrong input!\n");
  }
}
