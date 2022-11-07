#include <stdio.h>
#define SIZE 20

void print_matrix(int matrix[][SIZE], int size) {
  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++)
      printf(" %2d", matrix[y][x]);
    printf("\n");
  }
}

int main() {
  int matrix[SIZE][SIZE], size;
  printf("Enter the size of square: ");
  scanf("%d", &size);
  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {
      if (y + x < size - 1)
        matrix[y][x] = 1;
      else if (y + x == size - 1)
        matrix[y][x] = 0;
      else
        matrix[y][x] = -1;
    }
  }
  print_matrix(matrix, size);
  return 0;
}
