#include <stdio.h>
#include <stdlib.h>

void print_matrix(int size, int (*matrix)[size]) {
  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++)
      printf(" %2d", matrix[y][x]);
    printf("\n");
  }
}

int main() {
  printf("Enter the size of square: ");
  int size;
  scanf("%d", &size);
  int(*matrix)[size];
  matrix = (int(*)[size])malloc(size * sizeof(*matrix));
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
  print_matrix(size, matrix);
  free(matrix);
  return 0;
}
